#ifndef EIGHTSER_STANDARD_LIST_HPP
#define EIGHTSER_STANDARD_LIST_HPP

#include <cstdint> // uint64_t

#include <list> // list

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/Compress.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename ValueType, typename AllocatorType>), std::list<ValueType, AllocatorType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(list, (template <typename ValueType, typename AllocatorType>), std::list<ValueType, AllocatorType>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size = list.size();
        archive & size;

        ::eightser::compress::slow(archive, list);
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(list, (template <typename ValueType, typename AllocatorType>), std::list<ValueType, AllocatorType>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size{};
        archive & size;

        list.resize(size);
        ::eightser::compress::slow(archive, list);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_LIST_HPP
