#ifndef EIGHTSER_CORE_HPP
#define EIGHTSER_CORE_HPP

#include <Eightser/Config.hpp>

#include <Eightser/Serializable.hpp>
#include <Eightser/Hash.hpp>

#include <Eightser/InstantiableRegistry.hpp>

#include <Eightser/OArchive.hpp>
#include <Eightser/IArchive.hpp>

#include <Eightser/BuiltIn/Common.hpp>
#include <Eightser/BuiltIn/Aggregate.hpp>
#include <Eightser/BuiltIn/Union.hpp>

#include <Eightser/BuiltIn/ApplyFunctor.hpp>
#include <Eightser/BuiltIn/HierarchyTrack.hpp>
#include <Eightser/BuiltIn/DataTrack.hpp>

VISIBLE_SERIALIZABLE_ARCHIVE_DECLARATION((EIGHTSER_API), 0, ::eightser::iarchive_t<::eightser::wrapper::ibyte_stream_t<>>)
#ifdef EIGHTSER_FILESTREAM_ENABLE
VISIBLE_SERIALIZABLE_ARCHIVE_DECLARATION((EIGHTSER_API), 1, ::eightser::iarchive_t<::eightser::wrapper::ifile_stream_t<>>)
#endif // EIGHTSER_FILESTREAM_ENABLE

VISIBLE_SERIALIZABLE_ARCHIVE_DECLARATION((EIGHTSER_API), 0, ::eightser::oarchive_t<::eightser::wrapper::obyte_stream_t<>>)
#ifdef EIGHTSER_FILESTREAM_ENABLE
VISIBLE_SERIALIZABLE_ARCHIVE_DECLARATION((EIGHTSER_API), 1, ::eightser::oarchive_t<::eightser::wrapper::ofile_stream_t<>>)
#endif // EIGHTSER_FILESTREAM_ENABLE

#endif // EIGHTSER_CORE_HPP
