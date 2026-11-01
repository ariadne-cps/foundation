/***************************************************************************
 *            test_paradigm.cpp
 *
 *  Copyright  2026
 *
 ****************************************************************************/

#include "foundation/paradigm.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

class TestParadigm : public Ariadne::Testing::ContextFixture
{
  public:
    using ContextFixture::ContextFixture;
    Void test_concept();
    Void test_runtime();
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("paradigm");
    ARIADNE_TESTING_ADD_TEST(TestParadigm, test_runtime);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}

Void
TestParadigm::test_runtime()
{
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(ApproximateTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::APPROXIMATE));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(ValidatedTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::VALIDATED));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(EffectiveTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::EFFECTIVE));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(ExactTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::EXACT));

    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(MetricTag::code()), ParadigmCodeType(7));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(OrderTag::code()), ParadigmCodeType(3));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(UpperTag::code()), ParadigmCodeType(2));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(LowerTag::code()), ParadigmCodeType(1));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(ApproximationTag::code()), ParadigmCodeType(0));

    MetricTag metric_default;
    OrderTag order_default;
    UpperTag upper_default;
    LowerTag lower_default;
    ApproximationTag approximation_default;

    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(metric_default.code()), ParadigmCodeType(7));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(order_default.code()), ParadigmCodeType(3));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(upper_default.code()), ParadigmCodeType(2));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(lower_default.code()), ParadigmCodeType(1));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(approximation_default.code()), ParadigmCodeType(0));

    ValidatedTag validated;
    MetricTag metric(validated);
    OrderTag order(validated);
    UpperTag upper(validated);
    LowerTag lower(validated);
    ApproximationTag approximation(validated);

    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(metric.code()), ParadigmCodeType(7));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(order.code()), ParadigmCodeType(3));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(upper.code()), ParadigmCodeType(2));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(lower.code()), ParadigmCodeType(1));
    ARIADNE_TESTING_EXPECT_EQUAL(static_cast<ParadigmCodeType>(approximation.code()), ParadigmCodeType(0));
}

// Compile-only concept checks: deliberately not called at runtime.
Void
TestParadigm::test_concept()
{
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ApproximateTag,ApproximateTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ApproximateTag,ValidatedTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ApproximateTag,EffectiveTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ApproximateTag,ExactTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ValidatedTag,ValidatedTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ValidatedTag,EffectiveTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ValidatedTag,ExactTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<EffectiveTag,EffectiveTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<EffectiveTag,ExactTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((WeakerThan<ExactTag,ExactTag>));

    ARIADNE_TESTING_COMPILE_CONCEPT((not WeakerThan<ValidatedTag,ApproximateTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((not WeakerThan<EffectiveTag,ApproximateTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((not WeakerThan<EffectiveTag,ValidatedTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((not WeakerThan<ExactTag,ApproximateTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((not WeakerThan<ExactTag,ValidatedTag>));
    ARIADNE_TESTING_COMPILE_CONCEPT((not WeakerThan<ExactTag,EffectiveTag>));
}
