#ifndef EIGHTSER_STANDARD_DEQUE_HPP
#define EIGHTSER_STANDARD_DEQUE_HPP

#include <cstdint> // uint64_t

#include <deque> // deque

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/Compress.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename ValueType, typename AllocatorType>), std::deque<ValueType, AllocatorType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(deque, (template <typename ValueType, typename AllocatorType>), std::deque<ValueType, AllocatorType>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size = deque.size();
        archive & size;

        ::eightser::compress::slow(archive, deque);
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(deque, (template <typename ValueType, typename AllocatorType>), std::deque<ValueType, AllocatorType>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size{};
        archive & size;

        deque.resize(size);
        ::eightser::compress::slow(archive, deque);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_DEQUE_HPP
