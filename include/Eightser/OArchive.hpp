#ifndef EIGHTSER_OARCHIVE_HPP
#define EIGHTSER_OARCHIVE_HPP

#include <cstdint> // uintptr_t
#include <unordered_map> // unordered_map

#include <Eightser/ArchiveBase.hpp>
#include <Eightser/Memory.hpp>
#include <Eightser/Serializable.hpp>

#include <Eightser/InstantiableRegistry.hpp>

#include <Eightser/StreamWrapper.hpp>

#include <Eightser/Detail/Meta.hpp>
#include <Eightser/Detail/Macro.hpp>

namespace eightser
{

template <class StreamWrapperType>
class oarchive_t : public ioarchive_t
{
public:
    template <typename HoldPointerType>
    struct oarchive_track_overload_t
    {
    protected:
        std::unordered_map<std::uintptr_t, bool> xxpointer;

    public:
        auto pointer(HoldPointerType const&) noexcept -> decltype(xxpointer)& { return xxpointer; }
    };

    template <typename... HoldPointerTypes>
    struct oarchive_track_t : public oarchive_track_overload_t<HoldPointerTypes>...
    {
    protected:
        #ifdef EIGHTSER_PTRTRACK_ENABLE
        std::unordered_map<std::uintptr_t, std::unordered_map<::xxeightser_instantiable_traits_key_type, bool>> xxhierarchy;
        #endif // EIGHTSER_PTRTRACK_ENABLE
    public:
        using oarchive_track_overload_t<HoldPointerTypes>::pointer...;
        #ifdef EIGHTSER_PTRTRACK_ENABLE
        auto hierarchy() noexcept -> decltype(xxhierarchy)& { return xxhierarchy; }
        #endif // EIGHTSER_PTRTRACK_ENABLE

        #ifdef EIGHTSER_DEBUG
        template <typename PointerType>
        bool is_mixed(std::uintptr_t refer_key, PointerType const& pointer) const noexcept
        {
            using pointer_traits = memory::pointer_traits<PointerType>;
            using pointer_hold_type = typename pointer_traits::template pointer_template<INSTANTIABLE_TYPE>;

            return ((std::is_same_v<pointer_hold_type, HoldPointerTypes> ?
                    0 : oarchive_track_overload_t<HoldPointerTypes>::xxpointer.count(refer_key)) + ...) > 0;
        }
        #endif // EIGHTSER_DEBUG
    };

public:
    using TrackingType = oarchive_track_t<POINTER_HOLD_TYPES>;

private:
    StreamWrapperType xxstream;
    TrackingType xxtracking;

public:
    template <typename OutputStreamType>
    oarchive_t(OutputStreamType& stream, ::xxeightser_archive_type_key_type type = bin)
        : ioarchive_t(::xxeightser_archive_traits<oarchive_t>::key, type, false)
        , xxstream{stream}, xxtracking() {}

    StreamWrapperType& stream() noexcept { return xxstream; }
    TrackingType& tracking() noexcept { return xxtracking; }

    template <typename SerializableType>
    oarchive_t& operator<< (SerializableType const& data) { return operator()(data); }

    template <typename SerializableType>
    oarchive_t& operator& (SerializableType const& data) { return operator()(data); }

    template <typename SerializableType, typename... SerializableTypes>
    oarchive_t& operator() (SerializableType const& data, SerializableTypes const&... datas)
    {
        ::xxeightser<SerializableType>::save(*this, const_cast<SerializableType&>(data));
        return operator()(datas...);
    }

    oarchive_t& operator() () noexcept { return *this; }
};

// create default oarchive_t with wrapper::obyte_stream_t<>
template <typename OutputStreamType>
oarchive_t<wrapper::obyte_stream_t<OutputStreamType>> oarchive(OutputStreamType& stream, ::xxeightser_archive_type_key_type type = bin)
{
    return { stream, type };
}

template <template <class, typename...> class StreamWrapperTemplate,
          typename OutputStreamType>
oarchive_t<StreamWrapperTemplate<OutputStreamType>> oarchive(OutputStreamType& stream, ::xxeightser_archive_type_key_type type = bin)
{
    return { stream, type };
}

template <class StreamWrapperType,
          typename OutputStreamType>
oarchive_t<StreamWrapperType> oarchive(OutputStreamType& stream, ::xxeightser_archive_type_key_type type = bin)
{
    return { stream, type };
}

namespace meta
{

template <class StreamWrapperType>
struct is_oarchive<oarchive_t<StreamWrapperType>> : std::true_type {};

} // namespace meta

} // namespace eightser

#endif // EIGHTSER_OARCHIVE_HPP
