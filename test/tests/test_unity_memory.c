/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

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

void testEqualMemory(void)
{
    const char *testString = "whatever";

    TEST_ASSERT_EQUAL_MEMORY(testString, testString, 8);
    TEST_ASSERT_EQUAL_MEMORY("whatever", "whatever", 8);
    TEST_ASSERT_EQUAL_MEMORY("whatever", testString, 8);
    TEST_ASSERT_EQUAL_MEMORY(testString, "whatever", 8);
    TEST_ASSERT_EQUAL_MEMORY(testString, "whatever", 2);
    TEST_ASSERT_EQUAL_MEMORY(NULL, NULL, 1);
}

void testNotEqualMemory1(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("foo", "bar", 3);
    VERIFY_FAILS_END
}

void testNotEqualMemory2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", "food", 4);
    VERIFY_FAILS_END
}

void testNotEqualMemory3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY(NULL, "food", 4);
    VERIFY_FAILS_END
}

void testNotEqualMemory4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", NULL, 4);
    VERIFY_FAILS_END
}

void testNotEqualMemoryLengthZero(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY(NULL, NULL, 0);
    VERIFY_FAILS_END
}

void testNotEqualMemory(void)
{
    TEST_ASSERT_NOT_EQUAL_MEMORY("foo", "bar", 3);
    TEST_ASSERT_NOT_EQUAL_MEMORY_MESSAGE("fool", "food", 4, "fool should not equal food");
    TEST_ASSERT_NOT_EQUAL_MEMORY(NULL, "food", 4);
    TEST_ASSERT_NOT_EQUAL_MEMORY("fool", NULL, 4);
}

void testNotNotEqualMemory1(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_MEMORY("foo", "foo", 3);
    VERIFY_FAILS_END
}

void testNotNotEqualMemory2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_MEMORY(NULL, NULL, 1);
    VERIFY_FAILS_END
}

void testNotNotEqualMemoryLengthZero(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_MEMORY("foo", "bar", 0);
    VERIFY_FAILS_END
}

void testEachEqualMemory(void)
{
    unsigned char p0[] = {0xAB, 0xAB, 0xAB, 0xAB};
    unsigned char p1[] = {0x12, 0x12, 0x12, 0xFF};
    unsigned char p2[] = {0x00, 0x00, 0xFF, 0xFF};
    unsigned char p3[] = {0x5A, 0x01, 0x02, 0x03};

    TEST_ASSERT_EACH_EQUAL_MEMORY(0xAB, p0, 1, 1);
    TEST_ASSERT_EACH_EQUAL_MEMORY(0xAB, p0, 1, 4);
    TEST_ASSERT_EACH_EQUAL_MEMORY(0x12, p1, 1, 3);
    TEST_ASSERT_EACH_EQUAL_MEMORY(0x00, p2, 1, 2);
    TEST_ASSERT_EACH_EQUAL_MEMORY(0x5A, p3, 1, 1);
}

void testNotEachEqualMemory(void)
{
    unsigned char p0[] = {0xAB, 0xAB, 0x00, 0xAB};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(0xAB, p0, 1, 4);
    VERIFY_FAILS_END
}

void testEachEqualMemoryWithMessage(void)
{
    unsigned char p0[] = {0x42, 0x42, 0x42};

    TEST_ASSERT_EACH_EQUAL_MEMORY_MESSAGE(0x42, p0, 1, 3, "all bytes should be 0x42");
}

void testNotEachEqualMemoryWithMessage(void)
{
    unsigned char p0[] = {0x42, 0x42, 0x01};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY_MESSAGE(0x42, p0, 1, 3, "mismatch expected");
    VERIFY_FAILS_END
}
