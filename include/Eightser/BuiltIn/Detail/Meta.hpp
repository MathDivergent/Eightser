#ifndef EIGHTSER_BUILT_IN_DETAIL_META_HPP
#define EIGHTSER_BUILT_IN_DETAIL_META_HPP

namespace eightser
{

namespace meta
{

template <template <typename...> class AdapterTemplate,
          typename ValueType, class ContainerType, typename... ArgumentTypes>
ContainerType& underlying(AdapterTemplate<ValueType, ContainerType, ArgumentTypes...>& adapter) noexcept
{
    using AdapterType = AdapterTemplate<ValueType, ContainerType, ArgumentTypes...>;

    struct xxinner : public AdapterType
    {
        static ContainerType& underlying(AdapterType& base)
        {
            return base.*(&xxinner::c);
        }
    };

    return xxinner::underlying(adapter);
}

} // namespace meta

} // namespace eightser

#endif // EIGHTSER_BUILT_IN_DETAIL_META_HPP
