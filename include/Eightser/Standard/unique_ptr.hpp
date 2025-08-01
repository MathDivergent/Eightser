#ifndef EIGHTSER_STANDARD_UNIQUE_PTR_HPP
#define EIGHTSER_STANDARD_UNIQUE_PTR_HPP

#include <memory> // unique_ptr

#include <Eightser/Serializable.hpp>
#include <Eightser/Memory.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename ElementType, typename DeleterType>), std::unique_ptr<ElementType, DeleterType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE(save, unique_ptr, (template <typename ElementType, typename DeleterType>), std::unique_ptr<ElementType, DeleterType>)
    SERIALIZATION
    (
        auto data = unique_ptr.get();
        archive & data;
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE(load, unique_ptr, (template <typename ElementType, typename DeleterType>), std::unique_ptr<ElementType, DeleterType>)
    SERIALIZATION
    (
        ElementType* data = nullptr;
        archive & data;

        unique_ptr.reset(data);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_UNIQUE_PTR_HPP
