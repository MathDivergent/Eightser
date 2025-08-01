#include <EightserTestingBase.hpp>

#include <Eightser/Standard/unique_ptr.hpp>

TEST_SPACE()
{

struct Parent : instantiable_t
{
    virtual ~Parent() = default;
    int p = 0;
};

struct Child : Parent
{
    int c = 0;
};

bool operator== (const Child& lhs, const Child& rhs)
{
    return lhs.p == rhs.p && lhs.c == rhs.c;
}

} // TEST_SPACE

SERIALIZABLE_DECLARATION(Parent)
    #ifdef EIGHTSER_RTTI_ENABLE
    INSTANTIABLE(S)
    #else
    INSTANTIABLE_KEY(EIGHTSER_STRING_HASH("Parent"), S)
    #endif // EIGHTSER_RTTI_ENABLE
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE(saveload, self, Parent)
    SERIALIZATION
    (
        archive & self.p;
    )
SERIALIZABLE_INIT()

SERIALIZABLE_DECLARATION(Child)
    #ifdef EIGHTSER_RTTI_ENABLE
    INSTANTIABLE(S)
    #else
    INSTANTIABLE_KEY(EIGHTSER_STRING_HASH("Child"), S)
    #endif // EIGHTSER_RTTI_ENABLE
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE(saveload, self, Child)
    SERIALIZATION
    (
        archive & hierarchy<Parent>(self) & self.c;
    )
SERIALIZABLE_INIT()

TEST(TestMemory, TestUniquePtr)
{
    static int sv_i = 8791;

    Child sv_c;
    sv_c.p = 506;
    sv_c.c = 1234;

    std::vector<unsigned char> storage;
    {
        std::unique_ptr<int> u_i(new int(sv_i));
        std::unique_ptr<Parent> u_p(new Child(sv_c));

        auto ar = oarchive(storage);
        ar & u_i & u_p;
    }
    {
        std::unique_ptr<int> u_i;
        std::unique_ptr<Parent> u_p;

        auto ar = iarchive(storage);
        ar & u_i & u_p;

        ASSERT("std::unique_ptr<>.inited", u_i != nullptr);
        EXPECT("std::unique_ptr<>.value", *u_i == sv_i);
        
        ASSERT("std::unique_ptr<polymorphic>.inited parent", u_p != nullptr);

        #ifdef EIGHTSER_RTTI_ENABLE
        auto raw_u_c = dynamic_cast<Child*>(u_p.get());
        #else
        auto raw_u_c = static_cast<Child*>(u_p.get());
        #endif // EIGHTSER_RTTI_ENABLE
        ASSERT("std::unique_ptr<polymorphic>.inited child", raw_u_c != nullptr);
        EXPECT("std::unique_ptr<polymorphic>.value", *raw_u_c == sv_c);
    }
}

#include <Eightser/Standard/shared_ptr.hpp>

#ifdef EIGHTSER_RTTI_ENABLE
TEST_SPACE()
{

struct A : instantiable_t
{
    int a = 0;
};

struct B : virtual A
{
    int b = 0;
};

struct C : virtual A
{
    int c = 0;
};

struct D : B, C
{
    int d = 0;
};

bool operator== (const D& lhs, const D& rhs)
{
    return lhs.a == rhs.a
        && lhs.b == rhs.b
        && lhs.c == rhs.c
        && lhs.d == rhs.d;
}

} // TEST_SPACE

SERIALIZABLE_DECLARATION(A)
    INSTANTIABLE(S)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_DECLARATION(B)
    INSTANTIABLE(S)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_DECLARATION(C)
    INSTANTIABLE(S)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE_DECLARATION(D)
    INSTANTIABLE(S)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE(saveload, self, A)
    SERIALIZATION
    (
        archive & self.a;
    )
SERIALIZABLE_INIT()

SERIALIZABLE(saveload, self, B)
    SERIALIZATION
    (
        archive & hierarchy<A>(self)
                & self.b;
    )
SERIALIZABLE_INIT()

SERIALIZABLE(saveload, self, C)
    SERIALIZATION
    (
        archive & hierarchy<A>(self)
                & self.c;
    )
SERIALIZABLE_INIT()

SERIALIZABLE(saveload, self, D)
    SERIALIZATION
    (
        archive & hierarchy<B, C>(self) // type order does not matter
                & self.d;
    )
SERIALIZABLE_INIT()

TEST(TestMemory, TestSharedPtr)
{
    D sv_d;
    sv_d.a = 1;
    sv_d.b = 2;
    sv_d.c = 3;
    sv_d.d = 4;
    
    std::vector<unsigned char> storage;
    {
        std::shared_ptr<D> s_d = std::make_shared<D>(sv_d);
        std::shared_ptr<B> s_b = s_d;
        std::shared_ptr<C> s_c = s_d;
        std::shared_ptr<A> s_a = s_d;

        auto ar = oarchive(storage);
        ar & s_b & s_a & s_c & s_d; // special shuffle
    }
    {
        std::shared_ptr<D> s_d;
        std::shared_ptr<B> s_b;
        std::shared_ptr<C> s_c;
        std::shared_ptr<A> s_a;

        {
            auto ar = iarchive(storage);
            ar & s_c;
            ar & s_a & s_d & s_b; // one more shuffle
        }

        ASSERT("std::shared_ptr<polymorphic>.inited",
            s_a != nullptr && s_b != nullptr && s_c != nullptr && s_d != nullptr);

        EXPECT("std::shared_ptr<polymorphic>.use_count",
            s_a.use_count() == 4 && s_b.use_count() == 4 &&
            s_c.use_count() == 4 && s_d.use_count() == 4);

        EXPECT("std::shared_ptr<polymorphic>.pure",
            std::dynamic_pointer_cast<void>(s_a) == std::dynamic_pointer_cast<void>(s_b) &&
            std::dynamic_pointer_cast<void>(s_b) == std::dynamic_pointer_cast<void>(s_c) &&
            std::dynamic_pointer_cast<void>(s_c) == std::dynamic_pointer_cast<void>(s_d));

        auto raw_s_d = s_d.get();
        auto raw_s_b = s_b.get();
        auto raw_s_c = s_c.get();
        auto raw_s_a = s_a.get();

        EXPECT("std::shared_ptr<polymorphic>.value",
            *raw_s_d == sv_d && *dynamic_cast<D*>(raw_s_b) == sv_d &&
            *dynamic_cast<D*>(raw_s_c) == sv_d && *dynamic_cast<D*>(raw_s_a) == sv_d);
    }
}
#endif // EIGHTSER_RTTI_ENABLE

#include <Eightser/Standard/weak_ptr.hpp>

TEST(TestMemory, TestWeakPtr)
{
    static int sv_i = 983258;

    Child sv_c;
    sv_c.p = 7458;
    sv_c.c = 589;

    std::vector<unsigned char> storage;
    {
        auto s_i = std::make_shared<int>(sv_i);
        auto s_p = std::make_shared<Child>(sv_c);

        std::weak_ptr<int> w_i = s_i;
        std::weak_ptr<Parent> w_p = s_p;

        auto ar = oarchive(storage);
        ar & w_i & w_p;
    }
    {
        std::weak_ptr<int> w_i;
        std::weak_ptr<Parent> w_p;

        auto ar = iarchive(storage);
        ar & w_i & w_p;

        ASSERT("std::weak_ptr<>.inited", !w_i.expired());
        EXPECT("std::weak_ptr<>.value", *w_i.lock() == sv_i);
        
        ASSERT("std::weak_ptr<polymorphic>.inited", !w_p.expired());

        #ifdef EIGHTSER_RTTI_ENABLE
        auto s_c = std::dynamic_pointer_cast<Child>(w_p.lock());
        #else
        auto s_c = std::static_pointer_cast<Child>(w_p.lock());
        #endif // EIGHTSER_RTTI_ENABLE

        EXPECT("std::weak_ptr<polymorphic>.value", *s_c == sv_c);
    }
}

TEST(TestMemory, TestSharedAndWeakPtr)
{
    static float sv_f = 0.5f;

    std::vector<unsigned char> storage;
    {
        std::shared_ptr<float> s_f = std::make_shared<float>(sv_f);
        std::weak_ptr<float> w1_f = s_f;
        std::weak_ptr<float> w2_f = s_f;

        auto ar = oarchive(storage);
        ar & w1_f & s_f & w2_f; // special shuffle
    }
    {
        std::shared_ptr<float> s_f;
        std::weak_ptr<float> w1_f;
        std::weak_ptr<float> w2_f;

        {
            auto ar = iarchive(storage);
            ar & w1_f & w2_f & s_f; // one more shuffle
        }

        ASSERT("std::shared_ptr<>.inited", s_f != nullptr);
        ASSERT("std::weak_ptr<>.inited", !w1_f.expired() && !w2_f.expired());

        EXPECT("std::shared_ptr<>.use_count", s_f.use_count() == 1);

        auto s1_f = w1_f.lock();
        auto s2_f = w2_f.lock();

        EXPECT("std::weak_ptr<>.use_count", w1_f.use_count() == 3 && w2_f.use_count() == 3);

        EXPECT("std::shared_ptr<>.value", *s_f == sv_f);
        EXPECT("std::weak_ptr<>.value", *w1_f.lock() == sv_f && *w2_f.lock() == sv_f);
    }
}

#include <Eightser/Standard/string.hpp>

TEST_SPACE()
{

struct Human
{
    std::string name;
    std::weak_ptr<Human> partner;

    Human(const std::string& name = "Unnamed") : name(name) {}
};

} // TEST_SPACE

SERIALIZABLE_DECLARATION(Human)
SERIALIZABLE_DECLARATION_INIT()

SERIALIZABLE(saveload, self, Human)
    SERIALIZATION
    (
        archive & self.name & self.partner;
    )
SERIALIZABLE_INIT()

TEST(TestMemory, TestReferenceCycles)
{
    static std::string sv_h1 = "Tom";
    static std::string sv_h2 = "Jack";
    static std::string sv_h3 = "Bruno";

    std::vector<unsigned char> storage;
    {
        std::shared_ptr<Human> s_h1 = std::make_shared<Human>(sv_h1);
        std::shared_ptr<Human> s_h2 = std::make_shared<Human>(sv_h2);
        std::shared_ptr<Human> s_h3 = std::make_shared<Human>(sv_h3);

        s_h1->partner = s_h2;
        s_h2->partner = s_h1;
        s_h3->partner = s_h3; // self

        auto ar = oarchive(storage);
        ar & s_h1 & s_h2 & s_h3;
    }
    {
        std::shared_ptr<Human> s_h1 = nullptr;
        std::shared_ptr<Human> s_h2 = nullptr;
        std::shared_ptr<Human> s_h3 = nullptr;

        {
            auto ar = iarchive(storage);
            ar & s_h1 & s_h2 & s_h3;
        }

        ASSERT("std::shared_ptr<cycle>.inited",
            s_h1 != nullptr && !s_h1->partner.expired() &&
            s_h2 != nullptr && !s_h2->partner.expired());

        EXPECT("std::shared_ptr<cycle>.use_count", s_h1.use_count() == 1 && s_h2.use_count() == 1);

        EXPECT("std::shared_ptr<cycle>.value",
            s_h1->name == sv_h1 && s_h1->partner.lock() == s_h2 &&
            s_h2->name == sv_h2 && s_h2->partner.lock() == s_h1);

        ASSERT("std::shared_ptr<self>.inited", s_h3 != nullptr && !s_h3->partner.expired());
        EXPECT("std::shared_ptr<self>.use_count", s_h1.use_count() == 1);

        EXPECT("std::shared_ptr<self>.value", s_h3->name == sv_h3 && s_h3->partner.lock() == s_h3);
    }
}
