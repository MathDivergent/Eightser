#ifndef EIGHTSER_HASH_HPP
#define EIGHTSER_HASH_HPP

#include <cstdint> // uint32_t, uint64_t

#ifdef EIGHTSER_RTTI_ENABLE
#include <typeinfo> // type_info
#endif // EIGHTSER_RTTI_ENABLE


template <typename SerializableType, typename HashType = std::uint64_t>
struct xxeightser_type_hash_t
{
    HashType operator()() const
    {
    #ifdef EIGHTSER_RTTI_ENABLE
        return typeid(SerializableType).hash_code();
    #else
        #warning "type hash not defined, behaviour undefined"
        return -1; // mock
    #endif // EIGHTSER_RTTI_ENABLE
    }
};

template <typename ExpressionType, typename HashType = std::uint64_t>
struct xxeightser_expression_hash_t
{
    HashType operator()(ExpressionType const& expression) const
    {
    #ifdef EIGHTSER_RTTI_ENABLE
        return typeid(expression).hash_code();
    #else
        #warning "expression hash not defined, behaviour undefined"
        return -1; // mock
    #endif // EIGHTSER_RTTI_ENABLE
    }
};

#endif // EIGHTSER_HASH_HPP
