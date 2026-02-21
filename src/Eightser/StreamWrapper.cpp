#include <Eightser/StreamWrapper.hpp>

extern template class ::eightser::wrapper::ibyte_stream_t<>;
#ifdef EIGHTSER_FILESTREAM_ENABLE
extern template class ::eightser::wrapper::ifile_stream_t<>;
#endif // EIGHTSER_FILESTREAM_ENABLE

extern template class ::eightser::wrapper::obyte_stream_t<>;
#ifdef EIGHTSER_FILESTREAM_ENABLE
extern template class ::eightser::wrapper::ofile_stream_t<>;
#endif // EIGHTSER_FILESTREAM_ENABLE
