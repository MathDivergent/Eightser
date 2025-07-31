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
    #define SERIALIZATION(...) EIGHTSER_SERIALIZATION(bin, __VA_ARGS__)
    #define SERIALIZATION_XML(...) EIGHTSER_SERIALIZATION(xml, __VA_ARGS__)
    #define SERIALIZATION_JSON(...) EIGHTSER_SERIALIZATION(json, __VA_ARGS__)
#else
    #define SERIALIZATION(...) __VA_ARGS__
#endif // EIGHTSER_MULTY_SERIALIZATION

#define TEMPLATE_SERIALIZABLE_DECLARATION(object_template_header, ...) \
    EIGHTSER_DEPAREN(object_template_header) struct xxeightser<__VA_ARGS__> { \
        using S = __VA_ARGS__; \
        template <class ArchiveType> static void save(ArchiveType&, S&); \
        template <class ArchiveType> static void load(ArchiveType&, S&);

#define CONDITIONAL_SERIALIZABLE_DECLARATION(...) \
    template <typename S> struct xxeightser<S, typename std::enable_if<__VA_ARGS__>::type> { \
        template <class ArchiveType> static void save(ArchiveType&, S&); \
        template <class ArchiveType> static void load(ArchiveType&, S&);

#define SERIALIZABLE_DECLARATION(...) \
    template <> struct xxeightser<__VA_ARGS__> { \
        using S = __VA_ARGS__; \
        static void save(::eightser::ioarchive_t&, S&); \
        static void load(::eightser::ioarchive_t&, S&);


#define SERIALIZABLE_DECLARATION_INIT(...) \
    };


#define TEMPLATE_SERIALIZABLE(mode, object, object_template_header, ...) \
     EIGHTSER_CONCAT(EIGHTSER_TEMPLATE_SERIALIZABLE_, mode)(object, object_template_header, __VA_ARGS__)

#define CONDITIONAL_SERIALIZABLE(mode, object, ...) \
    EIGHTSER_CONCAT(EIGHTSER_CONDITIONAL_SERIALIZABLE_, mode)(object, __VA_ARGS__)

#define SERIALIZABLE(mode, object, ...) EIGHTSER_CONCAT(EIGHTSER_SERIALIZABLE_, mode)(object, __VA_ARGS__)

#define SERIALIZABLE_INIT(...) \
    }


// impl
#ifdef EIGHTSER_FULLY_ENABLE
    #define EIGHTSER_SERIALIZATION(archive_type, ...) if (archive.type == ::eightser::archive_type) { __VA_ARGS__ return; }
#endif // EIGHTSER_FULLY_ENABLE

#define EIGHTSER_TEMPLATE_SERIALIZABLE(mode, object, object_template_header, ...) \
    EIGHTSER_DEPAREN(object_template_header) \
    template <class ArchiveType> void xxeightser<__VA_ARGS__>::mode(ArchiveType& archive, S& object) {

#define EIGHTSER_TEMPLATE_SERIALIZABLE_save(object, object_template_header, ...) \
    EIGHTSER_TEMPLATE_SERIALIZABLE(save, object, object_template_header, __VA_ARGS__)

#define EIGHTSER_TEMPLATE_SERIALIZABLE_load(object, object_template_header, ...) \
    EIGHTSER_TEMPLATE_SERIALIZABLE(load, object, object_template_header, __VA_ARGS__)

#define EIGHTSER_TEMPLATE_SERIALIZABLE_saveload(object, object_template_header, ...) \
    EIGHTSER_TEMPLATE_SERIALIZABLE_save(object, object_template_header, __VA_ARGS__) \
        SERIALIZATION(load(archive, object);) \
    SERIALIZABLE_INIT() \
    EIGHTSER_TEMPLATE_SERIALIZABLE_load(object, object_template_header, __VA_ARGS__)

#define EIGHTSER_CONDITIONAL_SERIALIZABLE(mode, object, ...) \
    template <typename S> \
    template <class ArchiveType> void xxeightser<S, typename std::enable_if<__VA_ARGS__>::type>::mode(ArchiveType& archive, S& object) {

#define EIGHTSER_CONDITIONAL_SERIALIZABLE_save(object, ...) EIGHTSER_CONDITIONAL_SERIALIZABLE(save, object, __VA_ARGS__)
#define EIGHTSER_CONDITIONAL_SERIALIZABLE_load(object, ...) EIGHTSER_CONDITIONAL_SERIALIZABLE(load, object, __VA_ARGS__)

#define EIGHTSER_CONDITIONAL_SERIALIZABLE_saveload(object, ...) \
    EIGHTSER_CONDITIONAL_SERIALIZABLE_save(object, __VA_ARGS__) \
        SERIALIZATION(load(archive, object);) \
    SERIALIZABLE_INIT() \
    EIGHTSER_CONDITIONAL_SERIALIZABLE_load(object, __VA_ARGS__) \

#define EIGHTSER_SERIALIZABLE(mode, object, ...) \
    void xxeightser<__VA_ARGS__>::mode(::eightser::ioarchive_t& archive, S& object) {

#define EIGHTSER_SERIALIZABLE_save(object, ...) EIGHTSER_SERIALIZABLE(save, object, __VA_ARGS__)
#define EIGHTSER_SERIALIZABLE_load(object, ...) EIGHTSER_SERIALIZABLE(load, object, __VA_ARGS__)

#define EIGHTSER_SERIALIZABLE_saveload(object, ...) \
    EIGHTSER_SERIALIZABLE_save(object, __VA_ARGS__) \
        SERIALIZATION(load(archive, object);) \
    SERIALIZABLE_INIT() \
    EIGHTSER_SERIALIZABLE_load(object, __VA_ARGS__)
// ~impl

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
    static_assert(std::negation<meta::is_unsupported<SerializableType>>::value,
        "The 'SerializableType' is an unsupported type for serialization.");

    instantiable_registry()->add<SerializableType>();

    #ifdef EIGHTSER_RTTI_ENABLE
    #ifdef EIGHTSER_ANY_SUPPORT_ENABLE
    any_registry()->add<SerializableType>();
    #endif // EIGHTSER_ANY_SUPPORT_ENABLE
    #endif // EIGHTSER_RTTI_ENABLE
}

template <typename SerializableType> SerializableType&& serializable(SerializableType&& object)
{
    serializable<typename std::decay<SerializableType>::type>();
    return std::forward<SerializableType>(object);
}

} // namepsace eightser

#endif // EIGHTSER_SERIALIZABLE_HPP
