#if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)
#include <Eightser/Standard/any.hpp>
#include <Eightser/Core.hpp>

#include <cstdint> // uint64_t

SERIALIZABLE_SAVE(any, std::any)
    BIN_SERIALIZABLE
    (
        std::uint64_t hash = any.type().hash_code();
        archive & hash;

        ::eightser::any_registry()->save(archive, any, hash);
    )
SERIALIZABLE_INIT()

SERIALIZABLE_LOAD(any, std::any)
    BIN_SERIALIZABLE
    (
        std::uint64_t hash{};
        archive & hash;

        ::eightser::any_registry()->load(archive, any, hash);
    )
SERIALIZABLE_INIT()
#endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE
