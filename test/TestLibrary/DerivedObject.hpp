#ifndef DERIVED_OBJECT_HPP
#define DERIVED_OBJECT_HPP

#include <EightserTestingBase.hpp>
#include "BaseObject.hpp"

struct DerivedObject : BaseObject
{
    float data;
};

SERIALIZABLE_DECLARATION(DerivedObject)
    #ifdef EIGHTSER_RTTI_ENABLE
    INSTANTIABLE(S)
    #else
    INSTANTIABLE_KEY(EIGHTSER_STRING_HASH("DerivedObject"), S)
    #endif // EIGHTSER_RTTI_ENABLE
SERIALIZABLE_DECLARATION_INIT()

#endif // DERIVED_OBJECT_HPP
