#ifndef EIGHTSER_STANDARD_VALARRAY_HPP
#define EIGHTSER_STANDARD_VALARRAY_HPP

#include <cstdint> // uint64_t

#include <valarray> // valarray

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/Compress.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename ValueType>, std::valarray<ValueType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(valarray, template <typename ValueType>, std::valarray<ValueType>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size = valarray.size();
        archive & size;

        ::eightser::compress::zip(archive, valarray);
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(valarray, template <typename ValueType>, std::valarray<ValueType>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size{};
        archive & size;

        valarray.resize(size);
        ::eightser::compress::zip(archive, valarray);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_VALARRAY_HPP
