#ifndef EIGHTSER_STANDARD_SHARED_PTR_HPP
#define EIGHTSER_STANDARD_SHARED_PTR_HPP

#include <memory> // shared_ptr

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/DataTrack.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename ElementType>, std::shared_ptr<ElementType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE(saveload, shared_ptr, template <typename ElementType>, std::shared_ptr<ElementType>)
    ::eightser::track(archive, shared_ptr);
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_SHARED_PTR_HPP
