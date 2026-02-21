#ifndef EIGHTSER_ARCHIVE_TRAITS_HPP
#define EIGHTSER_ARCHIVE_TRAITS_HPP

#include <cstdint> // uint8_t

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp> // EIGHTSER_DEPAREN

#ifndef EIGHTSER_ARCHIVE_TRAIT_MAX_KEY_SIZE
    #define EIGHTSER_ARCHIVE_TRAIT_MAX_KEY_SIZE 4
#endif // EIGHTSER_ARCHIVE_TRAIT_MAX_KEY_SIZE

#define VISIBLE_SERIALIZABLE_ARCHIVE_DECLARATION(module_api, archive_key, ...) \
    extern template class EIGHTSER_DEPAREN(module_api) __VA_ARGS__; \
    template <> struct xxeightser_archive_traits<__VA_ARGS__> { \
        static constexpr auto key = ::xxeightser_archive_traits_key_type(archive_key); \
    }; \
    template <> struct xxeightser_archive_registry<::xxeightser_archive_registry_overload<__VA_ARGS__>::type, archive_key> { \
        using type = __VA_ARGS__; \
    };

#define SERIALIZABLE_ARCHIVE_DECLARATION(archive_key, ...) VISIBLE_SERIALIZABLE_ARCHIVE_DECLARATION((/*no module_api*/), archive_key, __VA_ARGS__)

#define SERIALIZABLE_ARCHIVE(...) template class __VA_ARGS__;

namespace eightser
{

struct ioarchive_t;

namespace meta
{

template <class ArchiveType> struct is_iarchive : std::false_type {};
template <class ArchiveType> struct is_oarchive : std::false_type {};

} // namespace meta

} // namespace eightser

using xxeightser_archive_traits_key_type = std::uint8_t;
static constexpr auto xxeightser_archive_traits_base_key = ::xxeightser_archive_traits_key_type(EIGHTSER_ARCHIVE_TRAIT_MAX_KEY_SIZE);

template <class ArchiveType>
struct xxeightser_archive_traits
{
    static constexpr auto key = xxeightser_archive_traits_base_key;
};

template <class ArchiveRegistryOverloadType, ::xxeightser_archive_traits_key_type ArchiveKey>
struct xxeightser_archive_registry 
{
    using type = ::eightser::ioarchive_t;
};

struct xxeightser_none_archive_registry_overload;
struct xxeightser_iarchive_registry_overload;
struct xxeightser_oarchive_registry_overload;

template <class ArchiveType, typename enable = void>
struct xxeightser_archive_registry_overload
{
    using type = xxeightser_none_archive_registry_overload;
};

template <class ArchiveType>
struct xxeightser_archive_registry_overload<ArchiveType, std::enable_if_t<::eightser::meta::is_iarchive<ArchiveType>::value>>
{
    using type = xxeightser_iarchive_registry_overload;
};

template <class ArchiveType>
struct xxeightser_archive_registry_overload<ArchiveType, std::enable_if_t<::eightser::meta::is_oarchive<ArchiveType>::value>>
{
    using type = xxeightser_oarchive_registry_overload;
};

#endif // EIGHTSER_ARCHIVE_TRAITS_HPP
