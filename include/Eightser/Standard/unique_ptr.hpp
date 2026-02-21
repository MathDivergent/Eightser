#ifndef EIGHTSER_STANDARD_UNIQUE_PTR_HPP
#define EIGHTSER_STANDARD_UNIQUE_PTR_HPP

#include <memory> // unique_ptr

#include <Eightser/Serializable.hpp>
#include <Eightser/Memory.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename ElementType, typename DeleterType>), std::unique_ptr<ElementType, DeleterType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(unique_ptr, (template <typename ElementType, typename DeleterType>), std::unique_ptr<ElementType, DeleterType>)
    BIN_SERIALIZABLE
    (
        auto data = unique_ptr.get();
        archive & data;
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(unique_ptr, (template <typename ElementType, typename DeleterType>), std::unique_ptr<ElementType, DeleterType>)
    BIN_SERIALIZABLE
    (
        ElementType* data = nullptr;
        archive & data;

        unique_ptr.reset(data);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_UNIQUE_PTR_HPP
