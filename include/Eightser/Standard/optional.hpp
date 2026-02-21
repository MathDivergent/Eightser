#ifndef EIGHTSER_STANDARD_OPTIONAL_HPP
#define EIGHTSER_STANDARD_OPTIONAL_HPP

#include <optional> // optional

#include <Eightser/Serializable.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename ValueType>, std::optional<ValueType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(optional, template <typename ValueType>, std::optional<ValueType>)
    BIN_SERIALIZABLE
    (
        auto is_init = optional.has_value();
        archive & is_init;

        if (is_init) archive & optional.value();
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(optional, template <typename ValueType>, std::optional<ValueType>)
    BIN_SERIALIZABLE
    (
        auto is_init = false;
        archive & is_init;

        if (is_init)
        {
            optional.emplace();
            archive & optional.value();
        }
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_OPTIONAL_HPP
