#include <EightserTestingBase.hpp>

#include <Eightser/Standard/string.hpp>
#include <Eightser/Standard/any.hpp>
#include <Eightser/Standard/shared_ptr.hpp>

TEST_SPACE()
{

struct AggregateType
{
    std::string string;
    #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_ENABLE)
    std::any data;
    #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_ENABLE
    struct InnerType
    {
        char symbol;
        float constant;
    };

    std::shared_ptr<InnerType> inner;
};

// aggregate with 64 member-variables
struct BigAggregateType
{
    int EIGHTSER_PLACEHOLDERS(64);
};

struct BaseAggregateType
{
    std::string data;
    int id;
};

struct DerivedAggregateType // : BaseAggregateType - serialization not supported for aggregate hierarchy
{
    BaseAggregateType base;
    bool state;
};

} // TEST_SPACE

TEST(TestCommon, TestAggregate)
{
    using eightser::serializable; // do possible serialize data assigned to any

    static std::string s_string = "Hello, Aggregate!";
    #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_ENABLE)
    static double s_data = 123.321;
    #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_ENABLE
    static char s_symbol = 's';
    static float s_constant = 2.17f;

    std::vector<unsigned char> storage;
    {
        AggregateType::InnerType inner { s_symbol, s_constant };
        AggregateType at {
            s_string,
            #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_ENABLE)
            serializable(s_data),
            #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_ENABLE
            std::make_shared<AggregateType::InnerType>(inner)
        };

        auto ar = oarchive(storage);
        ar & at;
    }
    {
        AggregateType at;

        auto ar = iarchive(storage);
        ar & at;

        #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_ENABLE)
        auto data = std::any_cast<double>(&at.data);
        ASSERT("inited.data", data != nullptr);
        #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_ENABLE

        ASSERT("inited.inner", at.inner != nullptr);

        #if defined(EIGHTSER_RTTI_ENABLE) && defined(EIGHTSER_ANY_ENABLE)
        EXPECT("value",
            at.string == s_string &&
            *data == s_data &&
            at.inner->symbol == s_symbol && at.inner->constant == s_constant
        );
        #else
        EXPECT("value",
            at.string == s_string &&
            at.inner->symbol == s_symbol && at.inner->constant == s_constant
        );
        #endif // EIGHTSER_RTTI_ENABLE && EIGHTSER_ANY_ENABLE
    }

    static int s_bat_0 = 209;
    static int s_bat_63 = 23190;

    storage.clear();
    {
        BigAggregateType bat;
        bat._0 = s_bat_0;
        bat._63 = s_bat_63;

        auto ar = oarchive(storage);
        ar & bat;
    }
    {
        BigAggregateType bat;

        auto ar = iarchive(storage);
        ar & bat;

        EXPECT("big.value", bat._0 == s_bat_0 && bat._63 == s_bat_63);
    }

    static std::string s_b_data = "+S&*h1lK_873";
    static int s_b_id = 982981;
    static bool s_d_state = true;

    storage.clear();
    {
        DerivedAggregateType dat { s_b_data, s_b_id, s_d_state };

        auto ar = oarchive(storage);
        ar & dat;
    }
    {
        DerivedAggregateType dat;

        auto ar = iarchive(storage);
        ar & dat;

        EXPECT("inheritance.value", dat.base.data == s_b_data && dat.base.id == s_b_id && dat.state == s_d_state);
    }
}
