#ifndef EIGHTSER_SERIALIZABLE_HPP
#define EIGHTSER_SERIALIZABLE_HPP

#include <Eightser/ArchiveBase.hpp>
#include <Eightser/InstantiableRegistry.hpp>

#include <Eightser/Standard/AnyRegistry.hpp>

#include <Eightser/Detail/Macro.hpp>

#define SERIALIZABLE_ACCESS(...) \
    friend struct ::xxeightser_cast_to_non_public_base; \
    template <typename, typename> friend struct ::xxeightser;

#ifdef EIGHTSER_FULLY_ENABLE
    #define BIN_SERIALIZABLE(...)  if (archive.type == ::eightser::bin) { __VA_ARGS__ return; }
    #define XML_SERIALIZABLE(...)  if (archive.type == ::eightser::xml) { __VA_ARGS__ return; }
    #define JSON_SERIALIZABLE(...)  if (archive.type == ::eightser::json) { __VA_ARGS__ return; }
#else
    #define BIN_SERIALIZABLE(...) __VA_ARGS__
#endif // EIGHTSER_MULTY_SERIALIZATION

#define TEMPLATE_SERIALIZABLE_DECLARATION(object_template_header, ...) \
    EIGHTSER_DEPAREN(object_template_header) struct xxeightser<__VA_ARGS__> { \
        using S = __VA_ARGS__; \
        template <class ArchiveType> static void save(ArchiveType&, S&); \
        template <class ArchiveType> static void load(ArchiveType&, S&); \
        template <class ArchiveType> static void saveload(ArchiveType&, S&);

#define CONDITIONAL_SERIALIZABLE_DECLARATION(...) \
    template <typename S> struct xxeightser<S, std::enable_if_t<__VA_ARGS__>> { \
        template <class ArchiveType> static void save(ArchiveType&, S&); \
        template <class ArchiveType> static void load(ArchiveType&, S&); \
        template <class ArchiveType> static void saveload(ArchiveType&, S&);

#define VISIBLE_SERIALIZABLE_DECLARATION(module_api, ...) \
    template <> struct EIGHTSER_DEPAREN(module_api) xxeightser<__VA_ARGS__> { \
        using S = __VA_ARGS__; \
        [[maybe_unused]] static void save(::eightser::ioarchive_t&, S&); \
        [[maybe_unused]] static void load(::eightser::ioarchive_t&, S&); \
        [[maybe_unused]] static void saveload(::eightser::ioarchive_t&, S&);

#define SERIALIZABLE_DECLARATION(...) VISIBLE_SERIALIZABLE_DECLARATION((/*no module_api*/), __VA_ARGS__)

#define SERIALIZABLE_DECLARATION_INIT(...) \
    };


#define TEMPLATE_SERIALIZABLE_SAVE(object, object_template_header, ...) \
    EIGHTSER_DEPAREN(object_template_header) template <class ArchiveType> void xxeightser<__VA_ARGS__>::save([[maybe_unused]] ArchiveType& archive, [[maybe_unused]] S& object) {

#define TEMPLATE_SERIALIZABLE_LOAD(object, object_template_header, ...) \
    EIGHTSER_DEPAREN(object_template_header) template <class ArchiveType> void xxeightser<__VA_ARGS__>::load([[maybe_unused]] ArchiveType& archive, [[maybe_unused]] S& object) {

#define TEMPLATE_SERIALIZABLE_SAVELOAD(object, object_template_header, ...) \
    EIGHTSER_DEPAREN(object_template_header) template <class ArchiveType> void xxeightser<__VA_ARGS__>::save(ArchiveType& archive, S& object) { saveload(archive, object); } \
    EIGHTSER_DEPAREN(object_template_header) template <class ArchiveType> void xxeightser<__VA_ARGS__>::load(ArchiveType& archive, S& object) { saveload(archive, object); } \
    EIGHTSER_DEPAREN(object_template_header) template <class ArchiveType> void xxeightser<__VA_ARGS__>::saveload([[maybe_unused]] ArchiveType& archive, [[maybe_unused]] S& object) {


#define CONDITIONAL_SERIALIZABLE_SAVE(object, ...) \
    template <typename S> template <class ArchiveType> void xxeightser<S, std::enable_if_t<__VA_ARGS__>>::save([[maybe_unused]] ArchiveType& archive, [[maybe_unused]] S& object) {

#define CONDITIONAL_SERIALIZABLE_LOAD(object, ...) \
    template <typename S> template <class ArchiveType> void xxeightser<S, std::enable_if_t<__VA_ARGS__>>::load([[maybe_unused]] ArchiveType& archive, [[maybe_unused]] S& object) {

#define CONDITIONAL_SERIALIZABLE_SAVELOAD(object, ...) \
    template <typename S> template <class ArchiveType> void xxeightser<S, std::enable_if_t<__VA_ARGS__>>::save(ArchiveType& archive, S& object) { saveload(archive, object); } \
    template <typename S> template <class ArchiveType> void xxeightser<S, std::enable_if_t<__VA_ARGS__>>::load(ArchiveType& archive, S& object) { saveload(archive, object); } \
    template <typename S> template <class ArchiveType> void xxeightser<S, std::enable_if_t<__VA_ARGS__>>::saveload(ArchiveType& archive, S& object) {


#define SERIALIZABLE_SAVE(object, ...) \
    void xxeightser<__VA_ARGS__>::save([[maybe_unused]] ::eightser::ioarchive_t& archive, [[maybe_unused]] S& object) {

#define SERIALIZABLE_LOAD(object, ...) \
    void xxeightser<__VA_ARGS__>::load([[maybe_unused]] ::eightser::ioarchive_t& archive, [[maybe_unused]] S& object) {

#define SERIALIZABLE_SAVELOAD(object, ...) \
    void xxeightser<__VA_ARGS__>::save(::eightser::ioarchive_t& archive, S& object) { saveload(archive, object); } \
    void xxeightser<__VA_ARGS__>::load(::eightser::ioarchive_t& archive, S& object) { saveload(archive, object); } \
    void xxeightser<__VA_ARGS__>::saveload([[maybe_unused]] ::eightser::ioarchive_t& archive, [[maybe_unused]] S& object) {

#define SERIALIZABLE_INIT(...) \
    }

struct xxeightser_cast_to_non_public_base
{
    template <class BaseType, class DerivedType>
    static BaseType& call(DerivedType& object)
    {
        return static_cast<BaseType&>(object);
    }
};

namespace eightser
{

// Type registry for instantiable & any serialization, allowed registered and supported types only
template <typename SerializableType> void serializable()
{
    static_assert(std::negation_v<meta::is_unsupported<SerializableType>>, "The 'SerializableType' is an unsupported type for serialization.");

    instantiable_registry()->add<SerializableType>();
    #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)
    any_registry()->add<SerializableType>();
    #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE
}

template <typename SerializableType> SerializableType&& serializable(SerializableType&& object)
{
    serializable<std::decay_t<SerializableType>>();
    return std::forward<SerializableType>(object);
}

} // namepsace eightser

#endif // EIGHTSER_SERIALIZABLE_HPP
