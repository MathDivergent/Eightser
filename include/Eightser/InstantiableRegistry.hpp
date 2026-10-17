#ifndef EIGHTSER_INSTANTIABLE_REGISTRY_HPP
#define EIGHTSER_INSTANTIABLE_REGISTRY_HPP

#include <unordered_map> // unordered_map

#include <Eightser/ArchiveBase.hpp>
#include <Eightser/Memory.hpp>

#include <Eightser/Hash.hpp>

#include <Eightser/PolymorphicArchive.hpp>

#include <Eightser/Instantiable.hpp>

// By default library will use 'void*' and 'std::shared_ptr<void>' types for general instancing,
// if you want to specify behaviour, just define own POINTER_HOLD_TYPES
// Note that: any used ptr type should be provided in '::eightser::memory' namespace
#ifndef POINTER_HOLD_TYPES
    #define POINTER_HOLD_TYPES void*, std::shared_ptr<void>
#endif // POINTER_HOLD_TYPES

namespace eightser
{

struct EIGHTSER_API instantiable_registry_t final
{
public:
    template <typename PointerHoldType>
    struct instantiable_proxy_overload_t
    {
        using instantiable_pointer_type = typename memory::pointer_traits<PointerHoldType>::template pointer_template<void>;

        instantiable_pointer_type(*xxclone)() = nullptr;
    };

    template <typename... PointerHoldTypes>
    struct instantiable_proxy_t : instantiable_proxy_overload_t<PointerHoldTypes>...
    {
        ::xxeightser_instantiable_traits_key_type key = ::xxeightser_instantiable_traits_base_key;

        void(*save)(ioarchive_t&, void*) = nullptr;
        void(*load)(ioarchive_t&, void*) = nullptr;
    };

public:
    using instantiable_proxy_type = instantiable_proxy_t<POINTER_HOLD_TYPES>;

public:
    std::unordered_map<::xxeightser_instantiable_traits_key_type, instantiable_proxy_type> all;
    std::unordered_map<std::uint64_t, instantiable_proxy_type> rtti_all;

public:
    template <typename DerivedInstantiableType,
              class BaseInstantiableType = ::xxeightser_instantiable_traits<DerivedInstantiableType>::instantiable_type> 
    struct is_instantiable
        : std::disjunction<meta::is_static_castable<DerivedInstantiableType*, BaseInstantiableType*>,
                           std::is_convertible<DerivedInstantiableType*, BaseInstantiableType*>> {};

private:
    template <class InstantiableType>
    void add_impl(instantiable_proxy_type&) { /*pass*/ }

    template <class InstantiableType, typename OtherPointerHoldType, typename... OtherPointerHoldTypes>
    void add_impl(instantiable_proxy_type& proxy)
    {
        proxy.instantiable_proxy_overload_t<OtherPointerHoldType>::xxclone = []
        {
            typename memory::pointer_traits<OtherPointerHoldType>::template pointer_template<InstantiableType> pointer = nullptr;
            memory::allocate(pointer);
            return memory::static_pointer_cast<void>(to_instantiable(pointer));
        };

        add_impl<InstantiableType, OtherPointerHoldTypes...>(proxy);
    }

private:
    template <typename, typename enable = void> struct has_key : std::false_type {};
    template <typename SerializableType>
    struct has_key<SerializableType, std::void_t<decltype(&::xxeightser<SerializableType>::key)>> : std::true_type {};

public:
    template <class InstantiableType>
    ::xxeightser_instantiable_traits_key_type key()
    {
        if constexpr (has_key<InstantiableType>::value)
        {
            return ::xxeightser<InstantiableType>::key();
        }
        else
        {
            return ::xxeightser_type_hash_t<InstantiableType>{}();
        }
    }

public:
    template <class DerivedInstantiableType>
    void add()
    {
        if constexpr (is_instantiable<DerivedInstantiableType>::value)
        {
            using base_instantiable_type = ::xxeightser_instantiable_traits<DerivedInstantiableType>::instantiable_type;

            static auto lock = false; if (lock) return;
            lock = true;

            auto const instantiable_key = key<DerivedInstantiableType>();
            #ifdef EIGHTSER_DEBUG
            if (instantiable_key == ::xxeightser_instantiable_traits_base_key)
                throw "The 'eightser::instantiable_registry_t' must contains instance with valid key.";
            #endif // EIGHTSER_DEBUG
            instantiable_proxy_type proxy;

            add_impl<DerivedInstantiableType, POINTER_HOLD_TYPES>(proxy);

            proxy.key = instantiable_key;

            proxy.save = [](ioarchive_t& archive, void* address)
            {
                [[maybe_unused]] auto instantiable = to_instantiable(address);

                if constexpr (meta::is_static_castable<base_instantiable_type*, DerivedInstantiableType*>::value)
                {
                    archive << *memory::static_pointer_cast<DerivedInstantiableType>(instantiable);
                }
                else
                {
                    #ifdef EIGHTSER_RTTI_ENABLE
                    archive << *memory::dynamic_pointer_cast<DerivedInstantiableType>(instantiable);
                    #else
                    static_assert(meta::to_false<DerivedInstantiableType>(), "Serialization via virtual base not allowed.");
                    #endif // EIGHTSER_RTTI_ENABLE
                }
            };

            proxy.load = [](ioarchive_t& archive, void* address)
            {
                [[maybe_unused]] auto instantiable = to_instantiable(address);

                if constexpr (meta::is_static_castable<base_instantiable_type*, DerivedInstantiableType*>::value)
                {
                    archive >> *memory::static_pointer_cast<DerivedInstantiableType>(instantiable);
                }
                else
                {
                    #ifdef EIGHTSER_RTTI_ENABLE
                    archive >> *memory::dynamic_pointer_cast<DerivedInstantiableType>(instantiable);
                    #else
                    static_assert(meta::to_false<DerivedInstantiableType>(), "Deserialization via virtual base not allowed.");
                    #endif // EIGHTSER_RTTI_ENABLE
                }
            };

            all.emplace(instantiable_key, proxy);

            auto const hash = ::xxeightser_type_hash_t<DerivedInstantiableType>{}();
            rtti_all.emplace(hash, proxy);
        }
    }

    template <class InstantiableType>
    bool fixture()
    {
        #ifdef EIGHTSER_DEBUG
        if constexpr (std::negation_v<is_instantiable<InstantiableType>>)
            throw "The polymorphic 'InstantiableType' type is not convertible to 'instantiable_t'.";
        #endif // EIGHTSER_DEBUG

        if (all.find(key<InstantiableType>()) != all.end())
        #ifdef EIGHTSER_DEBUG
            throw "The 'eightser::instantiable_registry_t' must contains instance with unique key.";
        #else
            return false;
        #endif // EIGHTSER_DEBUG

        add<InstantiableType>();
        return true;
    }

    template <typename PointerType>
    void save(ioarchive_t& archive, PointerType& pointer, instantiable_proxy_type const& proxy) const
    {
        proxy.save(archive, memory::raw(to_instantiable(pointer)));
    }

    template <typename PointerType, typename PointerHoldType>
    void load(ioarchive_t& archive, PointerType& pointer, instantiable_proxy_type const& proxy, PointerHoldType& registry_element_fixture) const
    {
        clone(pointer, proxy);
        auto instantiable = to_instantiable(pointer);

        registry_element_fixture = instantiable;
        proxy.load(archive, memory::raw(instantiable));
    }

public:
    template <typename PointerType>
    void clone(PointerType& pointer, instantiable_proxy_type const& proxy) const
    {
        using pointer_traits = memory::pointer_traits<PointerType>;

        using serializable_type = typename pointer_traits::element_type;
        using pointer_hold_type = typename pointer_traits::template pointer_template<void>;

        auto instantiable = to_instantiable(proxy.instantiable_proxy_overload_t<pointer_hold_type>::xxclone());

        #ifdef EIGHTSER_RTTI_ENABLE
        pointer = memory::dynamic_pointer_cast<serializable_type>(instantiable);
        #else
        pointer = memory::static_pointer_cast<serializable_type>(instantiable);
        #endif // EIGHTSER_RTTI_ENABLE
    }

    template <typename PointerType, typename PointerHoldType>
    void assign(PointerType& pointer, PointerHoldType const& address) const
    {
        using serializable_type = typename memory::pointer_traits<PointerType>::element_type;

        auto instantiable = to_instantiable(address);

        #ifdef EIGHTSER_RTTI_ENABLE
        pointer = memory::dynamic_pointer_cast<serializable_type>(instantiable);
        #else
        pointer = memory::static_pointer_cast<serializable_type>(instantiable);
        #endif // EIGHTSER_RTTI_ENABLE
    }
};

extern EIGHTSER_API instantiable_registry_t* instantiable_registry();

} // namespace eightser

#endif // EIGHTSER_INSTANTIABLE_REGISTRY_HPP
