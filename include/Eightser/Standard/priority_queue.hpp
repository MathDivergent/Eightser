#ifndef EIGHTSER_STANDARD_PRIORITY_QUEUE_HPP
#define EIGHTSER_STANDARD_PRIORITY_QUEUE_HPP

#include <queue> // priority_queue

#include <Eightser/Serializable.hpp>

#include <Eightser/Standard/Detail/Meta.hpp>

// default container for priority_queue
#include <Eightser/Standard/vector.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename ValueType, class ContainerType>), std::priority_queue<ValueType, ContainerType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE(saveload, priority_queue, (template <typename ValueType, class ContainerType>), std::priority_queue<ValueType, ContainerType>)
    SERIALIZATION
    (
        archive & ::eightser::meta::underlying(priority_queue);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_PRIORITY_QUEUE_HPP
