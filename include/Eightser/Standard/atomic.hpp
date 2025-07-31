#ifndef EIGHTSER_STANDARD_ATOMIC_HPP
#define EIGHTSER_STANDARD_ATOMIC_HPP

#include <atomic> // atomic

#include <Eightser/Serializable.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename ValueType>, std::atomic<ValueType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE(save, atomic, template <typename ValueType>, std::atomic<ValueType>)
    SERIALIZATION
    (
        auto object = atomic.load();
        archive & object;
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE(load, atomic, template <typename ValueType>, std::atomic<ValueType>)
    SERIALIZATION
    (
        ValueType value{};
        archive & value;

        atomic.store(value);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_ATOMIC_HPP
