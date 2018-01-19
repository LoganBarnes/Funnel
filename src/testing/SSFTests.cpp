#include <ssf/test_cases.hpp>
#include <ssf/ssf.hpp>
#include <gmock/gmock.h>

template <typename T>
class SSFTests : public ::testing::Test
{
};

typedef ::testing::Types<float, double> FloatTypes;
TYPED_TEST_CASE(SSFTests, FloatTypes);

using namespace ::testing;

TYPED_TEST(SSFTests, all_test_cases)
{
    for (auto &testCase : ssf::test_cases<TypeParam>()) {
        ssf::TestData<TypeParam> testData = testCase.func();
        EXPECT_THAT(ssf::run_simple_stupid_funnel_algorithm(testData.input),
                    ::testing::ContainerEq(testData.expected_output))
            << testCase.name;
    }
}
