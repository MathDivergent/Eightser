#ifndef EIGHTSER_BUILTIN_AGGREGATE_HPP
#define EIGHTSER_BUILTIN_AGGREGATE_HPP

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/ApplyFunctor.hpp>

#include <Eightser/Detail/Macro.hpp> // EIGHTSER_PLACEHOLDERS, EIGHTSER_REPEAT

#define EIGHTSER_AGGREGATE_IMPLEMENTATION_GENERIC(count) \
    template <class ArchiveType, typename SerializableType> \
    void aggregate_impl(ArchiveType& archive, SerializableType& object, std::integral_constant<std::size_t, count>) { \
        auto& [EIGHTSER_PLACEHOLDERS(count)] = object; \
        archive(EIGHTSER_PLACEHOLDERS(count)); \
    }

namespace eightser
{

namespace meta
{

template <typename> struct is_weak_aggregate : std::false_type {};

template <typename AggregateType> struct is_aggregate
    : std::conjunction<std::is_aggregate<AggregateType>,
                       std::negation<is_weak_aggregate<AggregateType>>,
                       std::negation<std::is_array<AggregateType>>> {};

template <typename SerializableType> struct is_serializable_aggregate
    : std::conjunction<is_aggregate<SerializableType>, std::negation<std::is_union<SerializableType>>> {};

} // namespace meta

namespace detail
{

template <class ArchiveType, typename SerializableType>
void aggregate_impl(ArchiveType&, SerializableType&, std::integral_constant<std::size_t, 0>) noexcept { /*pass*/ }

EIGHTSER_REPEAT(EIGHTSER_AGGREGATE_IMPLEMENTATION_GENERIC, 64)

} // namespace detail

template <class ArchiveType, typename SerializableType>
void aggregate(ArchiveType& archive, SerializableType& object)
{
    constexpr auto N = meta::aggregate_size<SerializableType>::size();
    detail::aggregate_impl(archive, object, std::integral_constant<std::size_t, N>{});
}

namespace apply
{

template <typename SerializableType>
struct aggregate_functor_t : apply_functor_t
{
    SerializableType& object;

    aggregate_functor_t(SerializableType& object) noexcept : object(object) {}

    template <class ArchiveType>
    void operator() (ArchiveType& archive) const { aggregate(archive, object); }
};

} // namespace apply

template <typename SerializableType>
apply::aggregate_functor_t<SerializableType> aggregate(SerializableType& object) noexcept { return { object }; }

} // namespace eightser


CONDITIONAL_SERIALIZABLE_DECLARATION(::eightser::meta::is_serializable_aggregate<S>::value)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVELOAD(object, ::eightser::meta::is_serializable_aggregate<S>::value)
    BIN_SERIALIZABLE
    (
        ::eightser::aggregate(archive, object);
    )
SERIALIZABLE_INIT()

// clean up
#undef EIGHTSER_AGGREGATE_IMPLEMENTATION_GENERIC

#include <array> // array

namespace eightser
{

namespace meta
{

// write own specializations to control aggregate type serialization
template <typename ValueType, std::size_t SizeValue>
struct is_weak_aggregate<std::array<ValueType, SizeValue>> : std::true_type {};

} // namespace meta

} // namespace eightser

#endif // EIGHTSER_BUILTIN_AGGREGATE_HPP
