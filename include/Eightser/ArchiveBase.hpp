#ifndef EIGHTSER_ARCHIVE_BASE_HPP
#define EIGHTSER_ARCHIVE_BASE_HPP

#include <Eightser/ArchiveTraits.hpp>

#include <Eightser/Detail/Meta.hpp>

using xxeightser_archive_type_key_type = std::uint8_t;

// should be in global namespace
template <class SerializableType, typename enable = void> struct xxeightser;

namespace eightser
{

constexpr xxeightser_archive_type_key_type bin = 0;

#ifdef EIGHTSER_FULLY_ENABLE
constexpr xxeightser_archive_type_key_type xml = 1;
constexpr xxeightser_archive_type_key_type json = 2;
#endif // EIGHTSER_FULLY_ENABLE

struct ioarchive_t
{
    ioarchive_t(::xxeightser_archive_traits_key_type trait, ::xxeightser_archive_type_key_type type, bool readonly)
        : trait(trait), type(type), readonly(readonly) {}

    #ifdef EIGHTSER_DEBUG
    virtual ~ioarchive_t() = default;
    #endif // EIGHTSER_DEBUG

    std::uint64_t trait : sizeof(xxeightser_archive_traits_key_type) * 8;
    std::uint64_t type : sizeof(xxeightser_archive_type_key_type) * 8;
    std::uint64_t readonly : 1;
};

namespace meta
{

template <typename, typename enable = void> struct is_key : std::false_type {};
template <typename SerializableType>
struct is_key<SerializableType, std::void_t<decltype(&::xxeightser<SerializableType>::key)>> : std::true_type {};

template <class ArchiveType> struct is_iarchive : std::false_type {};
template <class ArchiveType> struct is_oarchive : std::false_type {};
template <class ArchiveType> struct is_ioarchive : std::is_base_of<ioarchive_t, ArchiveType> {};

} // namespace meta

} // namespace eightser

#endif // EIGHTSER_ARCHIVE_BASE_HPP
