#ifndef EIGHTSER_STANDARD_STACK_HPP
#define EIGHTSER_STANDARD_STACK_HPP

#include <stack> // stack

#include <Eightser/Serializable.hpp>

#include <Eightser/Standard/Detail/Meta.hpp>

// default container for stack
#include <Eightser/Standard/deque.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename ValueType, class ContainerType>), std::stack<ValueType, ContainerType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVELOAD(stack, (template <typename ValueType, class ContainerType>), std::stack<ValueType, ContainerType>)
    BIN_SERIALIZABLE
    (
        archive & ::eightser::meta::underlying(stack);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_STACK_HPP
