#include "BaseObject.hpp"

#include <Eightser/Standard/string.hpp>

SERIALIZABLE_SAVE(self, BaseObject)
    BIN_SERIALIZABLE
    (
        archive << self.id << self.name;
    )
SERIALIZABLE_INIT()

SERIALIZABLE_LOAD(self, BaseObject)
    BIN_SERIALIZABLE
    (
        archive >> self.id >> self.name;
    )
SERIALIZABLE_INIT()
