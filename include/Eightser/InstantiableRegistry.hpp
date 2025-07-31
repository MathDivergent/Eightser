#ifndef EIGHTSER_INSTANTIABLE_REGISTRY_HPP
#define EIGHTSER_INSTANTIABLE_REGISTRY_HPP

#include <unordered_map> // unordered_map

#include <Eightser/ArchiveBase.hpp>
#include <Eightser/Memory.hpp>

#include <Eightser/Hash.hpp>

#include <Eightser/PolymorphicArchive.hpp>

#include <Eightser/Instantiable.hpp>
#include <Eightser/InstantiableTraits.hpp>

// By default library will use 'INSTANTIABLE_TYPE*' and 'std::shared_ptr<INSTANTIABLE_TYPE>' types for general instancing,
// if you want to specify behaviour, just define own POINTER_HOLD_TYPES
// Note that: any used ptr type should be provided in '::eightser::memory' namespace
#ifndef POINTER_HOLD_TYPES
    #define POINTER_HOLD_TYPES INSTANTIABLE_TYPE*, std::shared_ptr<INSTANTIABLE_TYPE>
#endif // POINTER_HOLD_TYPES

namespace eightser
{

struct instantiable_registry_t final
{
public:
    template <typename PointerHoldType>
    struct instantiable_proxy_overload_t
    {
        using instantiable_pointer_type = typename memory::pointer_traits<PointerHoldType>::template pointer_template<INSTANTIABLE_TYPE>;

        instantiable_pointer_type(*xxclone)() = nullptr;
    };

    template <typename... PointerHoldTypes>
    struct instantiable_proxy_t : instantiable_proxy_overload_t<PointerHoldTypes>...
    {
        ::xxeightser_instantiable_traits_key_type key = ::xxeightser_instantiable_traits_base_key;

        void(*save)(ioarchive_t&, INSTANTIABLE_TYPE*) = nullptr;
        void(*load)(ioarchive_t&, INSTANTIABLE_TYPE*) = nullptr;
    };

public:
    using instantiable_proxy_type = instantiable_proxy_t<POINTER_HOLD_TYPES>;

public:
    std::unordered_map<::xxeightser_instantiable_traits_key_type, instantiable_proxy_type> all;
    std::unordered_map<std::uint64_t, instantiable_proxy_type> dynamic_all;

public:
    template <typename InstantiableType> struct is_instantiable
        : std::disjunction<meta::is_static_castable<InstantiableType*, INSTANTIABLE_TYPE*>,
                           std::is_convertible<InstantiableType*, INSTANTIABLE_TYPE*>> {};

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
            return memory::static_pointer_cast<INSTANTIABLE_TYPE>(pointer);
        };

        add_impl<InstantiableType, OtherPointerHoldTypes...>(proxy);
    }

public:
    template <typename InstantiableType>
    ::xxeightser_instantiable_traits_key_type key()
    {
        if constexpr (meta::is_key<InstantiableType>::value)
        {
            return ::xxeightser<InstantiableType>::key();
        }
        else
        {
            return EIGHTSER_TYPE_HASH(InstantiableType);
        }
    }

public:
    template <class InstantiableType>
    void add()
    {
        if constexpr (is_instantiable<InstantiableType>::value)
        {
            static auto lock = false; if (lock) return;
            lock = true;

            auto const instantiable_key = key<InstantiableType>();
            #ifdef EIGHTSER_DEBUG
            if (instantiable_key == ::xxeightser_instantiable_traits_base_key)
                throw "The 'eightser::instantiable_registry_t' must contains instance with valid key.";
            #endif // EIGHTSER_DEBUG
            instantiable_proxy_type proxy;

            add_impl<InstantiableType, POINTER_HOLD_TYPES>(proxy);

            proxy.key = instantiable_key;

            proxy.save = [](ioarchive_t& archive, INSTANTIABLE_TYPE* instance)
            {
                if constexpr (meta::is_static_castable<INSTANTIABLE_TYPE*, InstantiableType*>::value)
                {
                    archive << *memory::static_pointer_cast<InstantiableType>(instance);
                }
                else
                {
                    #ifdef EIGHTSER_RTTI_ENABLE
                    archive << *memory::dynamic_pointer_cast<InstantiableType>(instance);
                    #else
                    static_assert(meta::to_false<InstantiableType>(), "Serialization via virtual base not allowed.");
                    #endif // EIGHTSER_RTTI_ENABLE
                }
            };

            proxy.load = [](ioarchive_t& archive, INSTANTIABLE_TYPE* instance)
            {
                if constexpr (meta::is_static_castable<INSTANTIABLE_TYPE*, InstantiableType*>::value)
                {
                    archive >> *memory::static_pointer_cast<InstantiableType>(instance);
                }
                else
                {
                    #ifdef EIGHTSER_RTTI_ENABLE
                    archive >> *memory::dynamic_pointer_cast<InstantiableType>(instance);
                    #else
                    static_assert(meta::to_false<InstantiableType>(), "Deserialization via virtual base not allowed.");
                    #endif // EIGHTSER_RTTI_ENABLE
                }
            };

            all.emplace(instantiable_key, proxy);

            auto const hash = EIGHTSER_TYPE_HASH(InstantiableType);
            dynamic_all.emplace(hash, proxy);
        }
    }

    template <class InstantiableType>
    bool fixture()
    {
        #ifdef EIGHTSER_DEBUG
        if (std::negation<is_instantiable<InstantiableType>>::value)
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
        proxy.save(archive, memory::raw(pointer));
    }

    template <typename PointerType, typename PointerHoldType>
    void load(ioarchive_t& archive, PointerType& pointer, instantiable_proxy_type const& proxy, PointerHoldType& cache) const
    {
        clone(pointer, proxy);
        cache = pointer;

        proxy.load(archive, memory::raw(pointer));
    }

public:
    template <typename PointerType>
    void clone(PointerType& pointer, instantiable_proxy_type const& proxy) const
    {
        using pointer_traits = memory::pointer_traits<PointerType>;

        using serializable_type = typename pointer_traits::element_type;
        using pointer_hold_type = typename pointer_traits::template pointer_template<INSTANTIABLE_TYPE>;

        auto instance = proxy.instantiable_proxy_overload_t<pointer_hold_type>::xxclone();

        #ifdef EIGHTSER_RTTI_ENABLE
        pointer = memory::dynamic_pointer_cast<serializable_type>(instance);
        #else
        pointer = memory::static_pointer_cast<serializable_type>(instance);
        #endif // EIGHTSER_RTTI_ENABLE
    }

    template <typename PointerType, typename PointerHoldType>
    void assign(PointerType& pointer, PointerHoldType const& address) const
    {
        using serializable_type = typename memory::pointer_traits<PointerType>::element_type;

        #ifdef EIGHTSER_RTTI_ENABLE
        pointer = memory::dynamic_pointer_cast<serializable_type>(address);
        #else
        pointer = memory::static_pointer_cast<serializable_type>(address);
        #endif // EIGHTSER_RTTI_ENABLE
    }
};

extern instantiable_registry_t* instantiable_registry();

} // namespace eightser

#endif // EIGHTSER_INSTANTIABLE_REGISTRY_HPP
