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

void testInt64ArrayWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};
    UNITY_INT64 actualSmallDelta[] = {12345001, -12344996, 12345005};
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    TEST_ASSERT_INT64_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_INT64_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
#endif
}

void testInt64ArrayWithinDeltaAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};
    UNITY_INT64 actualSmallDelta[] = {12345001, -12344996, 12345005};
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    TEST_ASSERT_INT64_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_INT64_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
#endif
}

void tesUInt64ArrayNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayNotWithinDeltaAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayWithinDeltaPointless(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayWithinDeltaPointlessAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayWithinDeltaExpectedNull(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayWithinDeltaExpectedNullAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 actualBigDelta[] = {12345101, -12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayWithinDeltaActualNull(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayWithinDeltaActualNullAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testInt64ArrayWithinDeltaSamePointer(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};

    TEST_ASSERT_UINT64_ARRAY_WITHIN(110, expected, expected, 3);
#endif
}

void testInt64ArrayWithinDeltaSamePointerAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 expected[] = {12345000, -12344995, 12345005};

    TEST_ASSERT_INT64_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
#endif
}

void testIntArrayWithinDelta(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};
    UNITY_INT actualSmallDelta[] = {5001, -4996, 5005};
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    TEST_ASSERT_INT_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_INT_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
}

void testIntArrayWithinDeltaAndMessage(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};
    UNITY_INT actualSmallDelta[] = {5001, -4996, 5005};
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
}

void testIntArrayNotWithinDelta(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testIntArrayNotWithinDeltaAndMessage(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testIntArrayWithinDeltaPointless(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testIntArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testIntArrayWithinDeltaExpectedNull(void)
{
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testIntArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testIntArrayWithinDeltaActualNull(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testIntArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testIntArrayWithinDeltaSamePointer(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};

    TEST_ASSERT_INT_ARRAY_WITHIN(110, expected, expected, 3);
}

void testIntArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_INT expected[] = {5000, -4995, 5005};

    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
}

void testInt16ArrayWithinDelta(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};
    UNITY_INT16 actualSmallDelta[] = {5001, -4996, 5005};
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    TEST_ASSERT_INT16_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_INT16_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
}

void testInt16ArrayWithinDeltaAndMessage(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};
    UNITY_INT16 actualSmallDelta[] = {5001, -4996, 5005};
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
}

void testInt16ArrayNotWithinDelta(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testInt16ArrayNotWithinDeltaAndMessage(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt16ArrayWithinDeltaPointless(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testInt16ArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt16ArrayWithinDeltaExpectedNull(void)
{
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testInt16ArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_INT16 actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt16ArrayWithinDeltaActualNull(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testInt16ArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt16ArrayWithinDeltaSamePointer(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};

    TEST_ASSERT_INT16_ARRAY_WITHIN(110, expected, expected, 3);
}

void testInt16ArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};

    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
}

void testInt8ArrayWithinDelta(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};
    UNITY_INT8 actualSmallDelta[] = {21, -94, 55};
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    TEST_ASSERT_INT8_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_INT8_ARRAY_WITHIN(11, expected, actualBigDelta, 3);
}

void testInt8ArrayWithinDeltaAndMessage(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};
    UNITY_INT8 actualSmallDelta[] = {21, -94, 55};
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    TEST_ASSERT_INT8_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_INT8_ARRAY_WITHIN_MESSAGE(11, expected, actualBigDelta, 3, "Custom Message.");
}

void testInt8ArrayNotWithinDelta(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testInt8ArrayNotWithinDeltaAndMessage(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt8ArrayWithinDeltaPointless(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN(11, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testInt8ArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN_MESSAGE(11, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt8ArrayWithinDeltaExpectedNull(void)
{
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN(11, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testInt8ArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_INT8 actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN_MESSAGE(11, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt8ArrayWithinDeltaActualNull(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN(11, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testInt8ArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_ARRAY_WITHIN_MESSAGE(11, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testInt8ArrayWithinDeltaSamePointer(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};

    TEST_ASSERT_INT8_ARRAY_WITHIN(11, expected, expected, 3);
}

void testInt8ArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_INT8 expected[] = {20, -95, 55};

    TEST_ASSERT_INT8_ARRAY_WITHIN_MESSAGE(11, expected, expected, 3, "Custom Message.");
}

void testCHARArrayWithinDelta(void)
{
    char expected[] = {20, -95, 55};
    char actualSmallDelta[] = {21, -94, 55};
    char actualBigDelta[] = {11, -86, 45};

    TEST_ASSERT_CHAR_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_CHAR_ARRAY_WITHIN(11, expected, actualBigDelta, 3);
}

void testCHARArrayWithinDeltaAndMessage(void)
{
    char expected[] = {20, -95, 55};
    char actualSmallDelta[] = {21, -94, 55};
    char actualBigDelta[] = {11, -86, 45};

    TEST_ASSERT_CHAR_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_CHAR_ARRAY_WITHIN_MESSAGE(11, expected, actualBigDelta, 3, "Custom Message.");
}

void testCHARArrayNotWithinDelta(void)
{
    char expected[] = {20, -95, 55};
    char actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testCHARArrayNotWithinDeltaAndMessage(void)
{
    char expected[] = {20, -95, 55};
    char actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testCHARArrayWithinDeltaPointless(void)
{
    char expected[] = {20, -95, 55};
    char actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN(11, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testCHARArrayWithinDeltaPointlessAndMessage(void)
{
    char expected[] = {20, -95, 55};
    char actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN_MESSAGE(11, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testCHARArrayWithinDeltaExpectedNull(void)
{
    char actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN(11, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testCHARArrayWithinDeltaExpectedNullAndMessage(void)
{
    char actualBigDelta[] = {11, -86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN_MESSAGE(11, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testCHARArrayWithinDeltaActualNull(void)
{
    char expected[] = {20, -95, 55};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN(11, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testCHARArrayWithinDeltaActualNullAndMessage(void)
{
    char expected[] = {20, -95, 55};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_ARRAY_WITHIN_MESSAGE(11, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testCHARArrayWithinDeltaSamePointer(void)
{
    char expected[] = {20, -95, 55};

    TEST_ASSERT_CHAR_ARRAY_WITHIN(11, expected, expected, 3);
}

void testCHARArrayWithinDeltaSamePointerAndMessage(void)
{
    char expected[] = {20, -95, 55};

    TEST_ASSERT_CHAR_ARRAY_WITHIN_MESSAGE(11, expected, expected, 3, "Custom Message.");
}

void testUInt64ArrayWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};
    UNITY_UINT64 actualSmallDelta[] = {12345001, 12344996, 12345005};
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    TEST_ASSERT_UINT64_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_UINT64_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
#endif
}

void testUInt64ArrayWithinDeltaAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};
    UNITY_UINT64 actualSmallDelta[] = {12345001, 12344996, 12345005};
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    TEST_ASSERT_UINT64_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_UINT64_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
#endif
}

void testUInt64ArrayNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayNotWithinDeltaAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayWithinDeltaPointless(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayWithinDeltaPointlessAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayWithinDeltaExpectedNull(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayWithinDeltaExpectedNullAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 actualBigDelta[] = {12345101, 12344896, 12345055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayWithinDeltaActualNull(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayWithinDeltaActualNullAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testUInt64ArrayWithinDeltaSamePointer(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};

    TEST_ASSERT_UINT64_ARRAY_WITHIN(110, expected, expected, 3);
#endif
}

void testUInt64ArrayWithinDeltaSamePointerAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {12345000, 12344995, 12345005};

    TEST_ASSERT_UINT64_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
#endif
}

void testUIntArrayWithinDelta(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};
    UNITY_UINT actualSmallDelta[] = {125001, 124996, 125005};
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    TEST_ASSERT_UINT_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_UINT_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
}

void testUIntArrayWithinDeltaAndMessage(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};
    UNITY_UINT actualSmallDelta[] = {125001, 124996, 125005};
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    TEST_ASSERT_UINT_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_UINT_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
}

void testUIntArrayNotWithinDelta(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testUIntArrayNotWithinDeltaAndMessage(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUIntArrayWithinDeltaPointless(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testUIntArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testUIntArrayWithinDeltaExpectedNull(void)
{
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testUIntArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_UINT actualBigDelta[] = {125101, 124896, 125055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUIntArrayWithinDeltaActualNull(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testUIntArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUIntArrayWithinDeltaSamePointer(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};

    TEST_ASSERT_UINT_ARRAY_WITHIN(110, expected, expected, 3);
}

void testUIntArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_UINT expected[] = {125000, 124995, 125005};

    TEST_ASSERT_UINT_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
}

void testUInt16ArrayWithinDelta(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};
    UNITY_UINT16 actualSmallDelta[] = {5001, 4996, 5005};
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    TEST_ASSERT_UINT16_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_UINT16_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
}

void testUInt16ArrayWithinDeltaAndMessage(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};
    UNITY_UINT16 actualSmallDelta[] = {5001, 4996, 5005};
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    TEST_ASSERT_UINT16_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_UINT16_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
}

void testUInt16ArrayNotWithinDelta(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testUInt16ArrayNotWithinDeltaAndMessage(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt16ArrayWithinDeltaPointless(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testUInt16ArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt16ArrayWithinDeltaExpectedNull(void)
{
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testUInt16ArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_UINT16 actualBigDelta[] = {5101, 4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt16ArrayWithinDeltaActualNull(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testUInt16ArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt16ArrayWithinDeltaSamePointer(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};

    TEST_ASSERT_UINT16_ARRAY_WITHIN(110, expected, expected, 3);
}

void testUInt16ArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_UINT16 expected[] = {5000, 4995, 5005};

    TEST_ASSERT_UINT16_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
}

void testUInt8ArrayWithinDelta(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};
    UNITY_UINT8 actualSmallDelta[] = {21, 94, 55};
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    TEST_ASSERT_UINT8_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_UINT8_ARRAY_WITHIN(11, expected, actualBigDelta, 3);
}

void testUInt8ArrayWithinDeltaAndMessage(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};
    UNITY_UINT8 actualSmallDelta[] = {21, 94, 55};
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(11, expected, actualBigDelta, 3, "Custom Message.");
}

void testUInt8ArrayNotWithinDelta(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testUInt8ArrayNotWithinDeltaAndMessage(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt8ArrayWithinDeltaPointless(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN(11, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testUInt8ArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(11, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt8ArrayWithinDeltaExpectedNull(void)
{
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN(11, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testUInt8ArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_UINT8 actualBigDelta[] = {11, 86, 45};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(11, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt8ArrayWithinDeltaActualNull(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN(11, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testUInt8ArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(11, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testUInt8ArrayWithinDeltaSamePointer(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};

    TEST_ASSERT_UINT8_ARRAY_WITHIN(11, expected, expected, 3);
}

void testUInt8ArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_UINT8 expected[] = {20, 95, 55};

    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(11, expected, expected, 3, "Custom Message.");
}

void testHEX64ArrayWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};
    UNITY_UINT64 actualSmallDelta[] = {0xABCD123500000000, 0xABCD112100000000, 0xABCD127700000000};
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    TEST_ASSERT_HEX64_ARRAY_WITHIN(0x100000000, expected, actualSmallDelta, 3);
    TEST_ASSERT_HEX64_ARRAY_WITHIN(0x6E00000000, expected, actualBigDelta, 3);
#endif
}

void testHEX64ArrayWithinDeltaShouldNotHaveSignIssues(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0x7FFFFFFFFFFFFFFF, 0x8000000000000000};
    UNITY_UINT64 actualBigDelta[] = {0x8000000000000000, 0x7FFFFFFFFFFFFFFF};

    TEST_ASSERT_HEX64_ARRAY_WITHIN(1, expected, actualBigDelta, 2);
#endif
}

void testHEX64ArrayWithinDeltaAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};
    UNITY_UINT64 actualSmallDelta[] = {0xABCD123500000000, 0xABCD112100000000, 0xABCD127700000000};
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    TEST_ASSERT_HEX64_ARRAY_WITHIN_MESSAGE(0x100000000, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_HEX64_ARRAY_WITHIN_MESSAGE(0x6E00000000, expected, actualBigDelta, 3, "Custom Message.");
#endif
}

void testHEX64ArrayNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN(0x100000000, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayNotWithinDeltaAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN_MESSAGE(0x100000000, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayWithinDeltaPointless(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN(0x6E00000000, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayWithinDeltaPointlessAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN_MESSAGE(0x6E00000000, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayWithinDeltaExpectedNull(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN(0x6E00000000, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayWithinDeltaExpectedNullAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 actualBigDelta[] = {0xABCD126700000000, 0xABCD118800000000, 0xABCD12AC00000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN_MESSAGE(0x6E00000000, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayWithinDeltaActualNull(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN(0x6E00000000, expected, NULL, 3);
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayWithinDeltaActualNullAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_ARRAY_WITHIN_MESSAGE(0x6E00000000, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
#endif
}

void testHEX64ArrayWithinDeltaSamePointer(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};

    TEST_ASSERT_HEX64_ARRAY_WITHIN(0x6E00000000, expected, expected, 3);
#endif
}

void testHEX64ArrayWithinDeltaSamePointerAndMessage(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 expected[] = {0xABCD123400000000, 0xABCD112200000000, 0xABCD127700000000};

    TEST_ASSERT_HEX64_ARRAY_WITHIN_MESSAGE(0x6E00000000, expected, expected, 3, "Custom Message.");
#endif
}

void testHEX32ArrayWithinDelta(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};
    UNITY_UINT actualSmallDelta[] = {0xABCD1235, 0xABCD1121, 0xABCD1277};
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    TEST_ASSERT_HEX32_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_HEX32_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
}

void testHEX32ArrayWithinDeltaShouldNotHaveSignIssues(void)
{
    UNITY_UINT32 expected[] = {0x7FFFFFFF, 0x80000000};
    UNITY_UINT32 actualBigDelta[] = {0x80000000, 0x7FFFFFFF};

    TEST_ASSERT_HEX32_ARRAY_WITHIN(1, expected, actualBigDelta, 2);
}

void testHEX32ArrayWithinDeltaAndMessage(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};
    UNITY_UINT actualSmallDelta[] = {0xABCD1235, 0xABCD1121, 0xABCD1277};
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    TEST_ASSERT_HEX32_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_HEX32_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
}

void testHEX32ArrayNotWithinDelta(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testHEX32ArrayNotWithinDeltaAndMessage(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX32ArrayWithinDeltaPointless(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testHEX32ArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX32ArrayWithinDeltaExpectedNull(void)
{
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testHEX32ArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_UINT actualBigDelta[] = {0xABCD1267, 0xABCD1188, 0xABCD12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX32ArrayWithinDeltaActualNull(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testHEX32ArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX32ArrayWithinDeltaSamePointer(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};

    TEST_ASSERT_HEX32_ARRAY_WITHIN(110, expected, expected, 3);
}

void testHEX32ArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_UINT expected[] = {0xABCD1234, 0xABCD1122, 0xABCD1277};

    TEST_ASSERT_HEX32_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
}


void testHEX16ArrayWithinDelta(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};
    UNITY_UINT16 actualSmallDelta[] = {0x1235, 0x1121, 0x1277};
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    TEST_ASSERT_HEX16_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_HEX16_ARRAY_WITHIN(110, expected, actualBigDelta, 3);
}

void testHEX16ArrayWithinDeltaShouldNotHaveSignIssues(void)
{
    UNITY_UINT16 expected[] = {0x7FFF, 0x8000};
    UNITY_UINT16 actualBigDelta[] = {0x8000, 0x7FFF};

    TEST_ASSERT_HEX16_ARRAY_WITHIN(1, expected, actualBigDelta, 2);
}

void testHEX16ArrayWithinDeltaAndMessage(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};
    UNITY_UINT16 actualSmallDelta[] = {0x1235, 0x1121, 0x1277};
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    TEST_ASSERT_HEX16_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_HEX16_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
}

void testHEX16ArrayNotWithinDelta(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testHEX16ArrayNotWithinDeltaAndMessage(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX16ArrayWithinDeltaPointless(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN(110, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testHEX16ArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX16ArrayWithinDeltaExpectedNull(void)
{
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN(110, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testHEX16ArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_UINT16 actualBigDelta[] = {0x1267, 0x1188, 0x12AC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN_MESSAGE(110, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX16ArrayWithinDeltaActualNull(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN(110, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testHEX16ArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_ARRAY_WITHIN_MESSAGE(110, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX16ArrayWithinDeltaSamePointer(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};

    TEST_ASSERT_HEX16_ARRAY_WITHIN(110, expected, expected, 3);
}

void testHEX16ArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_UINT16 expected[] = {0x1234, 0x1122, 0x1277};

    TEST_ASSERT_HEX16_ARRAY_WITHIN_MESSAGE(110, expected, expected, 3, "Custom Message.");
}

void testHEX8ArrayWithinDelta(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};
    UNITY_UINT8 actualSmallDelta[] = {0x35, 0x21, 0x77};
    UNITY_UINT8 actualBigDelta[] = {0x47, 0x48, 0x4C};

    TEST_ASSERT_HEX8_ARRAY_WITHIN(1, expected, actualSmallDelta, 3);
    TEST_ASSERT_HEX8_ARRAY_WITHIN(60, expected, actualBigDelta, 3);
}

void testHEX8ArrayWithinDeltaAndMessage(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};
    UNITY_UINT8 actualSmallDelta[] = {0x35, 0x21, 0x77};
    UNITY_UINT8 actualBigDelta[] = {0x47, 0x48, 0x4C};

    TEST_ASSERT_HEX8_ARRAY_WITHIN_MESSAGE(1, expected, actualSmallDelta, 3, "Custom Message.");
    TEST_ASSERT_HEX8_ARRAY_WITHIN_MESSAGE(60, expected, actualBigDelta, 3, "Custom Message.");
}

void testHEX8ArrayNotWithinDelta(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};
    UNITY_UINT8 actualBigDelta[] = {0x67, 0x88, 0xAC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN(1, expected, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaShouldNotHaveSignIssues(void)
{
    UNITY_UINT8 expected[] = {0x7F, 0x80};
    UNITY_UINT8 actualBigDelta[] = {0x80, 0x7F};

    TEST_ASSERT_HEX8_ARRAY_WITHIN(1, expected, actualBigDelta, 2);
}

void testHEX8ArrayNotWithinDeltaAndMessage(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};
    UNITY_UINT8 actualBigDelta[] = {0x67, 0x88, 0xAC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN_MESSAGE(1, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaPointless(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};
    UNITY_UINT8 actualBigDelta[] = {0x67, 0x88, 0xAC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN(60, expected, actualBigDelta, 0);
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaPointlessAndMessage(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};
    UNITY_UINT8 actualBigDelta[] = {0x67, 0x88, 0xAC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN_MESSAGE(60, expected, actualBigDelta, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaExpectedNull(void)
{
    UNITY_UINT8 actualBigDelta[] = {0x67, 0x88, 0xAC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN(60, NULL, actualBigDelta, 3);
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaExpectedNullAndMessage(void)
{
    UNITY_UINT8 actualBigDelta[] = {0x67, 0x88, 0xAC};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN_MESSAGE(60, NULL, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaActualNull(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN(60, expected, NULL, 3);
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaActualNullAndMessage(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_ARRAY_WITHIN_MESSAGE(60, expected, NULL, 3, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX8ArrayWithinDeltaSamePointer(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};

    TEST_ASSERT_HEX8_ARRAY_WITHIN(60, expected, expected, 3);
}

void testHEX8ArrayWithinDeltaSamePointerAndMessage(void)
{
    UNITY_UINT8 expected[] = {0x34, 0x22, 0x77};

    TEST_ASSERT_HEX8_ARRAY_WITHIN_MESSAGE(60, expected, expected, 3, "Custom Message.");
}

void testEqualIntArrays(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    int p2[] = {1, 8, 987, 2};
    int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p3, 1);
    TEST_ASSERT_EQUAL_INT_ARRAY(NULL, NULL, 1);
}

void testNotEqualIntArraysNullExpected(void)
{
    int* p0 = NULL;
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArraysNullActual(void)
{
    int* p1 = NULL;
    int p0[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArrays1(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArrays2(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArrays3(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArraysLengthZero(void)
{
    UNITY_UINT32 p0[1] = {1};
    UNITY_UINT32 p1[1] = {1};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 0);
    VERIFY_FAILS_END
}

void testEqualIntEachEqual(void)
{
    int p0[] = {1, 1, 1, 1};
    int p1[] = {987, 987, 987, 987};
    int p2[] = {-2, -2, -2, -3};
    int p3[] = {1, 5, 600, 700};

    TEST_ASSERT_EACH_EQUAL_INT(1, p0, 1);
    TEST_ASSERT_EACH_EQUAL_INT(1, p0, 4);
    TEST_ASSERT_EACH_EQUAL_INT(987, p1, 4);
    TEST_ASSERT_EACH_EQUAL_INT(-2, p2, 3);
    TEST_ASSERT_EACH_EQUAL_INT(1, p3, 1);
}

void testNotEqualIntEachEqualNullActual(void)
{
    int* p1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(1, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntEachEqual1(void)
{
    int p0[] = {1, 1, 1, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(1, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntEachEqual2(void)
{
    int p0[] = {-5, -5, -1, -5};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(-5, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntEachEqual3(void)
{
    int p0[] = {1, 88, 88, 88};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(88, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualEachEqualLengthZero(void)
{
    UNITY_UINT32 p0[1] = {1};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(0, p0, 0);
    VERIFY_FAILS_END
}

void testEqualPtrArrays(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&A, &B, &C};
    char* p1[] = {&A, &B, &C, &A};
    char* p2[] = {&A, &B};
    char* p3[] = {&A};

    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p0, 3);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 3);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p1, p2, 2);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p3, p0, 1);
}

void testNotEqualPtrArraysNullExpected(void)
{
    char A = 1;
    char B = 2;
    char** p0 = NULL;
    char* p1[] = {&A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 2);
    VERIFY_FAILS_END
}

void testNotEqualPtrArraysNullActual(void)
{
    char A = 1;
    char B = 2;
    char** p0 = NULL;
    char* p1[] = {&A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p1, p0, 2);
    VERIFY_FAILS_END
}

void testNotEqualPtrArrays1(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&A, &B, &C, &B};
    char* p1[] = {&A, &B, &C, &A};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualPtrArrays2(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&B, &B, &C, &A};
    char* p1[] = {&A, &B, &C, &A};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualPtrArrays3(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&A, &B, &B, &A};
    char* p1[] = {&A, &B, &C, &A};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualPtrEachEqual(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&A, &A, &A};
    char* p1[] = {&A, &B, &C, &A};
    char* p2[] = {&B, &B};
    char* p3[] = {&C};

    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 1);
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 3);
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p1, 1);
    TEST_ASSERT_EACH_EQUAL_PTR(&B, p2, 2);
    TEST_ASSERT_EACH_EQUAL_PTR(&C, p3, 1);
}

void testNotEqualPtrEachEqualNullExpected(void)
{
    char A = 1;
    char B = 1;
    char* p0[] = {&A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 2);
    VERIFY_FAILS_END
}

void testNotEqualPtrEachEqualNullActual(void)
{
    char A = 1;
    char** p0 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 2);
    VERIFY_FAILS_END
}

void testNotEqualPtrEachEqual1(void)
{
    char A = 1;
    char B = 1;
    char* p0[] = {&A, &A, &A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualPtrEachEqual2(void)
{
    char A = 1;
    char B = 1;
    char* p0[] = {&B, &B, &A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&B, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualPtrEachEqual3(void)
{
    char A = 1;
    char B = 1;
    char* p0[] = {&A, &B, &B, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&B, p0, 4);
    VERIFY_FAILS_END
}

void testEqualInt8Arrays(void)
{
    UNITY_INT8 p0[] = {1, 8, 117, -2};
    UNITY_INT8 p1[] = {1, 8, 117, -2};
    UNITY_INT8 p2[] = {1, 8, 117, 2};
    UNITY_INT8 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p3, 1);
}

void testNotEqualInt8Arrays(void)
{
    UNITY_INT8 p0[] = {1, 8, 36, -2};
    UNITY_INT8 p1[] = {1, 8, 36, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualInt8EachEqual(void)
{
    UNITY_INT8 p0[] = {1, 1, 1, 1};
    UNITY_INT8 p1[] = {117, 117, 117, -2};
    UNITY_INT8 p2[] = {-1, -1, 117, 2};
    UNITY_INT8 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 1);
    TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 4);
    TEST_ASSERT_EACH_EQUAL_INT8(117, p1, 3);
    TEST_ASSERT_EACH_EQUAL_INT8(-1, p2, 2);
    TEST_ASSERT_EACH_EQUAL_INT8(1, p3, 1);
}

void testNotEqualInt8EachEqual(void)
{
    UNITY_INT8 p0[] = {1, 8, 36, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 2);
    VERIFY_FAILS_END
}

void testEqualCHARArrays(void)
{
    char p0[] = {1, 8, 117, -2};
    char p1[] = {1, 8, 117, -2};
    char p2[] = {1, 8, 117, 2};
    char p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_CHAR_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(p0, p3, 1);
}

void testNotEqualCHARArrays(void)
{
    char p0[] = {1, 8, 36, -2};
    char p1[] = {1, 8, 36, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_CHAR_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualCHAREachEqual(void)
{
    char p0[] = {1, 1, 1, 1};
    char p1[] = {117, 117, 117, -2};
    char p2[] = {-1, -1, 117, 2};
    char p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EACH_EQUAL_CHAR(1, p0, 1);
    TEST_ASSERT_EACH_EQUAL_CHAR(1, p0, 4);
    TEST_ASSERT_EACH_EQUAL_CHAR(117, p1, 3);
    TEST_ASSERT_EACH_EQUAL_CHAR(-1, p2, 2);
    TEST_ASSERT_EACH_EQUAL_CHAR(1, p3, 1);
}

void testNotEqualCHAREachEqual(void)
{
    char p0[] = {1, 8, 36, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_CHAR(1, p0, 2);
    VERIFY_FAILS_END
}

void testEqualUIntArrays(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65132u};
    unsigned int p2[] = {1, 8, 987, 2};
    unsigned int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p3, 1);
}

void testNotEqualUIntArrays1(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUIntArrays2(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUIntArrays3(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualUIntEachEqual(void)
{
    unsigned int p0[] = {1, 1, 1, 1};
    unsigned int p1[] = {65132u, 65132u, 65132u, 65132u};
    unsigned int p2[] = {8, 8, 987, 2};
    unsigned int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EACH_EQUAL_UINT(1, p0, 1);
    TEST_ASSERT_EACH_EQUAL_UINT(1, p0, 4);
    TEST_ASSERT_EACH_EQUAL_UINT(65132u, p1, 4);
    TEST_ASSERT_EACH_EQUAL_UINT(8, p2, 2);
    TEST_ASSERT_EACH_EQUAL_UINT(1, p3, 1);
}

void testNotEqualUIntEachEqual1(void)
{
    unsigned int p0[] = {1, 65132u, 65132u, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUIntEachEqual2(void)
{
    unsigned int p0[] = {987, 8, 987, 987};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT(987, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUIntEachEqual3(void)
{
    unsigned int p0[] = {1, 1, 1, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT(1, p0, 4);
    VERIFY_FAILS_END
}

void testEqualInt16Arrays(void)
{
    UNITY_INT16 p0[] = {1, 8, 117, 3};
    UNITY_INT16 p1[] = {1, 8, 117, 3};
    UNITY_INT16 p2[] = {1, 8, 117, 2};
    UNITY_INT16 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p3, 1);
}

void testNotEqualInt16Arrays(void)
{
    UNITY_INT16 p0[] = {1, 8, 127, 3};
    UNITY_INT16 p1[] = {1, 8, 127, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualInt16EachEqual(void)
{
    UNITY_INT16 p0[] = {1, 1, 1, 1};
    UNITY_INT16 p1[] = {32111, 32111, 32111, 3};
    UNITY_INT16 p2[] = {-1, -1, -1, 2};
    UNITY_INT16 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EACH_EQUAL_INT16(1, p0, 1);
    TEST_ASSERT_EACH_EQUAL_INT16(1, p0, 4);
    TEST_ASSERT_EACH_EQUAL_INT16(32111, p1, 3);
    TEST_ASSERT_EACH_EQUAL_INT16(-1, p2, 3);
    TEST_ASSERT_EACH_EQUAL_INT16(1, p3, 1);
}

void testNotEqualInt16EachEqual(void)
{
    UNITY_INT16 p0[] = {127, 127, 127, 3};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT16(127, p0, 4);
    VERIFY_FAILS_END
}

void testEqualInt32Arrays(void)
{
    UNITY_INT32 p0[] = {1, 8, 117, 3};
    UNITY_INT32 p1[] = {1, 8, 117, 3};
    UNITY_INT32 p2[] = {1, 8, 117, 2};
    UNITY_INT32 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p3, 1);
}

void testNotEqualInt32Arrays(void)
{
    UNITY_INT32 p0[] = {1, 8, 127, 3};
    UNITY_INT32 p1[] = {1, 8, 127, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualInt32EachEqual(void)
{
    UNITY_INT32 p0[] = {8, 8, 8, 8};
    UNITY_INT32 p1[] = {65537, 65537, 65537, 65537};
    UNITY_INT32 p2[] = {-3, -3, -3, 2};
    UNITY_INT32 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EACH_EQUAL_INT32(8, p0, 1);
    TEST_ASSERT_EACH_EQUAL_INT32(8, p0, 4);
    TEST_ASSERT_EACH_EQUAL_INT32(65537, p1, 4);
    TEST_ASSERT_EACH_EQUAL_INT32(-3, p2, 3);
    TEST_ASSERT_EACH_EQUAL_INT32(1, p3, 1);
}

void testNotEqualInt32EachEqual(void)
{
    UNITY_INT32 p0[] = {127, 8, 127, 127};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT32(127, p0, 4);
    VERIFY_FAILS_END
}

void testEqualUINT8Arrays(void)
{
    UNITY_UINT8 p0[] = {1, 8, 100, 127};
    UNITY_UINT8 p1[] = {1, 8, 100, 127};
    UNITY_UINT8 p2[] = {1, 8, 100, 2};
    UNITY_UINT8 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p3, 1);
}

void testNotEqualUINT8Arrays1(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT8Arrays2(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT8Arrays3(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}


void testEqualUINT16Arrays(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65132u};
    unsigned short p2[] = {1, 8, 987, 2};
    unsigned short p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p3, 1);
}

void testNotEqualUINT16Arrays1(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT16Arrays2(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT16Arrays3(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualUINT32Arrays(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p2[] = {1, 8, 987, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p3, 1);
}

void testNotEqualUINT32Arrays1(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT32Arrays2(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT32Arrays3(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualHEXArrays(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p2[] = {1, 8, 987, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p3, 1);
}

void testNotEqualHEXArrays1(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEXArrays2(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEXArrays3(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualHEX32Arrays(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p2[] = {1, 8, 987, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p3, 1);
}

void testNotEqualHEX32Arrays1(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX32Arrays2(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX32Arrays3(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualHEX16Arrays(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65132u};
    unsigned short p2[] = {1, 8, 987, 2};
    unsigned short p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p3, 1);
}

void testNotEqualHEX16Arrays1(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX16Arrays2(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX16Arrays3(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualHEX8Arrays(void)
{
    unsigned char p0[] = {1, 8, 254u, 123};
    unsigned char p1[] = {1, 8, 254u, 123};
    unsigned char p2[] = {1, 8, 254u, 2};
    unsigned char p3[] = {1, 23, 25, 26};

    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p3, 1);
}

void testNotEqualHEX8Arrays1(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 254u, 252u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX8Arrays2(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {2, 8, 254u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX8Arrays3(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 255u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualUINT8EachEqual(void)
{
    UNITY_UINT8 p0[] = {127u, 127u, 127u, 127u};
    UNITY_UINT8 p1[] = {1u, 1u, 1u, 1u};
    UNITY_UINT8 p2[] = {128u, 128u, 128u, 2u};
    UNITY_UINT8 p3[] = {1u, 50u, 60u, 70u};

    TEST_ASSERT_EACH_EQUAL_UINT8(127u, p0, 1);
    TEST_ASSERT_EACH_EQUAL_UINT8(127u, p0, 4);
    TEST_ASSERT_EACH_EQUAL_UINT8(1u, p1, 4);
    TEST_ASSERT_EACH_EQUAL_UINT8(128u, p2, 3);
    TEST_ASSERT_EACH_EQUAL_UINT8(1u, p3, 1);
}

void testNotEqualUINT8EachEqual1(void)
{
    unsigned char p0[] = {127u, 127u, 128u, 127u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(127u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT8EachEqual2(void)
{
    unsigned char p0[] = {1, 1, 1, 127u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(1, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT8EachEqual3(void)
{
    unsigned char p0[] = {54u, 55u, 55u, 55u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(55u, p0, 4);
    VERIFY_FAILS_END
}

void testEqualUINT16EachEqual(void)
{
    unsigned short p0[] = {65132u, 65132u, 65132u, 65132u};
    unsigned short p1[] = {987, 987, 987, 987};
    unsigned short p2[] = {1, 1, 1, 2};
    unsigned short p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 1);
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    TEST_ASSERT_EACH_EQUAL_UINT16(987, p1, 4);
    TEST_ASSERT_EACH_EQUAL_UINT16(1, p2, 3);
    TEST_ASSERT_EACH_EQUAL_UINT16(1, p3, 1);
}

void testNotEqualUINT16EachEqual1(void)
{
    unsigned short p0[] = {1, 65132u, 65132u, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT16EachEqual2(void)
{
    unsigned short p0[] = {65132u, 65132u, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT16EachEqual3(void)
{
    unsigned short p0[] = {65132u, 65132u, 65132u, 65133u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testEqualUINT32EachEqual(void)
{
    UNITY_UINT32 p0[] = {65132u, 65132u, 65132u, 65132u};
    UNITY_UINT32 p1[] = {987, 987, 987, 987};
    UNITY_UINT32 p2[] = {8, 8, 8, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EACH_EQUAL_UINT32(65132u, p0, 1);
    TEST_ASSERT_EACH_EQUAL_UINT32(65132u, p0, 4);
    TEST_ASSERT_EACH_EQUAL_UINT32(987, p1, 4);
    TEST_ASSERT_EACH_EQUAL_UINT32(8, p2, 3);
    TEST_ASSERT_EACH_EQUAL_UINT32(1, p3, 1);
}

void testNotEqualUINT32EachEqual1(void)
{
    UNITY_UINT32 p0[] = {65132u, 65132u, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT32(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT32EachEqual2(void)
{
    UNITY_UINT32 p0[] = {1, 987, 987, 987};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT32(987, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT32EachEqual3(void)
{
    UNITY_UINT32 p0[] = {1, 1, 1, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT32(1, p0, 4);
    VERIFY_FAILS_END
}

void testEqualHEXEachEqual(void)
{
    UNITY_UINT32 p0[] = {65132u, 65132u, 65132u, 65132u};
    UNITY_UINT32 p1[] = {987, 987, 987, 987};
    UNITY_UINT32 p2[] = {8, 8, 8, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EACH_EQUAL_HEX(65132u, p0, 1);
    TEST_ASSERT_EACH_EQUAL_HEX(65132u, p0, 4);
    TEST_ASSERT_EACH_EQUAL_HEX(987, p1, 4);
    TEST_ASSERT_EACH_EQUAL_HEX(8, p2, 3);
    TEST_ASSERT_EACH_EQUAL_HEX(1, p3, 1);
}

void testNotEqualHEXEachEqual1(void)
{
    UNITY_UINT32 p0[] = {1, 65132u, 65132u, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEXEachEqual2(void)
{
    UNITY_UINT32 p0[] = {987, 987, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEXEachEqual3(void)
{
    UNITY_UINT32 p0[] = {8, 8, 987, 8};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX(8, p0, 4);
    VERIFY_FAILS_END
}

void testEqualHEX32EachEqual(void)
{
    UNITY_UINT32 p0[] = {65132u, 65132u, 65132u, 65132u};
    UNITY_UINT32 p1[] = {987, 987, 987, 987};
    UNITY_UINT32 p2[] = {8, 8, 8, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 1);
    TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
    TEST_ASSERT_EACH_EQUAL_HEX32(987, p1, 4);
    TEST_ASSERT_EACH_EQUAL_HEX32(8, p2, 3);
    TEST_ASSERT_EACH_EQUAL_HEX32(1, p3, 1);
}

void testNotEqualHEX32EachEqual1(void)
{
    UNITY_UINT32 p0[] = {65132u, 8, 65132u, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX32EachEqual2(void)
{
    UNITY_UINT32 p0[] = {1, 987, 987, 987};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX32EachEqual3(void)
{
    UNITY_UINT32 p0[] = {8, 8, 8, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(8, p0, 4);
    VERIFY_FAILS_END
}

void testEqualHEX16EachEqual(void)
{
    UNITY_UINT16 p0[] = {65132u, 65132u, 65132u, 65132u};
    UNITY_UINT16 p1[] = {987, 987, 987, 987};
    UNITY_UINT16 p2[] = {8, 8, 8, 2};
    UNITY_UINT16 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EACH_EQUAL_HEX16(65132u, p0, 1);
    TEST_ASSERT_EACH_EQUAL_HEX16(65132u, p0, 4);
    TEST_ASSERT_EACH_EQUAL_HEX16(987, p1, 4);
    TEST_ASSERT_EACH_EQUAL_HEX16(8, p2, 3);
    TEST_ASSERT_EACH_EQUAL_HEX16(1, p3, 1);
}

void testNotEqualHEX16EachEqual1(void)
{
    unsigned short p0[] = {65132u, 65132u, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX16(65132u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX16EachEqual2(void)
{
    unsigned short p0[] = {1, 987, 987, 987};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX16(987, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX16EachEqual3(void)
{
    unsigned short p0[] = {8, 8, 8, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX16(8, p0, 4);
    VERIFY_FAILS_END
}

void testEqualHEX8EachEqual(void)
{
    unsigned char p0[] = {254u, 254u, 254u, 254u};
    unsigned char p1[] = {123, 123, 123, 123};
    unsigned char p2[] = {8, 8, 8, 2};
    unsigned char p3[] = {1, 23, 25, 26};

    TEST_ASSERT_EACH_EQUAL_HEX8(254u, p0, 1);
    TEST_ASSERT_EACH_EQUAL_HEX8(254u, p0, 4);
    TEST_ASSERT_EACH_EQUAL_HEX8(123, p1, 4);
    TEST_ASSERT_EACH_EQUAL_HEX8(8, p2, 3);
    TEST_ASSERT_EACH_EQUAL_HEX8(1, p3, 1);
}

void testNotEqualHEX8EachEqual1(void)
{
    unsigned char p0[] = {253u, 253u, 254u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(253u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX8EachEqual2(void)
{
    unsigned char p0[] = {254u, 254u, 254u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(254u, p0, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX8EachEqual3(void)
{
    unsigned char p0[] = {1, 8, 8, 8};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(8, p0, 4);
    VERIFY_FAILS_END
}

void testEqualHEX64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT64 p1[] = {1, 8, 987, 65132u};
    UNITY_UINT64 p2[] = {1, 8, 987, 2};
    UNITY_UINT64 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p3, 1);
#endif
}

void testEqualUint64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT64 p1[] = {1, 8, 987, 65132u};
    UNITY_UINT64 p2[] = {1, 8, 987, 2};
    UNITY_UINT64 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p3, 1);
#endif
}

void testEqualInt64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 p0[] = {1, 8, 987, -65132};
    UNITY_INT64 p1[] = {1, 8, 987, -65132};
    UNITY_INT64 p2[] = {1, 8, 987, -2};
    UNITY_INT64 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p3, 1);
#endif
}


void testNotEqualHEX64Arrays1(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT64 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualHEX64Arrays2(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT64 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualUint64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT64 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualInt64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 p0[] = {1, 8, 987, -65132};
    UNITY_INT64 p1[] = {1, 8, 987, -65131};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testVerifyIntPassingPointerComparisonOnZeroLengthArray(void)
{
    int a[] = { 1 };

#ifndef UNITY_COMPARE_PTRS_ON_ZERO_ARRAY
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(a, a, 0);
    VERIFY_FAILS_END
#else

    TEST_ASSERT_EQUAL_INT_ARRAY(a, a, 0);
#endif
}

void testVerifyIntFailingPointerComparisonOnZeroLengthArray(void)
{
    int a[] = { 1 };
    int b[] = { 1 };

#ifndef UNITY_COMPARE_PTRS_ON_ZERO_ARRAY
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(a, b, 0);
    VERIFY_FAILS_END
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(a, b, 0);
    VERIFY_FAILS_END
#endif
}
