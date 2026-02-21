#ifndef EIGHTSER_STANDARD_STRING_HPP
#define EIGHTSER_STANDARD_STRING_HPP

#include <cstdint> // uint64_t

#include <string> // basic_string

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/Compress.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION
(
    (template <typename CharType, typename TraitsType, typename AllocatorType>),
    std::basic_string<CharType, TraitsType, AllocatorType>
)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE
(
    string,
    (template <typename CharType, typename TraitsType, typename AllocatorType>),
    std::basic_string<CharType, TraitsType, AllocatorType>
)
    BIN_SERIALIZABLE
    (
        std::uint64_t size = string.size();
        archive & size;

        ::eightser::compress::zip(archive, string);
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD
(
    string,
    (template <typename CharType, typename TraitsType, typename AllocatorType>),
    std::basic_string<CharType, TraitsType, AllocatorType>
)
    BIN_SERIALIZABLE
    (
        std::uint64_t size{};
        archive & size;

        string.resize(size);
        ::eightser::compress::zip(archive, string);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_STRING_HPP
