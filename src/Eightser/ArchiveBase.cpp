#include <Eightser/ArchiveBase.hpp>

namespace eightser
{

ioarchive_t::ioarchive_t(::xxeightser_archive_traits_key_type trait, ::xxeightser_archive_type_key_type type, bool saveload)
: trait(trait), type(type), saveload(saveload) {}

#ifdef EIGHTSER_DEBUG
ioarchive_t::~ioarchive_t() {}
#endif // EIGHTSER_DEBUG

} // namespace eightser
