#ifndef EIGHTSER_BUILTIN_HIERARCHY_TRACK_HPP
#define EIGHTSER_BUILTIN_HIERARCHY_TRACK_HPP

#include <Eightser/ArchiveBase.hpp>
#include <Eightser/Memory.hpp>
#include <Eightser/Hash.hpp>

#include <Eightser/Instantiable.hpp>

#include <Eightser/BuiltIn/ApplyFunctor.hpp>

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp>

namespace eightser
{

template <class BaseType, class ArchiveType, class DerivedType,
          EIGHTSER_REQUIRES(std::is_base_of<BaseType, DerivedType>::value)>
void base(ArchiveType& archive, DerivedType& object)
{
    archive & ::xxeightser_cast_to_non_public_base::call<BaseType>(object);
}

template <class BaseType, class ArchiveType, class DerivedType,
          EIGHTSER_REQUIRES(std::is_base_of<BaseType, DerivedType>::value)>
void virtual_base(ArchiveType& archive, DerivedType& object)
{
    #ifdef EIGHTSER_PTRTRACK_ENABLE
    auto address = hold_type_erasure(std::addressof(object));

    auto const key = reinterpret_cast<std::uintptr_t>(address);
    auto const traits = EIGHTSER_TYPE_HASH(BaseType);

    auto& is_tracking = archive.tracking().hierarchy()[key][traits];
    if (not is_tracking)
    {
        is_tracking = true;
        base<BaseType>(archive, object);
    }
    #else
    if (EIGHTSER_EXPRESSION_HASH(object) == EIGHTSER_TYPE_HASH(DerivedType))
        base<Base>(archive, object);
    #endif // EIGHTSER_PTRTRACK_ENABLE
}

namespace detail
{

template <class BaseType, class ArchiveType, class DerivedType>
void native_base(ArchiveType& archive, DerivedType& object_with_base)
{
    if constexpr (meta::is_virtual_base_of<BaseType, DerivedType>::value)
    {
        virtual_base<BaseType>(archive, object_with_base);
    }
    else
    {
        base<BaseType>(archive, object_with_base);
    }
}

} // namespace detail

namespace apply
{

template <class DerivedType, class BaseType>
struct base_functor_t : apply_functor_t
{
    DerivedType& object;

    base_functor_t(DerivedType& object) noexcept : object(object) {}

    template <class ArchiveType>
    void operator() (ArchiveType& archive) const { base<BaseType>(archive, object); }
};

template <class DerivedType, class BaseType>
struct virtual_base_functor_t : apply_functor_t
{
    DerivedType& object;

    virtual_base_functor_t(DerivedType& object) noexcept : object(object) {}

    template <class ArchiveType>
    void operator() (ArchiveType& archive) const { virtual_base<BaseType>(archive, object); }
};

} // namespace apply

template <class BaseType, class DerivedType,
          EIGHTSER_REQUIRES(std::is_base_of_v<BaseType, DerivedType>)>
apply::base_functor_t<DerivedType, BaseType> base(DerivedType& object) noexcept { return { object }; }

template <class BaseType, class DerivedType,
          EIGHTSER_REQUIRES(std::is_base_of_v<BaseType, DerivedType>)>
apply::virtual_base_functor_t<DerivedType, BaseType> virtual_base(DerivedType& object) noexcept { return { object }; }

// default empty impl
template <class ArchiveType, class DerivedType>
void hierarchy(ArchiveType&, DerivedType&) noexcept { /*pass*/ }

// Variadic native_base function
template <class BaseType, class... BaseTypes, class ArchiveType, class DerivedType,
          EIGHTSER_REQUIRES(meta::is_derived_of<DerivedType, BaseType, BaseTypes...>::value)>
void hierarchy(ArchiveType& archive, DerivedType& object)
{
    detail::native_base<BaseType>(archive, object);
    hierarchy<BaseTypes...>(archive, object);
}

namespace apply
{

template <class DerivedType, class BaseType, class... BaseTypes>
struct hierarchy_functor_t : apply_functor_t
{
    DerivedType& object;

    hierarchy_functor_t(DerivedType& object) noexcept : object(object) {}

    template <class ArchiveType>
    void operator() (ArchiveType& archive) const { hierarchy<BaseType, BaseTypes...>(archive, object); }
};

} // namespace apply

template <class BaseType, class... BaseTypes, class DerivedType,
          EIGHTSER_REQUIRES(meta::is_derived_of<DerivedType, BaseType, BaseTypes...>::value)>
apply::hierarchy_functor_t<DerivedType, BaseType, BaseTypes...> hierarchy(DerivedType& object) noexcept
{
    return { object };
}

} // namespace eightser

#endif // EIGHTSER_BUILTIN_HIERARCHY_TRACK_HPP
