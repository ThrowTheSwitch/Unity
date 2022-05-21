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

void testEqualHex64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 v0, v1;
    UNITY_UINT64 *p0, *p1;

    v0 = 0x9876543201234567;
    v1 = 0x9876543201234567;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_HEX64(0x9876543201234567, 0x9876543201234567);
    TEST_ASSERT_EQUAL_HEX64(v0, v1);
    TEST_ASSERT_EQUAL_HEX64(0x9876543201234567, v1);
    TEST_ASSERT_EQUAL_HEX64(v0, 0x9876543201234567);
    TEST_ASSERT_EQUAL_HEX64(*p0, v1);
    TEST_ASSERT_EQUAL_HEX64(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX64(*p0, 0x9876543201234567);
#endif
}

void testEqualUint64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 v0, v1;
    UNITY_UINT64 *p0, *p1;

    v0 = 0x9876543201234567;
    v1 = 0x9876543201234567;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_UINT64(0x9876543201234567, 0x9876543201234567);
    TEST_ASSERT_EQUAL_UINT64(v0, v1);
    TEST_ASSERT_EQUAL_UINT64(0x9876543201234567, v1);
    TEST_ASSERT_EQUAL_UINT64(v0, 0x9876543201234567);
    TEST_ASSERT_EQUAL_UINT64(*p0, v1);
    TEST_ASSERT_EQUAL_UINT64(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT64(*p0, 0x9876543201234567);
#endif
}

void testEqualInt64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 v0, v1;
    UNITY_INT64 *p0, *p1;

    v0 = (UNITY_INT64)0x9876543201234567;
    v1 = (UNITY_INT64)0x9876543201234567;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT64(0x9876543201234567, 0x9876543201234567);
    TEST_ASSERT_EQUAL_INT64(v0, v1);
    TEST_ASSERT_EQUAL_INT64(0x9876543201234567, v1);
    TEST_ASSERT_EQUAL_INT64(v0, 0x9876543201234567);
    TEST_ASSERT_EQUAL_INT64(*p0, v1);
    TEST_ASSERT_EQUAL_INT64(*p0, *p1);
    TEST_ASSERT_EQUAL_INT64(*p0, 0x9876543201234567);
#endif
}


void testNotEqualHex64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 v0, v1;

    v0 = 9000000000;
    v1 = 9100000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testNotEqualUint64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_UINT64 v0, v1;

    v0 = 9000000000;
    v1 = 9100000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testNotEqualInt64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 v0, v1;

    v0 = -9000000000;
    v1 = 9100000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testNotEqualHex64sIfSigned(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    UNITY_INT64 v0, v1;

    v0 = -9000000000;
    v1 = 9000000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testHEX64sWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    TEST_ASSERT_HEX64_WITHIN(1, 0x7FFFFFFFFFFFFFFF,0x7FFFFFFFFFFFFFFE);
    TEST_ASSERT_HEX64_WITHIN(5, 5000, 4996);
    TEST_ASSERT_HEX64_WITHIN(5, 5000, 5005);
#endif
}

void testHEX32sWithinDeltaShouldIgnoreSign(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    TEST_ASSERT_HEX64_WITHIN(1, 0x7FFFFFFFFFFFFFFF,0x8000000000000000);
#endif
}

void testHEX64sNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_WITHIN(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC);
    VERIFY_FAILS_END
#endif
}

void testHEX64sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
#endif
}

void testUINT64sWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    TEST_ASSERT_UINT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF,0x7FFFFFFFFFFFFFFE);
    TEST_ASSERT_UINT64_WITHIN(5, 5000, 4996);
    TEST_ASSERT_UINT64_WITHIN(5, 5000, 5005);
#endif
}

void testUINT64sNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC);
    VERIFY_FAILS_END
#endif
}

void testUINT64sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
#endif
}

void testINT64sWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    TEST_ASSERT_INT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF,0x7FFFFFFFFFFFFFFE);
    TEST_ASSERT_INT64_WITHIN(5, 5000, 4996);
    TEST_ASSERT_INT64_WITHIN(5, 5000, 5005);
#endif
}

void testINT64sNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC);
    VERIFY_FAILS_END
#endif
}

void testINT64sNotWithinDeltaAndDifferenceOverflows(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_WITHIN(1, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF);
    VERIFY_FAILS_END
#endif
}

void testPrintNumbersInt64(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
  #ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
  #else
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("0", 0);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("10000000000", 10000000000);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-9223372036854775808", (UNITY_INT)0x8000000000000000);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-1", (UNITY_INT)0xFFFFFFFFFFFFFFFF);
  #endif
#endif
}

void testPrintNumbersUInt64(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
  #ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
  #else
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("0", 0);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("70000000000", 70000000000);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("9223372036854775808",  (UNITY_UINT)0x8000000000000000);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("18446744073709551615", (UNITY_UINT)0xFFFFFFFFFFFFFFFF);
  #endif
#endif
}
