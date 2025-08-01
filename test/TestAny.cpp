#ifdef EIGHTSER_RTTI_ENABLE
#ifdef EIGHTSER_ANY_SUPPORT_ENABLE

#include <EightserTestingBase.hpp>

#include <Eightser/Standard/any.hpp>

#include <Eightser/Standard/string.hpp>
#include <Eightser/Standard/vector.hpp>
#include <Eightser/Standard/shared_ptr.hpp>

TEST_SPACE()
{

struct Entity : instantiable_t
{
    int e;
};

struct Boy : Entity
{
    int b;
};

struct Girl : Entity
{
    int g;
};

} // TEST_SPACE

SERIALIZABLE_DECLARATION(Boy)
    INSTANTIABLE(S)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE(saveload, self, Boy)
    SERIALIZATION
    (
        archive & hierarchy<Entity>(self) & self.b;
    )
SERIALIZABLE_INIT()

SERIALIZABLE_DECLARATION(Girl)
    INSTANTIABLE(S)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE(saveload, self, Girl)
    SERIALIZATION
    (
        archive & hierarchy<Entity>(self) & self.g;
    )
SERIALIZABLE_INIT()

SERIALIZABLE_DECLARATION(Entity)
    INSTANTIABLE(S)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE(saveload, self, Entity)
    SERIALIZATION
    (
        archive & self.e;
    )
SERIALIZABLE_INIT()

TEST(TestUtility, TestAny)
{
    using eightser::serializable; // do possible serialize data assigned to any

    enum class Property { Speed, Force, Intelige };

    static std::string sv_va_0 = "Tom";
    static int sv_va_1 = 20;
    static Property sv_va_2 = Property::Intelige;

    std::vector<unsigned char> storage;
    {
        std::vector<std::any> va = {
            "Jack", // serializable can be omitted, see type registry (0)
            serializable(sv_va_1), // (1)
            serializable(Property::Force) // (2)
        };

        // serializable<int>(); serializable with explicit type can be omitted, see previous type registry (1)
        va[0] = serializable(sv_va_0); // (0)
        va[2] = sv_va_2; // serializable can be omitted, see previous type registry (2)

        auto ar = oarchive(storage);
        ar & va;
    }
    {
        std::vector<std::any> va;

        auto ar = iarchive(storage);
        ar & va;

        ASSERT("std::any.inited",
            va[0].has_value() && va[1].has_value() && va[2].has_value());

        ASSERT("std::any.any_cast",
            std::any_cast<std::string>(&va[0]) != nullptr &&
            std::any_cast<int>(&va[1]) != nullptr &&
            std::any_cast<Property>(&va[2]) != nullptr);

        EXPECT("std::any.value",
            std::any_cast<std::string&>(va[0]) == sv_va_0 &&
            std::any_cast<int>(va[1]) == sv_va_1 &&
            std::any_cast<Property>(va[2]) == sv_va_2);
    }

    static Boy sv_b;
    sv_b.e = 332;
    sv_b.b = 8775;

    static Girl sv_g;
    sv_b.e = 42325;
    sv_g.g = 78;

    storage.clear();

    {
        std::shared_ptr<Entity> b = std::make_shared<Boy>(sv_b);
        std::shared_ptr<Entity> g = std::make_shared<Girl>(sv_g);

        std::any ab = serializable(b);
        std::any ag = serializable(g); // has same hash as above

        auto ar = oarchive(storage);
        ar & ab & ag;
    }
    {
        std::any ab;
        std::any ag;

        auto ar = iarchive(storage);
        ar & ab & ag;

        ASSERT("std::any.polymorphic.inited", ab.has_value() && ag.has_value());

        ASSERT("std::any.polymorphic.any_cast",
            std::any_cast<std::shared_ptr<Entity>>(&ab) != nullptr &&
            std::any_cast<std::shared_ptr<Entity>>(&ag) != nullptr);

        auto eb = std::any_cast<std::shared_ptr<Entity>>(ab);
        auto b = std::dynamic_pointer_cast<Boy>(eb);

        auto eg = std::any_cast<std::shared_ptr<Entity>>(ag);
        auto g = std::dynamic_pointer_cast<Girl>(eg);

        EXPECT("std::any.polymorphic.value",
            b != nullptr && b->e == sv_b.e && b->b == sv_b.b &&
            g != nullptr && g->e == sv_g.e && g->g == sv_g.g);
    }
}

#endif // EIGHTSER_ANY_SUPPORT_ENABLE
#endif // EIGHTSER_RTTI_ENABLE
