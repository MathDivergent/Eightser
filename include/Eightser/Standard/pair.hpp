#ifndef EIGHTSER_STANDARD_PAIR_HPP
#define EIGHTSER_STANDARD_PAIR_HPP

#include <utility> // pair

#include <Eightser/Serializable.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION((template <typename FirstType, typename SecondType>), std::pair<FirstType, SecondType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVELOAD(pair, (template <typename FirstType, typename SecondType>), std::pair<FirstType, SecondType>)
    BIN_SERIALIZABLE
    (
       archive & pair.first & pair.second;
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_PAIR_HPP
