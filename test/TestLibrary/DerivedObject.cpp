#include "DerivedObject.hpp"

SERIALIZABLE_SAVELOAD(self, DerivedObject)
    BIN_SERIALIZABLE
    (
        archive & hierarchy<BaseObject>(self) & self.data;
    )
SERIALIZABLE_INIT()
