#ifndef EIGHTSER_STANDARD_TUPLE_HPP
#define EIGHTSER_STANDARD_TUPLE_HPP

#include <tuple> // tuple

#include <utility> // index_sequence, make_index_sequence

#include <Eightser/Serializable.hpp>

namespace eightser
{

namespace detail
{

template <class ArchiveType, typename... ArgumentTypes, std::size_t... TupleElementIndexes>
void expand(ArchiveType& archive, std::tuple<ArgumentTypes...>& tuple, std::index_sequence<TupleElementIndexes...>)
{
    archive(std::get<TupleElementIndexes>(tuple)...);
}

} // namespace detail

} // namespace eightser

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename... ArgumentTypes>, std::tuple<ArgumentTypes...>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVELOAD(tuple, template <typename... ArgumentTypes>, std::tuple<ArgumentTypes...>)
    BIN_SERIALIZABLE
    (
        ::eightser::detail::expand(archive, tuple, std::make_index_sequence<sizeof...(ArgumentTypes)>{});
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_TUPLE_HPP
