#ifndef EIGHTSER_STREAM_WRAPPER_HPP
#define EIGHTSER_STREAM_WRAPPER_HPP

#include <cstddef> // size_t
#include <vector> // vector

#ifdef EIGHTSER_FILESTREAM_ENABLE
#include <fstream> // ifstream, ofstream
#endif // EIGHTSER_FILESTREAM_ENABLE

#include <Eightser/Memory.hpp>

#ifndef EIGHTSER_BYTE_STREAM_RESERVE_SIZE
    #define EIGHTSER_BYTE_STREAM_RESERVE_SIZE std::size_t(4096)
#endif // EIGHTSER_BYTE_STREAM_RESERVE_SIZE

namespace eightser
{

inline namespace wrapper
{

template <typename OutputStreamType = std::vector<unsigned char>, bool EndianSwap = false>
class obyte_stream_t
{
protected:
    using item_type = typename OutputStreamType::value_type;

protected:
    static_assert(sizeof(item_type) == 1, "Require byte size for 'eightser::wrapper::obyte_stream_t'.");

public:
    OutputStreamType& storage;

public:
    obyte_stream_t(OutputStreamType& stream) noexcept : storage(stream)
    {
        storage.reserve(EIGHTSER_BYTE_STREAM_RESERVE_SIZE); // default reserve memory
    }

    template <typename ArithmeticType>
    void call(ArithmeticType const* data, std::size_t size)
    {
        auto it = memory::const_byte_cast<item_type>(data);
        auto it_end = memory::const_byte_cast<item_type>(data + size);

        while (it != it_end)
        {
            for (std::size_t arithmetic_byte = 0; arithmetic_byte < sizeof(ArithmeticType); ++arithmetic_byte)
            {
                if constexpr (EndianSwap)
                {
                    storage.emplace_back(it[sizeof(ArithmeticType) - 1 - arithmetic_byte]);
                }
                else
                {
                    storage.emplace_back(it[arithmetic_byte]);
                }
            }

            it += sizeof(ArithmeticType);
        }
    }
};

template <typename InputStreamType = std::vector<unsigned char>, bool EndianSwap = false>
class ibyte_stream_t
{
protected:
    using item_type = typename InputStreamType::value_type;

protected:
    static_assert(sizeof(item_type) == 1, "Require byte size for 'eightser::wrapper::ibyte_stream_t'.");

public:
    InputStreamType& storage;
    std::size_t offset;

    ibyte_stream_t(InputStreamType& stream) noexcept : storage(stream), offset(0) {}

    template <typename ArithmeticType>
    void call(ArithmeticType* data, std::size_t size)
    {
        auto it = memory::byte_cast<item_type>(data);
        auto it_end = memory::byte_cast<item_type>(data + size);

        while (it != it_end)
        {
            for (std::size_t arithmetic_byte = 0; arithmetic_byte < sizeof(ArithmeticType); ++arithmetic_byte)
            {
                if constexpr (EndianSwap)
                {
                    it[sizeof(ArithmeticType) - 1 - arithmetic_byte] = storage[offset++];
                }
                else
                {
                    it[arithmetic_byte] = storage[offset++];
                }
            }

            it += sizeof(ArithmeticType);
        }
    }
};

#ifdef EIGHTSER_FILESTREAM_ENABLE
template <typename OutputStreamType = std::ofstream>
class ofile_stream_t
{
public:
    OutputStreamType& file;

public:
    ofile_stream_t(OutputStreamType& stream) noexcept : file(stream) {}

    template <typename ArithmeticType>
    void call(ArithmeticType const* data, std::size_t size)
    {
        file.write(memory::const_byte_cast(data), size * sizeof(ArithmeticType));
    }
};

template <typename InputStreamType = std::ifstream>
class ifile_stream_t
{
public:
    InputStreamType& file;

public:
    ifile_stream_t(InputStreamType& stream) noexcept : file(stream) {}

    template <typename ArithmeticType>
    void call(ArithmeticType* data, std::size_t size)
    {
        file.read(memory::byte_cast(data), size * sizeof(ArithmeticType));
    }
};
#endif // EIGHTSER_FILESTREAM_ENABLE

} // inline namespace wrapper

} // namespace eightser

extern template class EIGHTSER_API ::eightser::wrapper::ibyte_stream_t<>;
extern template class EIGHTSER_API ::eightser::wrapper::obyte_stream_t<>;

#ifdef EIGHTSER_FILESTREAM_ENABLE
extern template class EIGHTSER_API ::eightser::wrapper::ifile_stream_t<>;
extern template class EIGHTSER_API ::eightser::wrapper::ofile_stream_t<>;
#endif // EIGHTSER_FILESTREAM_ENABLE

#endif // EIGHTSER_STREAM_WRAPPER_HPP
