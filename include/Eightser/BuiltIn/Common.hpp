#ifndef EIGHTSER_BUILTIN_COMMON_HPP
#define EIGHTSER_BUILTIN_COMMON_HPP

#include <Eightser/Serializable.hpp>

#include <Eightser/OArchive.hpp>
#include <Eightser/IArchive.hpp>

#include <Eightser/BuiltIn/Binary.hpp>
#include <Eightser/BuiltIn/DataTrack.hpp>
#include <Eightser/BuiltIn/Compress.hpp>

CONDITIONAL_SERIALIZABLE_DECLARATION(std::is_arithmetic_v<S>)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVELOAD(number, std::is_arithmetic_v<S>)
    BIN_SERIALIZABLE
    (
        ::eightser::binary(archive, number);
    )
SERIALIZABLE_INIT()


CONDITIONAL_SERIALIZABLE_DECLARATION(std::is_enum_v<S>)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVE(enumerator, std::is_enum_v<S>)
    BIN_SERIALIZABLE
    (
        auto value = static_cast<std::underlying_type_t<S>>(enumerator);
        archive & value;
    )
SERIALIZABLE_INIT()

CONDITIONAL_SERIALIZABLE_LOAD(enumerator, std::is_enum_v<S>)
    BIN_SERIALIZABLE
    (
        std::underlying_type_t<S> buff{};
        archive & buff;

        enumerator = static_cast<S>(buff);
    )
SERIALIZABLE_INIT()


CONDITIONAL_SERIALIZABLE_DECLARATION(std::is_array_v<S>)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVELOAD(array, std::is_array_v<S>)
    BIN_SERIALIZABLE
    (
        ::eightser::compress::zip(archive, array);
    )
SERIALIZABLE_INIT()


template <typename SerializableType>
struct xxeightser_is_pointer
    : std::conjunction<std::is_pointer<SerializableType>, ::eightser::meta::is_pointer_to_any<SerializableType>> {};

CONDITIONAL_SERIALIZABLE_DECLARATION(xxeightser_is_pointer<S>::value)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVELOAD(pointer, xxeightser_is_pointer<S>::value)
    BIN_SERIALIZABLE
    (
        #ifdef EIGHTSER_PTRTRACK_ENABLE
        ::eightser::track(archive, pointer);
        #else
        ::eightser::raw(archive, pointer);
        #endif // EIGHTSER_PTRTRACK_ENABLE
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_BUILTIN_COMMON_HPP
