#ifndef EIGHTSER_POLYMORPHIC_ARCHIVE_HPP
#define EIGHTSER_POLYMORPHIC_ARCHIVE_HPP

#include <Eightser/ArchiveBase.hpp>
#include <Eightser/ArchiveTraits.hpp>
#include <Eightser/Hash.hpp>

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp>

namespace eightser
{

class polymorphic_archive_t
{
public:
    template <typename SerializableType>
    static void save(ioarchive_t& archive, SerializableType& data)
    {
        call<::xxeightser_archive_registry, ::xxeightser_oarchive_registry_overload>(archive, data);
    }

    template <typename SerializableType>
    static void load(ioarchive_t& archive, SerializableType& data)
    {
        call<::xxeightser_archive_registry, ::xxeightser_iarchive_registry_overload>(archive, data);
    }

private:
    template <template <class, ::xxeightser_archive_traits_key_type> class ArchiveRegistryTemplate,
              class ArchiveRegistryOverloadType,
              ::xxeightser_archive_traits_key_type ArchiveKeyValue = 0,
              typename SerializableType>
    static void call(ioarchive_t& archive, SerializableType& data)
    {
        if constexpr (ArchiveKeyValue == ::xxeightser_archive_traits_base_key)
        {
            throw "The read/write archive has invalid type key.";
        }
        else
        {
            using DerivedArchiveType = typename ArchiveRegistryTemplate<ArchiveRegistryOverloadType, ArchiveKeyValue>::type;

            if (::xxeightser_archive_traits<DerivedArchiveType>::key == archive.trait)
                return try_call<DerivedArchiveType>(archive, data);

            call<ArchiveRegistryTemplate, ArchiveRegistryOverloadType, ArchiveKeyValue + 1>(archive, data);
        }
    }

    template <class DerivedArchiveType, typename SerializableType>
    static void try_call(ioarchive_t& archive, SerializableType& data)
    {
        if constexpr (::xxeightser_archive_traits<DerivedArchiveType>::key == ::xxeightser_archive_traits_base_key)
        {
            throw "The read/write archive was not registered.";
        }
        else
        {
            #ifdef EIGHTSER_DEBUG
            if (EIGHTSER_EXPRESSION_HASH(archive) != EIGHTSER_TYPE_HASH(DerivedArchiveType))
                throw "The read/write archive was registered incorrect.";
            #endif // EIGHTSER_DEBUG
            try_call_impl<DerivedArchiveType>(static_cast<DerivedArchiveType&>(archive), data);
        }
    }

    template <class DerivedArchiveType, typename SerializableType>
    static void try_call_impl(ioarchive_t& archive, SerializableType& data)
    {
        if constexpr (meta::is_oarchive<DerivedArchiveType>::value)
        {
            ::xxeightser<SerializableType>::save(static_cast<DerivedArchiveType&>(archive), data);
        }
        else if constexpr (meta::is_iarchive<DerivedArchiveType>::value)
        {
            ::xxeightser<SerializableType>::load(static_cast<DerivedArchiveType&>(archive), data);
        }
        else
        {
            static_assert(meta::to_false<DerivedArchiveType>(), "The read/write archive has not traits.");
        }
    }
};

template <typename SerializableType,
          EIGHTSER_REQUIRES(std::negation_v<meta::is_unsupported<SerializableType>>)>
ioarchive_t& operator<< (ioarchive_t& archive, SerializableType const& data)
{
    polymorphic_archive_t::save(archive, const_cast<SerializableType&>(data));
    return archive;
}

template <typename SerializableType,
          EIGHTSER_REQUIRES(std::negation_v<meta::is_unsupported<SerializableType>>)>
ioarchive_t& operator>> (ioarchive_t& archive, SerializableType const& data)
{
    polymorphic_archive_t::load(archive, const_cast<SerializableType&>(data));
    return archive;
}

template <typename SerializableType>
ioarchive_t& operator& (ioarchive_t& archive, SerializableType const& data)
{
    if constexpr (meta::is_unsupported<SerializableType>::value)
    {
        static_assert(meta::to_false<SerializableType>(), "The 'SerializableType' is an unsupported type for the 'eightser::ioarchive_t'.");
        return archive;
    }
    else
    {
        return archive.saveload ? archive << data : archive >> data;
    }
}

} // namespace eightser

#endif // EIGHTSER_POLYMORPHIC_ARCHIVE_HPP
