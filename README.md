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
[   OK   ] TestMemory::TestReferenceCycles.std::shared_ptr<cycle>.inited
[   OK   ] TestMemory::TestReferenceCycles.std::shared_ptr<cycle>.use_count
[   OK   ] TestMemory::TestReferenceCycles.std::shared_ptr<cycle>.value
[   OK   ] TestMemory::TestReferenceCycles.std::shared_ptr<self>.inited
[   OK   ] TestMemory::TestReferenceCycles.std::shared_ptr<self>.use_count
[   OK   ] TestMemory::TestReferenceCycles.std::shared_ptr<self>.value
[   OK   ] TestMemory::TestSharedAndWeakPtr.std::shared_ptr<>.inited
[   OK   ] TestMemory::TestSharedAndWeakPtr.std::weak_ptr<>.inited
[   OK   ] TestMemory::TestSharedAndWeakPtr.std::shared_ptr<>.use_count
[   OK   ] TestMemory::TestSharedAndWeakPtr.std::weak_ptr<>.use_count
[   OK   ] TestMemory::TestSharedAndWeakPtr.std::shared_ptr<>.value
[   OK   ] TestMemory::TestSharedAndWeakPtr.std::weak_ptr<>.value
[   OK   ] TestMemory::TestWeakPtr.std::weak_ptr<>.inited
[   OK   ] TestMemory::TestWeakPtr.std::weak_ptr<>.value
[   OK   ] TestMemory::TestWeakPtr.std::weak_ptr<polymorphic>.inited
[   OK   ] TestMemory::TestWeakPtr.std::weak_ptr<polymorphic>.value
[   OK   ] TestMemory::TestSharedPtr.std::shared_ptr<polymorphic>.inited
[   OK   ] TestMemory::TestSharedPtr.std::shared_ptr<polymorphic>.use_count
[   OK   ] TestMemory::TestSharedPtr.std::shared_ptr<polymorphic>.pure
[   OK   ] TestMemory::TestSharedPtr.std::shared_ptr<polymorphic>.value
[   OK   ] TestMemory::TestUniquePtr.std::unique_ptr<>.inited
[   OK   ] TestMemory::TestUniquePtr.std::unique_ptr<>.value
[   OK   ] TestMemory::TestUniquePtr.std::unique_ptr<polymorphic>.inited parent
[   OK   ] TestMemory::TestUniquePtr.std::unique_ptr<polymorphic>.inited child
[   OK   ] TestMemory::TestUniquePtr.std::unique_ptr<polymorphic>.value
[   OK   ] TestLibrary::TestTracking.data track.data value
[   OK   ] TestLibrary::TestTracking.data track.pointer inited
[   OK   ] TestLibrary::TestTracking.data track.pointer value
[   OK   ] TestLibrary::TestTracking.data tarck.value
[   OK   ] TestLibrary::TestPolymorphicArchive.value
[   OK   ] TestLibrary::TestPolymorphicArchive.bad oarchive
[   OK   ] TestLibrary::TestPolymorphicArchive.bad iarchive
[   OK   ] TestLibrary::TestAbstract.inited
[   OK   ] TestLibrary::TestAbstract.traits
[   OK   ] TestLibrary::TestStreamWrapper.char storage
[   OK   ] TestLibrary::TestStreamWrapper.storage converting
[   OK   ] TestLibrary::TestExportInstantiable.export instantiable key.traits
[   OK   ] TestLibrary::TestExportInstantiable.export instantiable.equivalent
[   OK   ] TestLibrary::TestExportInstantiable.instantiable runtime key.traits
[   OK   ] TestLibrary::TestNoMacro.no-macro.value
[   OK   ] TestLibrary::TestNoMacro.no-macro polymorphic.inited
[   OK   ] TestLibrary::TestNoMacro.no-macro polymorphic.value
[   OK   ] TestLibrary::TestPartition.saveload partition.inited
[   OK   ] TestLibrary::TestPartition.saveload partition.value
[   OK   ] TestLibrary::TestNoTraits.no-traits.inited
[   OK   ] TestLibrary::TestNoTraits.no-traits.value
[   OK   ] TestLibrary::TestAggregateOverload.serialization as non-aggregate.
[   OK   ] TestLibrary::TestAggregateOverload.serialization as aggregate.
[   OK   ] TestLibrary::TestInstantiableRegistry.same polymorphic key
[   OK   ] TestLibrary::TestInstantiableRegistry.non-instantiable type
[   OK   ] TestLibrary::TestInstantiableRegistry.clone non-instantiable raw
[   OK   ] TestLibrary::TestInstantiableRegistry.clone non-instantiable shared
[   OK   ] TestLibrary::TestInheritance.write.hierarchy count
[   OK   ] TestLibrary::TestInheritance.read.inited
[   OK   ] TestLibrary::TestInheritance.read.hierarchy count
[   OK   ] TestLibrary::TestAccess.non-public inheritance.value
[   OK   ] TestLibrary::TestAccess.public instantiable
[   OK   ] TestLibrary::TestAccess.non-public instantiable
[   OK   ] TestLibrary::TestSwapEndian.value.read
[   OK   ] TestLibrary::TestSwapEndian.value.read.endian swap
[   OK   ] TestLibrary::TestSwapEndian.value.write
[   OK   ] TestLibrary::TestSwapEndian.value.write.endian swap
[   OK   ] TestLibrary::TestValidation.bad data track
[   OK   ] TestLibrary::TestValidation.garbage ptr.read
[   OK   ] TestLibrary::TestValidation.mixed track.raw shared
[   OK   ] TestLibrary::TestValidation.mixed track.shared raw
[   OK   ] TestDemo::TestExample.inited
[   OK   ] TestDemo::TestExample.value
[   OK   ] TestCommon::TestUnion.union binary
[   OK   ] TestCommon::TestUnion.union
[   OK   ] TestCommon::TestUserType.Struct
[   OK   ] TestCommon::TestPolymorphic.polymorphic.inited
[   OK   ] TestCommon::TestPolymorphic.polymorphic.traits
[   OK   ] TestCommon::TestPolymorphic.polymorphic.value
[   OK   ] TestCommon::TestPolymorphic.polymorphic.derived inited
[   OK   ] TestCommon::TestPolymorphic.polymorphic.derived traits
[   OK   ] TestCommon::TestPolymorphic.polymorphic.derived value
[   OK   ] TestCommon::TestEnum.Enum
[   OK   ] TestCommon::TestEnum.Scoped Enum
[   OK   ] TestCommon::TestNumber.int
[   OK   ] TestCommon::TestNumber.long long
[   OK   ] TestCommon::TestNumber.float
[   OK   ] TestCommon::TestNumber.short
[   OK   ] TestCommon::TestNumber.double
[   OK   ] TestCommon::TestNumber.char
[   OK   ] TestCommon::TestNumber.int32_t
[   OK   ] TestCommon::TestNumber.wchar_t
[   OK   ] TestCommon::TestDynamicArray.dynamic array.inited
[   OK   ] TestCommon::TestDynamicArray.dynamic array.size
[   OK   ] TestCommon::TestDynamicArray.dynamic array.inited inner
[   OK   ] TestCommon::TestDynamicArray.dynamic array.value
[   OK   ] TestCommon::TestAlias.inited
[   OK   ] TestCommon::TestAlias.value
[   OK   ] TestCommon::TestAlias.polymorphic inited
[   OK   ] TestCommon::TestAlias.polymorphic value
[   OK   ] TestCommon::TestInheritance.inheritance.value
[   OK   ] TestCommon::TestAggregate.inited.inner
[   OK   ] TestCommon::TestAggregate.value
[   OK   ] TestCommon::TestAggregate.big.value
[   OK   ] TestCommon::TestAggregate.inheritance.value
[   OK   ] TestConcurrency::TestAtomic.std::atomic<>
[   OK   ] Common::TestBitPack.inited
[   OK   ] Common::TestBitPack.value
[   OK   ] TestSTL::TestVector.std::vector<bool>
[   OK   ] TestSTL::TestVector.std::vector<>
[   OK   ] TestSTL::TestSet.std::set<>
[   OK   ] TestSTL::TestSet.std::unordered_set<>
[   OK   ] TestSTL::TestSet.std::multiset<>
[   OK   ] TestSTL::TestSet.std::unordered_multiset<>
[   OK   ] TestSTL::TestDeque.std::deque<>
[   OK   ] TestSTL::TestMap.std::map<>
[   OK   ] TestSTL::TestMap.std::unordered_map<>
[   OK   ] TestSTL::TestMap.std::multimap<>
[   OK   ] TestSTL::TestMap.std::unordered_multimap<>
[   OK   ] TestSTL::TestString.std::string
[   OK   ] TestSTL::TestString.std::wstring
[   OK   ] TestSTL::TestString.std::u16string
[   OK   ] TestSTL::TestString.std::u32string
[   OK   ] TestSTL::TestList.std::list<>
[   OK   ] TestSTL::TestForwardList.std::forward_list<>
[   OK   ] TestSTL::TestArray.std::array<>
[   OK   ] TestSTLAdapter::TestStack.std::stack<>
[   OK   ] TestSTLAdapter::TestQueue.std::queue<>
[   OK   ] TestSTLAdapter::TestPriorityQueue.std::priority_queue<>
[   OK   ] TestNumeric::TestValarray.std::valarray<>
[   OK   ] TestNumeric::TestComplex.std::complex<>
[   OK   ] TestUtility::TestTuple.std::tuple<empty>
[   OK   ] TestUtility::TestTuple.std::tuple<>
[   OK   ] TestUtility::TestOptional.std::optional<>.nullopt
[   OK   ] TestUtility::TestOptional.std::optional<>.has_value
[   OK   ] TestUtility::TestOptional.std::optional<>.value
[   OK   ] TestUtility::TestOptional.std::optional<atomic>.has_value
[   OK   ] TestUtility::TestOptional.std::optional<atomic>.value
[   OK   ] TestUtility::TestBitset.std::bitset<>.test
[   OK   ] TestUtility::TestBitset.std::bitset<>
[   OK   ] TestUtility::TestVariant.std::variant<>.index
[   OK   ] TestUtility::TestVariant.std::variant<>.get
[   OK   ] TestUtility::TestVariant.std::variant<monostate>.index
[   OK   ] TestUtility::TestPair.std::pair<>
[   OK   ] TestUtility::TestAny.std::any.inited
[   OK   ] TestUtility::TestAny.std::any.any_cast
[   OK   ] TestUtility::TestAny.std::any.value
[   OK   ] TestUtility::TestAny.std::any.polymorphic.inited
[   OK   ] TestUtility::TestAny.std::any.polymorphic.any_cast
[   OK   ] TestUtility::TestAny.std::any.polymorphic.value
