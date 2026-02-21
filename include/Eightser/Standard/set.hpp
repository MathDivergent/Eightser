#ifndef EIGHTSER_STANDARD_SET_HPP
#define EIGHTSER_STANDARD_SET_HPP

#include <cstdint> // uint64_t

#include <type_traits> // true_type, false_type

#include <set> // set, multiset
#include <unordered_set> // unordered_set, unordered_multiset

#include <utility> // move

#include <Eightser/Serializable.hpp>

#include <Eightser/BuiltIn/Compress.hpp>

#define EIGHTSER_IS_STD_SET_TYPE_META_GENERIC(set_type) \
    template <typename> struct xxeightser_is_std_##set_type : std::false_type {}; \
    template <typename KeyType, typename ComparatorType, typename AllocatorType> \
    struct xxeightser_is_std_##set_type<std::set_type<KeyType, ComparatorType, AllocatorType>> : std::true_type {};


EIGHTSER_IS_STD_SET_TYPE_META_GENERIC(set)
EIGHTSER_IS_STD_SET_TYPE_META_GENERIC(unordered_set)
EIGHTSER_IS_STD_SET_TYPE_META_GENERIC(multiset)
EIGHTSER_IS_STD_SET_TYPE_META_GENERIC(unordered_multiset)

template <class StdSetType> struct xxeightser_is_std_any_unordered_set
    : std::disjunction<::xxeightser_is_std_unordered_set<StdSetType>,
                       ::xxeightser_is_std_unordered_multiset<StdSetType>> {};

template <class StdSetType> struct xxeightser_is_std_any_set
    : std::disjunction<::xxeightser_is_std_set<StdSetType>,
                       ::xxeightser_is_std_multiset<StdSetType>,
                       ::xxeightser_is_std_any_unordered_set<StdSetType>> {};

CONDITIONAL_SERIALIZABLE_DECLARATION(::xxeightser_is_std_any_set<S>::value)
SERIALIZABLE_DECLARATION_INIT()

CONDITIONAL_SERIALIZABLE_SAVE(set, ::xxeightser_is_std_any_set<S>::value)
    BIN_SERIALIZABLE
    (
        std::uint64_t size = set.size();
        archive & size;

        ::eightser::compress::slow(archive, set);
    )
SERIALIZABLE_INIT()

CONDITIONAL_SERIALIZABLE_LOAD(set, ::xxeightser_is_std_any_set<S>::value)
    BIN_SERIALIZABLE
    (
        using value_type = typename S::value_type;

        std::uint64_t size{};
        archive & size;

        set.clear();
        if constexpr (::xxeightser_is_std_any_unordered_set<S>::value)
        {
            set.reserve(size);
        }

        auto hint = set.begin();
        for (std::uint64_t i = 0; i < size; ++i)
        {
            value_type item{}; // temp
            archive & item;

            hint = set.emplace_hint(hint, std::move(item));
        }
    )
SERIALIZABLE_INIT()

// clean up
#undef EIGHTSER_IS_STD_SET_TYPE_META_GENERIC

#endif // EIGHTSER_STANDARD_SET_HPP
