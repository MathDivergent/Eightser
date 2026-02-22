#ifndef EIGHTSER_STANDARD_ANY_REGISTRY_HPP
#define EIGHTSER_STANDARD_ANY_REGISTRY_HPP

#if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)

#include <cstdint> // uint64_t

#include <unordered_map> // unordered_map
#include <any> // any

#include <Eightser/PolymorphicArchive.hpp>

#include <Eightser/Detail/Meta.hpp>

namespace eightser
{

struct EIGHTSER_API any_registry_t final
{
public:
    struct any_proxy_t
    {
        // we use raw function ptr instead std::function to reach perfomance
        void(*save)(ioarchive_t&, std::any&) = nullptr;
        void(*load)(ioarchive_t&, std::any&) = nullptr;
    };

public:
    std::unordered_map<std::uint64_t, any_proxy_t> all;

public:
    template <typename SerializableType>
    void add()
    {
        static auto lock = false; if (lock) return;
        lock = true;

        any_proxy_t proxy;

        proxy.save = [](ioarchive_t& archive, std::any& any)
        {
            archive << std::any_cast<SerializableType&>(any);
        };

        proxy.load = [](ioarchive_t& archive, std::any& any)
        {
            any.emplace<SerializableType>();
            archive >> std::any_cast<SerializableType&>(any);
        };

        all.emplace(typeid(SerializableType).hash_code(), proxy);
    }

    void save(ioarchive_t& archive, std::any& any, std::uint64_t hash);
    void load(ioarchive_t& archive, std::any& any, std::uint64_t hash);
};

extern EIGHTSER_API any_registry_t* any_registry();

} // namespace eightser

#endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE

#endif // EIGHTSER_STANDARD_ANY_REGISTRY_HPP
