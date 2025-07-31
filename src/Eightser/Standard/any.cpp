#ifdef EIGHTSER_RTTI_ENABLE
#ifdef EIGHTSER_ANY_SUPPORT_ENABLE
#include <Eightser/Standard/any.hpp>
#include <Eightser/Core.hpp>

#include <cstdint> // uint64_t

SERIALIZABLE(save, any, std::any)
    SERIALIZATION
    (
        std::uint64_t hash = any.type().hash_code();
        archive & hash;

        ::eightser::any_registry()->save(archive, any, hash);
    )
SERIALIZABLE_INIT()

SERIALIZABLE(load, any, std::any)
    SERIALIZATION
    (
        std::uint64_t hash{};
        archive & hash;

        ::eightser::any_registry()->load(archive, any, hash);
    )
SERIALIZABLE_INIT()
#endif // EIGHTSER_ANY_SUPPORT_ENABLE
#endif // EIGHTSER_RTTI_ENABLE
