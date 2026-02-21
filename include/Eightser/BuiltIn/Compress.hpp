#ifndef EIGHTSER_BUILTIN_COMPRESS_HPP
#define EIGHTSER_BUILTIN_COMPRESS_HPP

#include <cstddef> // size_t

#include <memory> // addressof
#include <valarray> // valarray

#include <Eightser/ArchiveBase.hpp>

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp>

namespace eightser
{

namespace utility
{

template <class ContainerType>
typename ContainerType::value_type const* data(ContainerType const& c) noexcept
{
    // if begin return iterator - we should deref it and take address again
    return std::addressof(*std::begin(c));
}

template <typename ValueType>
ValueType const* data(std::valarray<ValueType> const& c) noexcept
{
    return std::begin(c);
}

template <typename ValueType, std::size_t SizeValue>
ValueType const* data(ValueType const (&array)[SizeValue]) noexcept
{
    return array;
}

template <class ContainerType>
auto size(ContainerType const& c) noexcept -> decltype(c.size())
{
    return c.size();
}

template <typename ValueType, std::size_t SizeValue>
constexpr std::size_t size(ValueType const (&)[SizeValue]) noexcept
{
    return SizeValue;
}

} // namespace utility

namespace compress
{

// always require compressible type for fast compression
template <class ArchiveType, typename SerializableType,
          EIGHTSER_REQUIRES(meta::is_compressible<SerializableType>::value)>
void fast(ArchiveType& archive, SerializableType& object)
{
    using item_type = typename meta::value<SerializableType>::type;

    archive.stream().call
    (
        const_cast<item_type*>(utility::data(object)),
        utility::size(object)
    );
}

template <class ArchiveType, typename SerializableType>
void slow(ArchiveType& archive, SerializableType& object)
{
    for (auto&& item : object)
        archive & item;
}

template <class ArchiveType, typename SerializableType>
void zip(ArchiveType& archive, SerializableType& object)
{
    if constexpr (meta::is_compressible<SerializableType>::value)
    {
        fast(archive, object);
    }
    else
    {
        slow(archive, object);
    }
}

} // namespace compress

} // namespace eightser

#endif // EIGHTSER_BUILTIN_COMPRESS_HPP
