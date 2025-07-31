#ifndef EIGHTSER_BUILTIN_ALIAS_HPP
#define EIGHTSER_BUILTIN_ALIAS_HPP

#include <memory> // addressof

#include <Eightser/BuiltIn/DataTrack.hpp>

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp>

namespace eightser
{

template <typename ElementType>
class alias_t
{
private:
    ElementType* xxdata;

public:
    using element_type = ElementType;

    // DONT use dereferencing of null data before rebinding
    alias_t() noexcept : xxdata(nullptr) {}

    template <typename OtherElementType,
              EIGHTSER_REQUIRES(meta::is_static_castable<OtherElementType*, element_type*>::value)>
    alias_t(OtherElementType& data) noexcept : xxdata(std::addressof(data)) {}

    template <typename OtherElementType>
    alias_t(alias_t<OtherElementType> const& data) noexcept : alias_t(data.get()) {}

public:
    // rebinding data
    alias_t(alias_t const&) = default;
    alias_t& operator=(alias_t const&) = default;

    bool is_refer() const noexcept { return xxdata != nullptr; }

    template <typename OtherElementType>
    bool is_refer(OtherElementType& data)  const noexcept { return xxdata == std::addressof(data); }

    operator element_type&() const noexcept { return get(); }

    element_type& get() const noexcept { return *xxdata; }
    void set(element_type& data) noexcept { xxdata = std::addressof(data); }
};

} // namespace eightser

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename ElementType>, ::eightser::alias_t<ElementType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE(save, alias, template <typename ElementType>, ::eightser::alias_t<ElementType>)
    SERIALIZATION
    (
        using pointer_hold_type = INSTANTIABLE_TYPE*; // need to overload tracking

        if (not alias.is_refer())
            throw "The write alias_t must be initialized.";

        auto pointer = std::addressof(alias.get());
        auto const key = ::eightser::detail::tracking_key(archive, pointer);

        auto& is_tracking = archive.tracking().pointer(pointer_hold_type())[key];

        if (not is_tracking)
            throw "The write alias_t must be tracked before.";
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE(load, alias, template <typename ElementType>, ::eightser::alias_t<ElementType>)
    SERIALIZATION
    (
        using pointer_hold_type = INSTANTIABLE_TYPE*; // need to overload tracking

        #ifdef EIGHTSER_GARBAGE_CHECK_ENABLE
        if (alias.is_refer())
            throw "The read alias_t must be null.";
        #endif // EIGHTSER_GARBAGE_CHECK_ENABLE

        std::uintptr_t key{};
        archive & key;

        ElementType* pointer = nullptr;
        auto& address = archive.tracking().pointer(pointer_hold_type())[key];

        if (address == nullptr)
            throw "The read alias_t must be tracked before.";

        ::eightser::detail::native_assign(archive, pointer, address);

        alias.set(*pointer); // pointer will never nullptr
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_BUILTIN_ALIAS_HPP
