#include <Eightser/Core.hpp>

SERIALIZABLE_ARCHIVE(::eightser::iarchive_t<::eightser::wrapper::ibyte_stream_t<>>);
#ifdef EIGHTSER_FILESTREAM_ENABLE
SERIALIZABLE_ARCHIVE(::eightser::iarchive_t<::eightser::wrapper::ifile_stream_t<>>);
#endif // EIGHTSER_FILESTREAM_ENABLE

SERIALIZABLE_ARCHIVE(::eightser::oarchive_t<::eightser::wrapper::obyte_stream_t<>>);
#ifdef EIGHTSER_FILESTREAM_ENABLE
SERIALIZABLE_ARCHIVE(::eightser::oarchive_t<::eightser::wrapper::ofile_stream_t<>>);
#endif // EIGHTSER_FILESTREAM_ENABLE
