#ifndef EIGHTSER_INSTANTIABLE_HPP
#define EIGHTSER_INSTANTIABLE_HPP

#include <Eightser/Memory.hpp>
#include <Eightser/InstantiableTraits.hpp>

namespace eightser
{

struct EIGHTSER_API instantiable_t
{
    virtual ~instantiable_t();
};

template <typename PointerType>
auto to_instantiable(PointerType const& pointer)
{
    using serializable_type = typename memory::pointer_traits<PointerType>::element_type;
    using instantiable_type = typename ::xxeightser_instantiable_traits<serializable_type>::instantiable_type;

    if constexpr (meta::is_pointer_to_polymorphic<PointerType>::value)
    {
        return memory::static_pointer_cast<instantiable_type>(pointer);
    }
    else
    {
        return memory::static_pointer_cast<instantiable_type>(memory::static_pointer_cast<void>(pointer));
    }
}

} // namespace eightser

#endif // EIGHTSER_INSTANTIABLE_HPP
