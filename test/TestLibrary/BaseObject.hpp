#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include <string>

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
    INSTANTIABLE_KEY(EIGHTSER_STRING_HASH("BaseObject"), S)
    #endif // EIGHTSER_RTTI_ENABLE
SERIALIZABLE_DECLARATION_INIT()

#endif // BASE_OBJECT_HPP
