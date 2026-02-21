#ifndef EIGHTSER_BUILTIN_APPLY_FUNCTOR_HPP
#define EIGHTSER_BUILTIN_APPLY_FUNCTOR_HPP

#include <Eightser/Serializable.hpp>

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp>

namespace eightser
{

namespace apply
{

struct apply_functor_t {};

} // namespace apply

namespace meta
{

template <typename ApplyFunctorType> struct is_apply_functor : std::is_base_of<apply::apply_functor_t, ApplyFunctorType> {};

} // namespace meta

} // namespace eightser

CONDITIONAL_SERIALIZABLE_DECLARATION(::eightser::meta::is_apply_functor<std::decay_t<S>>::value)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVELOAD(apply_functor, ::eightser::meta::is_apply_functor<std::decay_t<S>>::value)
    BIN_SERIALIZABLE
    (
        apply_functor(archive);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_BUILTIN_APPLY_FUNCTOR_HPP
