#include <Eightser/InstantiableRegistry.hpp>

namespace eightser
{

instantiable_registry_t* instantiable_registry()
{
    static instantiable_registry_t self; return &self;
}

} // namespace eightser
