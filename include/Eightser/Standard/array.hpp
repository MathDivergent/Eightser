#ifndef EIGHTSER_STANDARD_ARRAY_HPP
#define EIGHTSER_STANDARD_ARRAY_HPP

//#include <array> // array

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/Compress.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename ValueType, std::size_t SizeValue>), std::array<ValueType, SizeValue>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE(saveload, array, (template <typename ValueType, std::size_t SizeValue>), std::array<ValueType, SizeValue>)
    SERIALIZATION
    (
        ::eightser::compress::zip(archive, array);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_ARRAY_HPP
