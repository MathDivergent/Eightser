#ifdef EIGHTSER_RTTI_ENABLE
#ifndef EIGHTSER_ANY_SUPPORT_DISABLE

#include <Eightser/Dynamic/AnyRegistry.hpp>

namespace eightser
{

namespace dynamic
{

void any_registry_t::save(ioarchive_t& archive, std::any& any, std::uint64_t hash)
{
    all.at(hash).save(archive, any);
}

void any_registry_t::load(ioarchive_t& archive, std::any& any, std::uint64_t hash)
{
    all.at(hash).load(archive, any);
}

any_registry_t* any_registry()
{
    static any_registry_t self; return &self;
}

} // namespace dynamic

} // namespace eightser

#endif // EIGHTSER_ANY_SUPPORT_DISABLE
#endif // EIGHTSER_RTTI_ENABLE
