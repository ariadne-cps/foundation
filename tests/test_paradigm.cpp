/***************************************************************************
 *            test_paradigm.cpp
 *
 *  Copyright  2026
 *
 ****************************************************************************/

#include "foundation/paradigm.hpp"
#include "utility/test.hpp"

using namespace Ariadne;

class TestParadigm
{
  public:
    Void test();
  private:
    Void test_concept();
    Void test_runtime();
};

Int main() {
    ARIADNE_TEST_CLASS(TestParadigm,TestParadigm());
    return ARIADNE_TEST_FAILURES;
}

Void
TestParadigm::test()
{
    ARIADNE_TEST_CALL(test_runtime());
}

Void
TestParadigm::test_runtime()
{
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(ApproximateTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::APPROXIMATE));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(ValidatedTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::VALIDATED));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(EffectiveTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::EFFECTIVE));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(ExactTag::code()), static_cast<ParadigmCodeType>(ParadigmCode::EXACT));

    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(MetricTag::code()), ParadigmCodeType(7));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(OrderTag::code()), ParadigmCodeType(3));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(UpperTag::code()), ParadigmCodeType(2));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(LowerTag::code()), ParadigmCodeType(1));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(ApproximationTag::code()), ParadigmCodeType(0));

    ValidatedTag validated;
    MetricTag metric(validated);
    OrderTag order(validated);
    UpperTag upper(validated);
    LowerTag lower(validated);
    ApproximationTag approximation(validated);

    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(metric.code()), ParadigmCodeType(7));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(order.code()), ParadigmCodeType(3));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(upper.code()), ParadigmCodeType(2));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(lower.code()), ParadigmCodeType(1));
    ARIADNE_TEST_EQUAL(static_cast<ParadigmCodeType>(approximation.code()), ParadigmCodeType(0));
}

// Compile-only concept checks: deliberately not called at runtime.
Void
TestParadigm::test_concept()
{
    ARIADNE_TEST_CONCEPT(WeakerThan<ApproximateTag,ApproximateTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<ApproximateTag,ValidatedTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<ApproximateTag,EffectiveTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<ApproximateTag,ExactTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<ValidatedTag,ValidatedTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<ValidatedTag,EffectiveTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<ValidatedTag,ExactTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<EffectiveTag,EffectiveTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<EffectiveTag,ExactTag>);
    ARIADNE_TEST_CONCEPT(WeakerThan<ExactTag,ExactTag>);

    ARIADNE_TEST_CONCEPT(not WeakerThan<ValidatedTag,ApproximateTag>);
    ARIADNE_TEST_CONCEPT(not WeakerThan<EffectiveTag,ApproximateTag>);
    ARIADNE_TEST_CONCEPT(not WeakerThan<EffectiveTag,ValidatedTag>);
    ARIADNE_TEST_CONCEPT(not WeakerThan<ExactTag,ApproximateTag>);
    ARIADNE_TEST_CONCEPT(not WeakerThan<ExactTag,ValidatedTag>);
    ARIADNE_TEST_CONCEPT(not WeakerThan<ExactTag,EffectiveTag>);
}
