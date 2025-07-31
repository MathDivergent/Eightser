#ifndef EIGHTSER_INSTANTIABLE_HPP
#define EIGHTSER_INSTANTIABLE_HPP

#include <Eightser/Memory.hpp>

// By default library will use instantiable_t type for general instancing,
// if you want to specify behaviour, just define own INSTANTIABLE_TYPE
// Note that: any defined INSTANTIABLE_TYPE must have virtual destructor
#ifndef INSTANTIABLE_TYPE
    #define INSTANTIABLE_TYPE ::eightser::instantiable_t
#endif // INSTANTIABLE_TYPE

namespace eightser
{

struct instantiable_t { virtual ~instantiable_t() = default; };


template <typename PointerType>
typename memory::pointer_traits<PointerType>::template pointer_template<INSTANTIABLE_TYPE> hold_type_erasure(PointerType const& pointer)
{
    if constexpr (meta::is_pointer_to_polymorphic<PointerType>::value)
    {
        return pointer;
    }
    else
    {
        return memory::static_pointer_cast<INSTANTIABLE_TYPE>(memory::static_pointer_cast<void>(pointer));
    }
}

} // namespace eightser

#endif // EIGHTSER_INSTANTIABLE_HPP
