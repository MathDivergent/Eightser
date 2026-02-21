#ifndef EIGHTSER_STANDARD_VARIANT_HPP
#define EIGHTSER_STANDARD_VARIANT_HPP

#include <cstdint> // uint64_t

#include <variant> // variant

#include <Eightser/Serializable.hpp>

// serialization of std::monostate
#include <Eightser/BuiltIn/Aggregate.hpp>

namespace eightser
{

namespace detail
{

template <std::uint64_t VariantAlternativeIndexValue = 0, class ArchiveType, class VariantType>
void variant_save(ArchiveType& archive, VariantType& variant, std::uint64_t varian_alternative_index)
{
    if constexpr (VariantAlternativeIndexValue < std::variant_size_v<VariantType>)
    {
        if (VariantAlternativeIndexValue < varian_alternative_index)
        {
            return variant_save<VariantAlternativeIndexValue + 1>(archive, variant, varian_alternative_index);
        }
        else
        {
            archive & std::get<VariantAlternativeIndexValue>(variant);
        }
    }
}

template <std::uint64_t VariantAlternativeIndexValue = 0, class ArchiveType, class VariantType>
void variant_load(ArchiveType& archive, VariantType& variant, std::uint64_t varian_alternative_index)
{
    if constexpr (VariantAlternativeIndexValue < std::variant_size_v<VariantType>)
    {
        using variant_alternative_type = std::variant_alternative_t<VariantAlternativeIndexValue, VariantType>;

        if (VariantAlternativeIndexValue < varian_alternative_index)
        {
            return variant_load<VariantAlternativeIndexValue + 1>(archive, variant, varian_alternative_index);
        }

        if constexpr (std::is_constructible_v<variant_alternative_type>)
        {
            archive & variant.template emplace<variant_alternative_type>();
        }
        else
        {
            throw "Require default constructor for specify type.";
        }
    }
}

} // namespace detail

} // namespace eightser

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename... ArgumentTypes>, std::variant<ArgumentTypes...>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(variant, template <typename... ArgumentTypes>, std::variant<ArgumentTypes...>)
    BIN_SERIALIZABLE
    (
        std::uint64_t varian_alternative_index = variant.index();
        archive & varian_alternative_index;

        if (varian_alternative_index != std::variant_npos)
        {
            ::eightser::detail::variant_save(archive, variant, varian_alternative_index);
        }
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(variant, (template <typename... ArgumentTypes>), std::variant<ArgumentTypes...>)
    BIN_SERIALIZABLE
    (
        std::uint64_t varian_alternative_index{};
        archive & varian_alternative_index;

        if (varian_alternative_index != std::variant_npos)
        {
            ::eightser::detail::variant_load(archive, variant, varian_alternative_index);
        }
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_VARIANT_HPP
