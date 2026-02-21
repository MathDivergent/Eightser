#include <Eightser/StreamWrapper.hpp>

template class ::eightser::wrapper::ibyte_stream_t<std::vector<unsigned char>, false /*EndianSwap*/>;
template class ::eightser::wrapper::ibyte_stream_t<std::vector<unsigned char>, true /*EndianSwap*/>;
#ifdef EIGHTSER_FILESTREAM_ENABLE
template class ::eightser::wrapper::ifile_stream_t<>;
#endif // EIGHTSER_FILESTREAM_ENABLE


template class ::eightser::wrapper::obyte_stream_t<std::vector<unsigned char>, false /*EndianSwap*/>;
template class ::eightser::wrapper::obyte_stream_t<std::vector<unsigned char>, true /*EndianSwap*/>;
#ifdef EIGHTSER_FILESTREAM_ENABLE
template class ::eightser::wrapper::ofile_stream_t<>;
#endif // EIGHTSER_FILESTREAM_ENABLE