# Eightser - Simple serialization library

![Type](https://img.shields.io/badge/type-engine%20module-%235a6770)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue)
![Platforms](https://img.shields.io/badge/platforms-Windows%20%7C%20Linux%20%7C%20macOS-red)
![Tests](https://img.shields.io/badge/tests-automated-yellow)
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://opensource.org/license/mit)

`Eightser` is a flexible and extensible C++ library with many features, that will allow you to easily and quickly turn your objects into a sequence of bits. The library has a small code base, but enough to use it. Is an external module of the [Eightgine](https://github.com/MathDivergent/Eightgine) (game engine).

Use macros for code generation or read the documentation and write your own code.
There are 2 main serialization classes under your control: `oarchive_t` and `iarchive_t`.
Both classes can be equipped with additional helper classes via template parameters if needed.
A wrapper for streams will help you flexibly configure the process of writing and reading data.
The type registrar will allow you to properly serialize and deserialize polymorphic and abstract types.

A library implemented purely in C++20.

See last stable library version 4.1.0 [here](https://github.com/MathDivergent/Eightser/releases).

## Functional:

### Serialization of common types:
- All arithmetic types
- Enum or scoped enum
- Static or dynamic array
- Bit fields
- Little-endian, Big-endian

See full list [here](https://github.com/MathDivergent/Eightser/tree/master/include/Eightser/BuiltIn).

### Data tracking:
- Raw pointers
- Smart pointers: unique, shared, weak
- Aliasing as referencing

### Serialization of standard library types:
- C++11: string, vector, map, and many others
- C++17: any, optional, variant

See full list [here](https://github.com/MathDivergent/Eightser/tree/master/include/Eightser/Standard).

### Serialization of user classes:
- Aggregate (since C++17)
- Common/Union/Template
- Hierarchy
- Polymorphic/Abstract
- Virtual Inheritance

## Auto Tests:
See library testing [here](https://github.com/MathDivergent/Eightser/tree/master/test).

## Quick start:
Warning! This is a simple example. You can see more examples [here](https://github.com/MathDivergent/Eightser/tree/master/test).

Preparing:

```C++
#include <Eightser/Core.hpp>
```
Let's include serialization support of common STL types:
```C++
#include <Eightser/Standard/string.hpp>
#include <Eightser/Standard/vector.hpp>
#include <Eightser/Standard/map.hpp>
#include <Eightser/Standard/shared_ptr.hpp>
```
And let's write own serializable types:
```C++
enum class Property
{
    Speed,
    Force,
    inteligance,
    Fly
};

struct Prototype
{
    std::string name;
    std::vector<Property> properties;
};

struct Handbook
{
    std::map<int, std::shared_ptr<Prototype>> prototypes;
};
```
Since C++17 we can ommit it, for aggregate types:
```C++
// in .hpp files
SERIALIZABLE_DECLARATION(Prototype)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_DECLARATION(Handbook)
SERIALIZABLE_DECLARATION_INIT()
// ~

// in .cpp files
SERIALIZABLE_SAVELOAD(self, Prototype)
    BIN_SERIALIZABLE
    (
        archive & self.name & self.properties;
    )
SERIALIZABLE_INIT()

SERIALIZABLE_SAVELOAD(self, Handbook)
    BIN_SERIALIZABLE
    (
        archive & self.prototypes;
    )
SERIALIZABLE_INIT()
// ~
```
Explaining of using macros above:
- ```SERIALIZABLE_DECLARATION(<type>) <properties> SERIALIZABLE_DECLARATION_INIT()``` - Generate header code to access the implementation for the specified class.
- ```SERIALIZABLE_<mode>(<object>, <type>) <serialization> SERIALIZABLE_INIT()``` - Generate 'save/load/saveload' serialization functions for the specified class.
- ```<archive_type>_SERIALIZABLE(<body>)``` - Generate 'if' branch for 'bin/xml/json' and custom archive types (`bin` is the default).

### Using of serialization library:

Sinse we are going to use memory storage, let's define a variable for it:
```C++
std::vector<unsigned char> storage;
```
Let's prepare our data for save:
```C++
Handbook db;

auto zero = std::make_shared<Prototype>();
zero->name = "Zero";
zero->properties = {Property::Speed};

auto rew = std::make_shared<Prototype>();
rew->name = "Rew";
rew->properties = {Property::Force};

auto ifly = std::make_shared<Prototype>();
ifly->name = "I.Fly";
ifly->properties = {Property::inteligance, Property::Fly};

db.prototypes[0] = zero;
db.prototypes[3] = rew;
db.prototypes[2] = ifly;
```

Serialization prepared data:
```C++
auto ar = eightser::oarchive(storage);
ar & db;
```

Deserialization from storage:
```C++
Handbook db; // some other clear db

auto ar = eightser::iarchive(storage);
ar & db;
```
See full code here: [TestExample.cpp](https://github.com/MathDivergent/Eightser/tree/master/test/TestExample.cpp)

### Notes:
For ```oarchive_t``` objects, you may also using overloaded ```operator <<``` instead ```operator &```
or ```operator()```, and also ```operator >>``` for ```iarchive_t``` objects.

Examples:
```C++
// saving objects to the archive
archive << obj_0 << ... << obj_n;
// loading objects from the archive
archive >> obj_0 >> ... >> obj_n;

//depends on archive type
archive & obj_0 & ... & obj_n;
// or
archive(obj_0, ..., obj_n);
```
## License:
This library is an open source project licensed under: [MIT](https://opensource.org/licenses/MIT).
