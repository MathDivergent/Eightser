#ifndef EIGHTSER_STANDARD_BITSET_HPP
#define EIGHTSER_STANDARD_BITSET_HPP

#include <bitset> // bitset

#include <Eightser/Serializable.hpp>

// default array for bitset convertion
#include <Eightser/Standard/string.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION(template <std::size_t SizeValue>, std::bitset<SizeValue>)
SERIALIZABLE_DECLARATION_INIT()

// slow impl
TEMPLATE_SERIALIZABLE_SAVE(bitset, template <std::size_t SizeValue>, std::bitset<SizeValue>)
    BIN_SERIALIZABLE
    (
        auto data = bitset.to_string();
        archive & data;
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(bitset, template <std::size_t SizeValue>, std::bitset<SizeValue>)
    BIN_SERIALIZABLE
    (
        std::string data;
        archive & data;

        bitset = std::bitset<SizeValue>(data);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_BITSET_HPP
