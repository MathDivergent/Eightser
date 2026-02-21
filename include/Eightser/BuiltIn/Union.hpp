#ifndef EIGHTSER_BUILTIN_UNION_HPP
#define EIGHTSER_BUILTIN_UNION_HPP

#ifdef EIGHTSER_UNION_ENABLE
#include <Eightser/Serializable.hpp>
#include <Eightser/BuiltIn/Binary.hpp>

CONDITIONAL_SERIALIZABLE_DECLARATION(std::is_union_v<S>)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVELOAD(data, std::is_union_v<S>)
    BIN_SERIALIZABLE
    (
        ::eightser::binary(archive, data);
    )
SERIALIZABLE_INIT()
#endif // EIGHTSER_UNION_ENABLE

#endif // EIGHTSER_BUILTIN_UNION_HPP
