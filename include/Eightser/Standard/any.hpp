#ifndef EIGHTSER_STANDARD_ANY_HPP
#define EIGHTSER_STANDARD_ANY_HPP

#if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)

#include <any> // any

#include <Eightser/Serializable.hpp>
#include <Eightser/Hash.hpp>

#include <Eightser/Standard/AnyRegistry.hpp>

// please, use 'eightser::serializable' for type any registry before std::any serialization
VISIBLE_SERIALIZABLE_DECLARATION((EIGHTSER_API), std::any)
SERIALIZABLE_DECLARATION_INIT()

#endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE

#endif // EIGHTSER_STANDARD_ANY_HPP
