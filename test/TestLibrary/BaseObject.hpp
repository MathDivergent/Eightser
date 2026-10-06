#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include <string> // string, hash

#include <EightserTestingBase.hpp>

struct BaseObject : eightser::instantiable_t
{
    int id;
    std::string name;
};

SERIALIZABLE_DECLARATION(BaseObject)
    #ifdef EIGHTSER_RTTI_ENABLE
    INSTANTIABLE(S)
    #else
    INSTANTIABLE_KEY(std::hash<std::string>{}("BaseObject"), S)
    #endif // EIGHTSER_RTTI_ENABLE
SERIALIZABLE_DECLARATION_INIT()

#endif // BASE_OBJECT_HPP
