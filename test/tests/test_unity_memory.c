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
