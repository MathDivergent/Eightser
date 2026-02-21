#ifndef EIGHTSER_BUILTIN_BIT_PACK_HPP
#define EIGHTSER_BUILTIN_BIT_PACK_HPP

#include <cstdint> // uint32_t

#include <Eightser/OArchive.hpp>
#include <Eightser/IArchive.hpp>

#include <Eightser/Detail/Macro.hpp> // EIGHTSER_FIRST_ARGUMENT, EIGHTSER_CONCAT

#include <Eightser/BuiltIn/Detail/Macro.hpp> // EIGHTSER_BITPACK

// Signature:
// BITPACK(common_fields_type, archive, (object.field0, field0_bits), (object.field1, field1_bits), ...)
// BITPACK macro will generate code:
// {
//     auto xxbitpack = ::eightser::bitpack<common_fields_type>(archive);
//     object.field0 = xxbitpack(object.field0, field0_bits);
//     object.field1 = xxbitpack(object.field1, field1_bits);
//     and etc.
// }
#define BITPACK(common_fields_type, archive, ...) EIGHTSER_BITPACK(common_fields_type, archive, __VA_ARGS__)

namespace eightser
{

namespace detail
{

template <class ArchiveType, typename FieldType, typename enable = void>
struct bitpack_t;

template <class ArchiveType, typename FieldType>
struct bitpack_t<ArchiveType, FieldType, std::enable_if_t<eightser::meta::is_oarchive<ArchiveType>::value>>
{
    ArchiveType& archive;
    FieldType data{};
    std::size_t offset{};

    bitpack_t(ArchiveType& archive) : archive(archive) {}
    ~bitpack_t() { archive & data; }

    FieldType operator()(FieldType field, std::size_t bits) noexcept
    {
        // same as data = data | (field << offset);
        data |= field << offset;
        offset += bits;

        return field;
    }
};

template <class ArchiveType, typename FieldType>
struct bitpack_t<ArchiveType, FieldType, std::enable_if_t<eightser::meta::is_iarchive<ArchiveType>::value>>
{
    ArchiveType& archive;
    FieldType data{};

    bitpack_t(ArchiveType& archive) : archive(archive) { archive & data; }

    FieldType operator()(FieldType field, std::size_t bits) noexcept
    {
        //same as field = data & ~(0xf...f << bits)
        field = data & ~(~FieldType{} << bits);
        data >>= bits;

        return field;
    }
};

} // namespace detail

template <typename PackType = std::uint32_t, class ArchiveType>
detail::bitpack_t<ArchiveType, PackType> bitpack(ArchiveType& archive) noexcept { return { archive }; }

} // namespace eightser

#endif // EIGHTSER_BUILTIN_BIT_PACK_HPP
