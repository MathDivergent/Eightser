#ifdef EIGHTSER_RTTI_ENABLE
#ifdef EIGHTSER_ANY_SUPPORT_ENABLE

#include <Eightser/Standard/AnyRegistry.hpp>

namespace eightser
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

} // namespace eightser

#endif // EIGHTSER_ANY_SUPPORT_ENABLE
#endif // EIGHTSER_RTTI_ENABLE
