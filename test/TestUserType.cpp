#include <EightserTestingBase.hpp>

#include <Eightser/Standard/any.hpp>

TEST_SPACE()
{

struct Vector
{
    Vector(float x = 0.f, float y = 0.f, float z = 0.f)
        : X(x), Y(y), Z(z) {}

    float X;
    float Y;
    float Z;
};

struct Box
{
    Box() {}
    Box(const Vector& min, const Vector& max)
        : Min(min), Max(max) {}

    Vector Min;
    Vector Max;
};

} // TEST_SPACE

SERIALIZABLE_DECLARATION(Vector)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_SAVELOAD(self, Vector)
    BIN_SERIALIZABLE
    (
        archive & self.X & self.Y & self.Z;
    )
SERIALIZABLE_INIT()

SERIALIZABLE_DECLARATION(Box)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_SAVELOAD(self, Box)
    BIN_SERIALIZABLE
    (
        archive & self.Min & self.Max;
    )
SERIALIZABLE_INIT()

TEST(TestCommon, TestUserType)
{
    static Vector s_min(0.1f, 1.3f, 2.1f);
    static Vector s_max(3.2f, 2.f, 3.5f);

    static auto equal = [](const Vector& A, const Vector& B)
    {
        return A.X == B.X && A.Y == B.Y && A.Z == B.Z;
    };

    std::vector<unsigned char> storage;
    {
        Box box(s_min, s_max);

        auto ar = oarchive(storage);
        ar & box;
    }
    {
        Box box;

        auto ar = iarchive(storage);
        ar & box;

        EXPECT("Struct", equal(box.Min, s_min) && equal(box.Max, s_max));
    }
}

#include <Eightser/Standard/string.hpp>
#include <Eightser/Standard/any.hpp>

TEST_SPACE()
{

struct Product
{
    std::string name;
    std::size_t series;
    std::size_t price;
};

struct Printer : Product
{
    #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)
    std::any owner;
    #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE
};

} // TEST_SPACE

SERIALIZABLE_DECLARATION(Product)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_SAVELOAD(self, Product)
    BIN_SERIALIZABLE
    (
        archive & self.name & self.series & self.price;
    )
SERIALIZABLE_INIT()

SERIALIZABLE_DECLARATION(Printer)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_SAVELOAD(self, Printer)
    #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)
    BIN_SERIALIZABLE
    (
        archive & hierarchy<Product>(self);
        archive & self.owner;
    )
    #else
    BIN_SERIALIZABLE
    (
        archive & hierarchy<Product>(self);
    )
    #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE
SERIALIZABLE_INIT()

TEST(TestCommon, TestInheritance)
{
    #ifdef EIGHTSER_RTTI_ENABLE
    static std::string s_owner = "Jen";
    #endif // EIGHTSER_RTTI_ENABLE

    static Printer s_p;
    s_p.name = "Canon";
    s_p.series = 37868723;
    s_p.price = 1000;

    #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)
    s_p.owner = eightser::serializable(s_owner);
    #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE

    std::vector<unsigned char> storage;
    {
        Printer p = s_p;

        auto ar = oarchive(storage);
        ar & p;
    }
    {
        Printer p = s_p;

        auto ar = iarchive(storage);
        ar & p;

        #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_SUPPORT_ENABLE)
        auto owner = std::any_cast<std::string>(&p.owner);
        EXPECT("inheritance.inited", owner != nullptr && *owner == s_owner);
        #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_SUPPORT_ENABLE

        EXPECT("inheritance.value",
            p.name == s_p.name && p.series == s_p.series && p.price == s_p.price);
    }
}
