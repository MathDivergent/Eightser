#ifndef EIGHTSER_STANDARD_WEAK_PTR_HPP
#define EIGHTSER_STANDARD_WEAK_PTR_HPP

#include <memory> // weak_ptr

#include <Eightser/Serializable.hpp>
#include <Eightser/Memory.hpp>

// serialization of shared_ptr
#include <Eightser/Standard/shared_ptr.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename ElementType>, std::weak_ptr<ElementType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(weak_ptr, template <typename ElementType>, std::weak_ptr<ElementType>)
    BIN_SERIALIZABLE
    (
        auto sptr = weak_ptr.lock();
        archive & sptr;
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(weak_ptr, template <typename ElementType>, std::weak_ptr<ElementType>)
    BIN_SERIALIZABLE
    (
        std::shared_ptr<ElementType> sptr;
        archive & sptr;

        weak_ptr = sptr;
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_WEAK_PTR_HPP
