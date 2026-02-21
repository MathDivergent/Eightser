#include <Eightser/Standard/vector.hpp>
#include <Eightser/Core.hpp>

// slow impl
SERIALIZABLE_SAVE(vector, std::vector<bool>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size = vector.size();
        archive & size;

        for (auto item : vector)
        {
            bool boolean = item;
            archive & boolean;
        }
    )
SERIALIZABLE_INIT()

SERIALIZABLE_LOAD(vector, std::vector<bool>)
    BIN_SERIALIZABLE
    (
        std::uint64_t size{};
        archive & size;

        vector.resize(size);

        for (auto item/*bit_reference*/ : vector)
        {
            bool boolean{};
            archive & boolean;
            item = boolean;
        }
    )
SERIALIZABLE_INIT()
