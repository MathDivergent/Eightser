#ifndef EIGHTSER_BUILTIN_DATA_TRACK_HPP
#define EIGHTSER_BUILTIN_DATA_TRACK_HPP

#include <memory> // addressof

#include <Eightser/ArchiveBase.hpp>

#include <Eightser/InstantiableTraits.hpp>
#include <Eightser/InstantiableRegistry.hpp>

#include <Eightser/BuiltIn/ApplyFunctor.hpp>

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp>

namespace eightser
{

namespace detail
{

template <class ArchiveType, typename PointerType,
          EIGHTSER_REQUIRES(meta::is_oarchive<ArchiveType>::value)>
std::uintptr_t tracking_key(ArchiveType& archive, PointerType& pointer)
{
    auto key = reinterpret_cast<std::uintptr_t>
    (
        memory::raw(hold_type_erasure(pointer))
    );

    archive & key;
    return key;
}

template <class ArchiveType, typename PointerType,
          EIGHTSER_REQUIRES(meta::is_iarchive<ArchiveType>::value)>
std::uintptr_t tracking_key(ArchiveType& archive, [[maybe_unused]] PointerType& pointer)
{
    #ifdef EIGHTSER_DEBUG
    if (pointer != nullptr)
        throw "The read pointer must be initialized to nullptr.";
    #endif // EIGHTSER_DEBUG

    std::uintptr_t key{};
    archive & key;

    return key;
}

template <class ArchiveType, typename PointerType, typename PointerHoldType>
void native_assign(ArchiveType&, PointerType& pointer, PointerHoldType& address)
{
    if constexpr (meta::is_pointer_to_polymorphic<PointerType>::value)
    {
        instantiable_registry()->assign(pointer, address);
    }
    else
    {
        // safe when the address has pointer element type
        pointer = memory::static_pointer_cast<typename memory::pointer_traits<PointerType>::element_type>
        (
            memory::static_pointer_cast<void>(address)
        );
    }
}

} // namespace detail

template <class ArchiveType, typename PointerType,
          EIGHTSER_REQUIRES(std::conjunction_v<meta::is_oarchive<ArchiveType>,
                                               meta::is_pointer_to_any<PointerType>>)>
void strict(ArchiveType& archive, PointerType& pointer)
{
    if (pointer == nullptr)
        throw "The write pointer must be allocated.";

    if constexpr (meta::is_pointer_to_polymorphic<PointerType>::value)
    {
        auto const hash = EIGHTSER_EXPRESSION_HASH(*pointer);
        auto const& proxy = instantiable_registry()->dynamic_all.at(hash);

        auto key = proxy.key;
        archive & key;

        instantiable_registry()->save(archive, pointer, proxy);
    }
    else
    {
        archive & (*pointer);
    }
}

template <class ArchiveType, typename PointerType, typename PointerHoldType,
          EIGHTSER_REQUIRES(std::conjunction_v<meta::is_iarchive<ArchiveType>,
                                               meta::is_pointer_to_any<PointerType>>)>
void strict(ArchiveType& archive, PointerType& pointer, PointerHoldType& cache)
{
    #ifdef EIGHTSER_GARBAGE_CHECK_ENABLE
    if (pointer != nullptr)
        throw "The read pointer must be initialized to nullptr.";
    #endif // EIGHTSER_GARBAGE_CHECK_ENABLE

    if constexpr (meta::is_pointer_to_polymorphic<PointerType>::value)
    {
        ::xxeightser_instantiable_traits_key_type key{};
        archive & key;

        auto const& proxy = instantiable_registry()->all.at(key);

        instantiable_registry()->load(archive, pointer, proxy, cache);
    }
    else
    {
        memory::allocate(pointer);
        cache = hold_type_erasure(pointer);

        archive & (*pointer);
    }
}

template <class ArchiveType, typename PointerType,
          EIGHTSER_REQUIRES(std::conjunction_v<meta::is_oarchive<ArchiveType>,
                                               meta::is_pointer_to_any<PointerType>>)>
void track(ArchiveType& archive, PointerType& pointer)
{
    // need to overload tracking
    using pointer_hold_type = typename memory::pointer_traits<PointerType>::template pointer_template<INSTANTIABLE_TYPE>;

    auto const key = detail::tracking_key(archive, pointer); // serialize refer info
    if (not key) return;

    #ifdef EIGHTSER_DEBUG
    if (archive.tracking().is_mixed(key, pointer))
        throw "Mixed pointer tracking is not allowed.";
    #endif // EIGHTSER_DEBUG

    auto& is_tracking = archive.tracking().pointer(pointer_hold_type())[key];
    if (not is_tracking)
    {
        is_tracking = true;
        strict(archive, pointer); // call the strict serialization of not tracking pointer
    }
}

template <class ArchiveType, typename SerializableType,
          EIGHTSER_REQUIRES(std::conjunction_v<meta::is_oarchive<ArchiveType>,
                                               std::negation<meta::is_pointer_to_any<SerializableType>>>)>
void track(ArchiveType& archive, SerializableType& data)
{
    using pointer_hold_type = INSTANTIABLE_TYPE*; // need to overload tracking

    auto key = reinterpret_cast<std::uintptr_t>
    (
        hold_type_erasure(std::addressof(data))
    );

    auto& is_tracking = archive.tracking().pointer(pointer_hold_type())[key];
    if (is_tracking)
        throw "The write tracking data is already tracked.";

    is_tracking = true;

    archive & key;
    archive & data;
}

template <class ArchiveType, typename PointerType,
          EIGHTSER_REQUIRES(std::conjunction_v<meta::is_iarchive<ArchiveType>,
                                               meta::is_pointer_to_any<PointerType>>)>
void track(ArchiveType& archive, PointerType& pointer)
{
    // need to overload tracking
    using pointer_hold_type = typename memory::pointer_traits<PointerType>::template pointer_template<INSTANTIABLE_TYPE>;

    #ifdef EIGHTSER_GARBAGE_CHECK_ENABLE
    if (pointer != nullptr)
        throw "The read track pointer must be initialized to nullptr.";
    #endif // EIGHTSER_GARBAGE_CHECK_ENABLE

    auto const key = detail::tracking_key(archive, pointer); // deserialize refer info
    if (not key) return;

    auto& address = archive.tracking().pointer(pointer_hold_type())[key];
    if (address == nullptr)
    {
        // call the strict serialization of not tracking pointer
        strict(archive, pointer, address);
    }
    else
    {
        detail::native_assign(archive, pointer, address);
    }
}

template <class ArchiveType, typename SerializableType,
          EIGHTSER_REQUIRES(std::conjunction_v<meta::is_iarchive<ArchiveType>,
                                               std::negation<meta::is_pointer_to_any<SerializableType>>>)>
void track(ArchiveType& archive, SerializableType& data)
{
    using pointer_hold_type = INSTANTIABLE_TYPE*; // need to overload tracking

    std::uintptr_t key{};
    archive & key;

    auto const pointer = std::addressof(data);
    auto& address = archive.tracking().pointer(pointer_hold_type())[key];

    if (address != nullptr)
        throw  "The read tracking data is already tracked.";

    address = hold_type_erasure(pointer);
    archive & data;
}

template <class ArchiveType, typename PointerType,
          EIGHTSER_REQUIRES(meta::is_pointer_to_any<PointerType>::value)>
void raw(ArchiveType& archive, PointerType& pointer)
{
    if (detail::tracking_key(archive, pointer)) // serialize refer info
    {
        typename memory::pointer_traits<PointerType>::template pointer_template<INSTANTIABLE_TYPE> cache = nullptr; // mock
        strict(archive, pointer, cache);
    }
}

namespace apply
{

template <typename SerializableType>
struct strict_functor_t : public apply_functor_t
{
    SerializableType& data;

    strict_functor_t(SerializableType& data) noexcept : data(data) {}

    template <class ArchiveType>
    void operator() (ArchiveType& archive) const { strict(archive, data); }
};

template <typename SerializableType>
struct track_functor_t : apply_functor_t
{
    SerializableType& data;

    track_functor_t(SerializableType& data) noexcept : data(data) {}

    template <class ArchiveType>
    void operator() (ArchiveType& archive) const { track(archive, data); }
};

template <typename SerializableType>
struct raw_functor_t : apply_functor_t
{
    SerializableType& data;

    raw_functor_t(SerializableType& data) noexcept : data(data) {}

    template <class ArchiveType>
    void operator() (ArchiveType& archive) const { raw(archive, data); }
};

} // namespace apply

template <typename SerializableType>
apply::strict_functor_t<SerializableType> strict(SerializableType& data) noexcept { return { data }; }

template <typename SerializableType>
apply::track_functor_t<SerializableType> track(SerializableType& data) noexcept { return { data }; }

template <typename SerializableType>
apply::raw_functor_t<SerializableType> raw(SerializableType& data) noexcept { return { data }; }

} // namespace eightser

#endif // EIGHTSER_BUILTIN_DATA_TRACK_HPP
