/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include "unity.h"
#define TEST_INSTANCES
#include "self_assessment_utils.h"

static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;

void setUp(void)
{
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}

void tearDown(void)
{
    endPutcharSpy(); /* Stop suppressing test output */
    if (SetToOneToFailInTearDown == 1)
    {
        /* These will be skipped internally if already failed/ignored */
        TEST_FAIL_MESSAGE("<= Failed in tearDown");
        TEST_IGNORE_MESSAGE("<= Ignored in tearDown");
    }
    if ((SetToOneMeanWeAlreadyCheckedThisGuy == 0) && (Unity.CurrentTestFailed > 0))
    {
        UnityPrint(": [[[[ Test Should Have Passed But Did Not ]]]]");
        UNITY_OUTPUT_CHAR('\n');
    }
}

void testFloatsWithinDelta(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_WITHIN(0.00003f, 187245.03485f, 187245.03488f);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 187245.0f, 187246.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 9273.2549f, 9273.2049f);
    TEST_ASSERT_FLOAT_WITHIN(0.007f, -726.93724f, -726.94424f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_NOT_WITHIN(0.05f, 9273.2549f, 9273.2049f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotWithinDelta(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_NOT_WITHIN(0.05f, 9273.2649f, 9273.2049f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 9273.2649f, 9273.2049f);
    VERIFY_FAILS_END
#endif
}

void testFloatsEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_FLOAT(187245.0f, 187246.0f);
    TEST_ASSERT_EQUAL_FLOAT(18724.5f, 18724.6f);
    TEST_ASSERT_EQUAL_FLOAT(9273.2549f, 9273.2599f);
    TEST_ASSERT_EQUAL_FLOAT(-726.93724f, -726.9374f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_FLOAT(-726.93724f, -726.9374f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(9273.9649f, 9273.0049f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(9273.9649f, 9273.0049f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualNegative1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(-9273.9649f, -9273.0049f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(-9273.9649f, -9273.0049f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualNegative2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(-9273.0049f, -9273.9649f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(-9273.0049f, -9273.9649f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualActualNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(85.963f, 0.0f / f_zero);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(85.963f, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualExpectedNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(0.0f / f_zero, 85.963f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 85.963f);
    VERIFY_FAILS_END
#endif
}

void testFloatsEqualBothNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 0.0f / f_zero);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_FLOAT(0.0f / f_zero, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualInfNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(1.0f / f_zero, 0.0f / f_zero);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualNaNInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(0.0f / f_zero, 1.0f / f_zero);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualActualInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(321.642f, 1.0f / f_zero);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(321.642f, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualExpectedInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(1.0f / f_zero, 321.642f);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 321.642f);
    VERIFY_FAILS_END
#endif
}

void testFloatsEqualBothInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 1.0f / f_zero);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_FLOAT(1.0f / f_zero, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualPlusMinusInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_NOT_EQUAL_FLOAT(1.0f / f_zero, -1.0f / f_zero);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, -1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsGreaterThan(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_GREATER_THAN_FLOAT(1.0f, 2.0f);
    TEST_ASSERT_GREATER_THAN_FLOAT(-1.0f, 1.0f);
    TEST_ASSERT_GREATER_THAN_FLOAT(-2.0f, -1.0f);
#endif
}

void testFloatsGreaterThanInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_GREATER_THAN_FLOAT(1.0f, 1.0f / f_zero);
    TEST_ASSERT_GREATER_THAN_FLOAT(-1.0f / f_zero, 1.0f / f_zero);
    TEST_ASSERT_GREATER_THAN_FLOAT(-1.0f / f_zero, 1.0f);
#endif
}

void testFloatsNotGreaterThan(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(2.0f, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterThanNanActual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(1.0f, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterThanNanThreshold(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterThanNanBoth(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f / f_zero, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterThanInfActual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(1.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterThanNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(1.0f, -1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterThanBothInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(1.0f / f_zero, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterThanBothNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_FLOAT(-1.0f / f_zero, -1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsGreaterOrEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(1.0f, 2.0f);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(2.0f, 2.0f);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(-1.0f, 1.0f);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(-2.0f, -1.0f);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(-2.0f, -2.0f);
#endif
}

void testFloatsGreaterOrEqualInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(1.0f, 1.0f / f_zero);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(-1.0f / f_zero, 1.0f / f_zero);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(-1.0f / f_zero, 1.0f);
#endif
}

void testFloatsNotGreaterOrEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(2.0f, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterOrEqualNanActual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(1.0f, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterOrEqualNanThreshold(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsGreaterOrEqualNanBoth(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f / f_zero, 0.0f / f_zero);
#endif
}

void testFloatsNotGreaterOrEqualInfActual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(1.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotGreaterOrEqualNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(1.0f, -1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsGreaterOrEqualBothInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(1.0f / f_zero, 1.0f / f_zero);
#endif
}

void testFloatsGreaterOrEqualBothNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(-1.0f / f_zero, -1.0f / f_zero);
#endif
}

void testFloatsLessThan(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_LESS_THAN_FLOAT(2.0f, 1.0f);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, -1.0f);
    TEST_ASSERT_LESS_THAN_FLOAT(-1.0f, -2.0f);
#endif
}

void testFloatsLessThanInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f / f_zero, 1.0f);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f / f_zero, -1.0f / f_zero);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, -1.0f / f_zero);
#endif
}

void testFloatsNotLessThan(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, 2.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessThanNanActual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessThanNanThreshold(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(0.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessThanNanBoth(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(0.0f / f_zero, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessThanInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessThanNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(-1.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessThanBothInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f / f_zero, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessThanBothNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_FLOAT(-1.0f / f_zero, -1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}
void testFloatsLessOrEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(2.0f, 1.0f);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(2.0f, 2.0f);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, -1.0f);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(-1.0f, -2.0f);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(-2.0f, -2.0f);
#endif
}

void testFloatsLessOrEqualInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f / f_zero, 1.0f);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f / f_zero, -1.0f / f_zero);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, -1.0f / f_zero);
#endif
}

void testFloatsNotLessOrEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, 2.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessOrEqualNanActual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessOrEqualNanThreshold(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(0.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsLessOrEqualNanBoth(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(0.0f / f_zero, 0.0f / f_zero);
#endif
}

void testFloatsNotLessOrEqualInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotLessOrEqualNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(-1.0f / f_zero, 1.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatsLessOrEqualBothInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f / f_zero, 1.0f / f_zero);
#endif
}

void testFloatsLessOrEqualBothNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(-1.0f / f_zero, -1.0f / f_zero);
#endif
}

void testFloatIsPosInf1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_INF(2.0f / f_zero);
#endif
}

void testFloatIsPosInf2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_INF(2.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNegInf1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NEG_INF(-3.0f / f_zero);
#endif
}

void testFloatIsNegInf2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_NEG_INF(-3.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotPosInf1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(2.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotPosInf2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NOT_INF(2.0f);
#endif
}

void testFloatIsNotNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NEG_INF(-999.876f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNan1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NAN(0.0f / f_zero);
#endif
}

void testFloatIsNan2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_NAN(0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotNan1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NAN(234.9f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotNan2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NOT_NAN(234.9f);
#endif
}

void testFloatInfIsNotNan(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NAN(1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatNanIsNotInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsDeterminate1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_DETERMINATE(0.0f);
    TEST_ASSERT_FLOAT_IS_DETERMINATE(123.3f);
    TEST_ASSERT_FLOAT_IS_DETERMINATE(-88.3f);
#endif
}

void testFloatIsDeterminate2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(-88.3f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotDeterminate1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(1.0f / f_zero);
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(-1.0f / f_zero);
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(0.0f / f_zero);
#endif
}

void testFloatIsNotDeterminate2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_DETERMINATE(-1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatTraitFailsOnInvalidTrait(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    UnityAssertFloatSpecial(1.0f, NULL, __LINE__, UNITY_FLOAT_INVALID_TRAIT);
    VERIFY_FAILS_END
#endif
}

void testEqualFloatArrays(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, -8.0f,  25.4f, -0.123f};
    float p1[] = {1.0f, -8.0f,  25.4f, -0.123f};
    float p2[] = {1.0f, -8.0f,  25.4f, -0.2f};
    float p3[] = {1.0f, -23.0f, 25.0f, -0.26f};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p3, 1);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(NULL, NULL, 1);
#endif
}

void testNotEqualFloatArraysExpectedNull(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float* p0 = NULL;
    float p1[] = {1.0f, 8.0f, 25.4f, 0.252f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysActualNull(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float* p1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArrays1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float p1[] = {1.0f, 8.0f, 25.4f, 0.252f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArrays2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float p1[] = {2.0f, 8.0f, 25.4f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArrays3(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float p1[] = {1.0f, 8.0f, 25.5f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysNegative1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-1.0f, -8.0f, -25.4f, -0.253f};
    float p1[] = {-1.0f, -8.0f, -25.4f, -0.252f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysNegative2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-1.0f, -8.0f, -25.4f, -0.253f};
    float p1[] = {-2.0f, -8.0f, -25.4f, -0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysNegative3(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-1.0f, -8.0f, -25.4f, -0.253f};
    float p1[] = {-1.0f, -8.0f, -25.5f, -0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testEqualFloatArraysNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 0.0f / f_zero, 25.4f, 0.253f};
    float p1[] = {1.0f, 0.0f / f_zero, 25.4f, 0.253f};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
#endif
}

void testEqualFloatArraysInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 1.0f / f_zero, 25.4f, 0.253f};
    float p1[] = {1.0f, 1.0f / f_zero, 25.4f, 0.253f};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
#endif
}

void testNotEqualFloatArraysLengthZero(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[1] = {0.0f};
    float p1[1] = {0.0f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 0);
    VERIFY_FAILS_END
#endif
}

void testFloatArraysWithin(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, -8.0f,  25.4f, -0.123f};
    float p1[] = {1.0f, -8.0f,  25.4f, -0.123f};
    float p2[] = {1.0f, -8.0f,  25.4f, -0.2f};
    float p3[] = {1.0f, -23.0f, 25.0f, -0.26f};
    float p4[] = {2.0f, -9.0f,  26.2f, 0.26f};
    float p5[] = {-1.0f, -7.0f, 29.0f, 2.6f};

    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, p0, p0, 1);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, p0, p0, 4);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, p0, p1, 4);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, p0, p2, 3);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, p0, p3, 1);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, p0, p4, 1);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, p0, p4, 4);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(2.0f, p0, p5, 1);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(2.0f, p0, p5, 2);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1.0f, NULL, NULL, 1);
#endif
}

void testFloatArraysWithinUnusualDelta(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {(float)-INFINITY, -8.0f, 25.4f, -0.123f};
    float p1[] = {(float)INFINITY, 10.1f};

    TEST_ASSERT_FLOAT_ARRAY_WITHIN(INFINITY, p0, p1, 2);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(NAN, p0, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testEqualFloatEachEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float p1[] = {-0.123f, -0.123f, -0.123f, -0.123f};
    float p2[] = {25.4f, 25.4f, 25.4f, -0.2f};
    float p3[] = {1.0f, -23.0f, 25.0f, -0.26f};

    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 1);
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 4);
    TEST_ASSERT_EACH_EQUAL_FLOAT(-0.123f, p1, 4);
    TEST_ASSERT_EACH_EQUAL_FLOAT(25.4f, p2, 3);
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p3, 1);
#endif
}

void testNotEqualFloatEachEqualActualNull(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float* p0 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(5, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatEachEqual1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {0.253f, 8.0f, 0.253f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(0.253f, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatEachEqual2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {8.0f, 8.0f, 8.0f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(8.0f, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatEachEqual3(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 1.0f, 1.0f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatEachEqualNegative1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-1.0f, -0.253f, -0.253f, -0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(-0.253f, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatEachEqualNegative2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-25.4f, -8.0f, -25.4f, -25.4f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(-25.4f, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatEachEqualNegative3(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-8.0f, -8.0f, -8.0f, -0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(-8.0f, p0, 4);
    VERIFY_FAILS_END
#endif
}

void testEqualFloatEachEqualNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {0.0f / f_zero, 0.0f / f_zero, 0.0f / f_zero, 0.0f / f_zero};

    TEST_ASSERT_EACH_EQUAL_FLOAT(0.0f / f_zero, p0, 4);
#endif
}

void testEqualFloatEachEqualInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f / f_zero, 1.0f / f_zero, 25.4f, 0.253f};

    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f / f_zero, p0, 2);
#endif
}

void testNotEqualFloatEachEqualLengthZero(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[1] = {0.0f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(0.0f, p0, 0);
    VERIFY_FAILS_END
#endif
}

void testFloatPrinting(void)
{
#if defined(UNITY_EXCLUDE_FLOAT_PRINT) || defined(UNITY_INCLUDE_DOUBLE) || !defined(USING_OUTPUT_SPY)
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_PRINT_FLOATING("0",            0.0f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("4.99e-07",     0.000000499f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("0.1004695",    0.100469499f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("2",            1.9999995f); /*Rounding to int place*/
    TEST_ASSERT_EQUAL_PRINT_FLOATING("1",            1.0f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("1.25",         1.25f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("7.999999",     7.999999f); /*Not rounding*/
    TEST_ASSERT_EQUAL_PRINT_FLOATING("16.00002",     16.00002f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("16.00004",     16.00004f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("16.00006",     16.00006f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("9999999",      9999999.0f); /*Last full print integer*/

    TEST_ASSERT_EQUAL_PRINT_FLOATING("-0",            -0.0f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-4.99e-07",     -0.000000499f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-0.1004695",    -0.100469499f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-2",            -1.9999995f); /*Rounding to int place*/
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-1",            -1.0f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-1.25",         -1.25f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-7.999999",     -7.999999f); /*Not rounding*/
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-16.00002",     -16.00002f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-16.00004",     -16.00004f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-16.00006",     -16.00006f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-9999999",      -9999999.0f); /*Last full print integer*/

    /* Fails, prints "4.294968e+09" due to FP math imprecision
     * TEST_ASSERT_EQUAL_PRINT_FLOATING("4.294967e+09",  4294967296.0f); */
    TEST_ASSERT_EQUAL_PRINT_FLOATING("5e+09",        5000000000.0f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("8e+09",        8.0e+09f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("8.309999e+09", 8309999104.0f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("1e+10",        1.0e+10f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("1e+10",        10000000000.0f);
    /* Some compilers have trouble with inexact float constants, a float cast works generally */
    TEST_ASSERT_EQUAL_PRINT_FLOATING("1.000055e+10", (float)1.000055e+10f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("1.1e+38",      (float)1.10000005e+38f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("1.635299e+10", 1.63529943e+10f);
    /* Fails, prints "3.402824e+38" due to FP math imprecision
     * TEST_ASSERT_EQUAL_PRINT_FLOATING("3.402823e+38", 3.40282346638e38f); */

    TEST_ASSERT_EQUAL_PRINT_FLOATING("-1e+10",        -1.0e+10f);
    /* Fails, prints "-3.402824e+38" due to FP math imprecision
     * TEST_ASSERT_EQUAL_PRINT_FLOATING("-3.402823e+38", -3.40282346638e38f); */
#endif
}

void testFloatPrintingRoundTiesToEven(void)
{
#if defined(UNITY_EXCLUDE_FLOAT_PRINT) || defined(UNITY_INCLUDE_DOUBLE) || !defined(USING_OUTPUT_SPY)
    TEST_IGNORE();
#else
  #ifdef UNITY_ROUND_TIES_AWAY_FROM_ZERO
    TEST_ASSERT_EQUAL_PRINT_FLOATING("0.0004882813",  0.00048828125f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("488281.3",      488281.25f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("5.000001e-07",  0.00000050000005f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-5.000001e-07", -0.00000050000005f);
  #else /* Default to Round ties to even */
    TEST_ASSERT_EQUAL_PRINT_FLOATING("0.0004882812",  0.00048828125f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("488281.2",      488281.25f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("5e-07",         0.00000050000005f);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-5e-07",        -0.00000050000005f);
  #endif
#endif
}

void testFloatPrintingInfinityAndNaN(void)
{
#if defined(UNITY_EXCLUDE_FLOAT_PRINT) || !defined(USING_OUTPUT_SPY)
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_PRINT_FLOATING("inf",   1.0f / f_zero);
    TEST_ASSERT_EQUAL_PRINT_FLOATING("-inf", -1.0f / f_zero);

    TEST_ASSERT_EQUAL_PRINT_FLOATING("nan",   0.0f / f_zero);
#endif
}

#if defined(UNITY_TEST_ALL_FLOATS_PRINT_OK) && defined(USING_OUTPUT_SPY)
#ifdef UNITY_INCLUDE_DOUBLE
static void printFloatValue(float f)
{
    char expected[18];

    startPutcharSpy();
    UnityPrintFloat(f);

    sprintf(expected, "%.9g", f);
    /* We print all NaN's as "nan", not "-nan" */
    if (strcmp(expected, "-nan") == 0) strcpy(expected, "nan");

    if (strcmp(expected, getBufferPutcharSpy()))
    {
        /* Fail with diagnostic printing */
        TEST_ASSERT_EQUAL_PRINT_FLOATING(expected, f);
    }
}
#else
static void printFloatValue(float f)
{
    char expected[18];
    char expected_lower[18];
    char expected_lower2[18];
    char expected_lower3[18];
    char expected_higher[18];
    char expected_higher2[18];
    char expected_higher3[18];

    startPutcharSpy();
    UnityPrintFloat(f);

    sprintf(expected, "%.7g", f);
    /* We print all NaN's as "nan", not "-nan" */
    if (strcmp(expected, "-nan") == 0) strcpy(expected, "nan");

    strcpy(expected_lower, expected);
    strcpy(expected_lower2, expected);
    strcpy(expected_lower3, expected);
    strcpy(expected_higher, expected);
    strcpy(expected_higher2, expected);
    strcpy(expected_higher3, expected);

    /* Allow for rounding differences in the last digit */
    double lower = (double)f * 0.99999995;
    double higher = (double)f * 1.00000005;

    if(isfinite(lower)) sprintf(expected_lower, "%.7g", lower);
    if(isfinite(higher)) sprintf(expected_higher, "%.7g", higher);

    /* Outside [1,10000000] allow for relative error of +/-2.5e-7 */
    if (f < 1.0 || f > 10000000)
    {
        double lower2 = (double)f * 0.99999985;
        double lower3 = (double)f * 0.99999975;
        double higher2 = (double)f * 1.00000015;
        double higher3 = (double)f * 1.00000025;

        if (isfinite(lower2)) sprintf(expected_lower2, "%.7g", lower2);
        if (isfinite(lower3)) sprintf(expected_lower3, "%.7g", lower3);
        if (isfinite(higher2)) sprintf(expected_higher2, "%.7g", higher2);
        if (isfinite(higher3)) sprintf(expected_higher3, "%.7g", higher3);
    }

    if (strcmp(expected, getBufferPutcharSpy()) != 0 &&
        strcmp(expected_lower, getBufferPutcharSpy()) != 0 &&
        strcmp(expected_lower2, getBufferPutcharSpy()) != 0 &&
        strcmp(expected_lower3, getBufferPutcharSpy()) != 0 &&
        strcmp(expected_higher, getBufferPutcharSpy()) != 0 &&
        strcmp(expected_higher2, getBufferPutcharSpy()) != 0 &&
        strcmp(expected_higher3, getBufferPutcharSpy()) != 0)
    {
        /* Fail with diagnostic printing */
        TEST_ASSERT_EQUAL_PRINT_FLOATING(expected, f);
    }
}
#endif
#endif

void testFloatPrintingRandomSamples(void)
{
#if !defined(UNITY_TEST_ALL_FLOATS_PRINT_OK) || !defined(USING_OUTPUT_SPY)
    TEST_IGNORE();
#else
    union { float f_value; uint32_t int_value; } u;

    /* These values are not covered by the MINSTD generator */
    u.int_value = 0x00000000; printFloatValue(u.f_value);
    u.int_value = 0x80000000; printFloatValue(u.f_value);
    u.int_value = 0x7fffffff; printFloatValue(u.f_value);
    u.int_value = 0xffffffff; printFloatValue(u.f_value);

    uint32_t a = 1;
    for(int num_tested = 0; num_tested < 1000000; num_tested++)
    {
        /* MINSTD pseudo-random number generator */
        a = (uint32_t)(((uint64_t)a * 48271u) % 2147483647u);

        /* MINSTD does not set the highest bit; test both possibilities */
        u.int_value = a;              printFloatValue(u.f_value);
        u.int_value = a | 0x80000000; printFloatValue(u.f_value);
    }
#endif
}
