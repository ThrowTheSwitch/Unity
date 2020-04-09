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

void testUnitySizeInitializationReminder(void)
{
    /* This test ensures that sizeof(struct UNITY_STORAGE_T) doesn't change. If this
     * test breaks, go look at the initialization of the Unity global variable
     * in unity.c and make sure we're filling in the proper fields. */
    const char* message = "Unexpected size for UNITY_STORAGE_T struct. Please check that "
                     "the initialization of the Unity symbol in unity.c is "
                     "still correct.";

    /* Define a structure with all the same fields as `struct UNITY_STORAGE_T`. */
#ifdef UNITY_EXCLUDE_DETAILS
    struct {
        const char* TestFile;
        const char* CurrentTestName;
        UNITY_LINE_TYPE CurrentTestLineNumber;
        UNITY_COUNTER_TYPE NumberOfTests;
        UNITY_COUNTER_TYPE TestFailures;
        UNITY_COUNTER_TYPE TestIgnores;
        UNITY_COUNTER_TYPE CurrentTestFailed;
        UNITY_COUNTER_TYPE CurrentTestIgnored;
#ifdef UNITY_INCLUDE_EXEC_TIME
        UNITY_TIME_TYPE CurrentTestStartTime;
        UNITY_TIME_TYPE CurrentTestStopTime;
#endif
#ifndef UNITY_EXCLUDE_SETJMP_H
        jmp_buf AbortFrame;
#endif
    } _Expected_Unity;
#else
    struct {
        const char* TestFile;
        const char* CurrentTestName;
        const char* CurrentDetails1;
        const char* CurrentDetails2;
        UNITY_LINE_TYPE CurrentTestLineNumber;
        UNITY_COUNTER_TYPE NumberOfTests;
        UNITY_COUNTER_TYPE TestFailures;
        UNITY_COUNTER_TYPE TestIgnores;
        UNITY_COUNTER_TYPE CurrentTestFailed;
        UNITY_COUNTER_TYPE CurrentTestIgnored;
#ifdef UNITY_INCLUDE_EXEC_TIME
        UNITY_COUNTER_TYPE CurrentTestStartTime;
        UNITY_COUNTER_TYPE CurrentTestStopTime;
#endif
#ifndef UNITY_EXCLUDE_SETJMP_H
        jmp_buf AbortFrame;
#endif
    } _Expected_Unity;
#endif

    /* Compare our fake structure's size to the actual structure's size. They
     * should be the same.
     *
     * This accounts for alignment, padding, and packing issues that might come
     * up between different architectures. */
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(_Expected_Unity), sizeof(Unity), message);
}

void testPassShouldEndImmediatelyWithPass(void)
{
    TEST_PASS();
    TEST_FAIL_MESSAGE("We should have passed already and finished this test");
}

void testPassShouldEndImmediatelyWithPassAndMessage(void)
{
    TEST_PASS_MESSAGE("Woohoo! This Automatically Passes!");
    TEST_FAIL_MESSAGE("We should have passed already and finished this test");
}

void testMessageShouldDisplayMessageWithoutEndingAndGoOnToPass(void)
{
    TEST_MESSAGE("This is just a message");
    TEST_MESSAGE("This is another message");
    TEST_PASS();
}

void testMessageShouldDisplayMessageWithoutEndingAndGoOnToFail(void)
{
    TEST_MESSAGE("This is yet another message");

    EXPECT_ABORT_BEGIN
    TEST_FAIL();
    VERIFY_FAILS_END
}

void testTrue(void)
{
    TEST_ASSERT(1);

    TEST_ASSERT_TRUE(1);
}

void testFalse(void)
{
    TEST_ASSERT_FALSE(0);

    TEST_ASSERT_UNLESS(0);
}

void testSingleStatement(void)
{
    for(int i = 0; i < 2; i++)
    {
        /* TEST_ASSERT_TRUE should expand to a single C statement, minus
         * the semicolon. This if-else will fail to compile otherwise. */
        if(i > 0)
            TEST_ASSERT_TRUE(i);
        else
            TEST_ASSERT_FALSE(i);
    }
}

void testPreviousPass(void)
{
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void testNotVanilla(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT(0);
    VERIFY_FAILS_END
}

void testNotTrue(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(0);
    VERIFY_FAILS_END
}

void testNotFalse(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FALSE(1);
    VERIFY_FAILS_END
}

void testNotUnless(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UNLESS(1);
    VERIFY_FAILS_END
}

void testNotNotEqual(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL(10, 10);
    VERIFY_FAILS_END
}

void testFail(void)
{
    EXPECT_ABORT_BEGIN
    TEST_FAIL_MESSAGE("Expected for testing");
    VERIFY_FAILS_END
}

void testIsNull(void)
{
    char* ptr1 = NULL;
    const char* ptr2 = "hello";

    TEST_ASSERT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
}

void testIsNullShouldFailIfNot(void)
{
    const char* ptr1 = "hello";

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NULL(ptr1);
    VERIFY_FAILS_END
}

void testNotNullShouldFailIfNULL(void)
{
    char* ptr1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_NULL(ptr1);
    VERIFY_FAILS_END
}

void testIsEmpty(void)
{
    const char* ptr1 = "\0";
    const char* ptr2 = "hello";

    TEST_ASSERT_EMPTY(ptr1);
    TEST_ASSERT_NOT_EMPTY(ptr2);
}

void testIsEmptyShouldFailIfNot(void)
{
    const char* ptr1 = "hello";

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EMPTY(ptr1);
    VERIFY_FAILS_END
}

void testNotEmptyShouldFailIfEmpty(void)
{
    const char* ptr1 = "\0";

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EMPTY(ptr1);
    VERIFY_FAILS_END
}

void testIgnore(void)
{
    EXPECT_ABORT_BEGIN
    TEST_IGNORE();
    TEST_FAIL_MESSAGE("This should not be reached");
    VERIFY_IGNORES_END
}

void testIgnoreMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_IGNORE_MESSAGE("This is an expected TEST_IGNORE_MESSAGE string!");
    TEST_FAIL_MESSAGE("This should not be reached");
    VERIFY_IGNORES_END
}

void testProtection(void)
{
    volatile int mask = 0;

    if (TEST_PROTECT())
    {
        mask |= 1;
        TEST_ABORT();
    }
    else
    {
        Unity.CurrentTestFailed = 0;
        mask |= 2;
    }

    TEST_ASSERT_EQUAL(3, mask);
}

void testIgnoredAndThenFailInTearDown(void)
{
    SetToOneToFailInTearDown = 1;
    TEST_IGNORE();
}

void testFailureCountIncrementsAndIsReturnedAtEnd(void)
{
#ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
#else
    UNITY_UINT savedFailures = Unity.TestFailures;
    Unity.CurrentTestFailed = 1;
    startPutcharSpy(); /* Suppress output */
    startFlushSpy();
    TEST_ASSERT_EQUAL(0, getFlushSpyCalls());
    UnityConcludeTest();
    endPutcharSpy();
    TEST_ASSERT_EQUAL(savedFailures + 1, Unity.TestFailures);
#if defined(UNITY_OUTPUT_FLUSH) && defined(UNITY_OUTPUT_FLUSH_HEADER_DECLARATION)
    TEST_ASSERT_EQUAL(1, getFlushSpyCalls());
#else
    TEST_ASSERT_EQUAL(0, getFlushSpyCalls());
#endif
    endFlushSpy();

    startPutcharSpy(); /* Suppress output */
    int failures = UnityEnd();
    Unity.TestFailures--;
    endPutcharSpy();
    TEST_ASSERT_EQUAL(savedFailures + 1, failures);
#endif
}

/* ===================== THESE TEST WILL RUN IF YOUR CONFIG INCLUDES DETAIL SUPPORT ================== */

void testThatDetailsCanBeHandleOneDetail(void)
{
#ifdef UNITY_EXCLUDE_DETAILS
    TEST_IGNORE();
#else
    UNITY_SET_DETAIL("Detail1");

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, 6, "Should Fail And Say Detail1");
    VERIFY_FAILS_END
#endif
}

void testThatDetailsCanHandleTestFail(void)
{
#ifdef UNITY_EXCLUDE_DETAILS
    TEST_IGNORE();
#else
    UNITY_SET_DETAILS("Detail1","Detail2");

    EXPECT_ABORT_BEGIN
    TEST_FAIL_MESSAGE("Should Fail And Say Detail1 and Detail2");
    VERIFY_FAILS_END
#endif
}

void testThatDetailsCanBeHandleTwoDetails(void)
{
#ifdef UNITY_EXCLUDE_DETAILS
    TEST_IGNORE();
#else
    UNITY_SET_DETAILS("Detail1","Detail2");

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(7, 8, "Should Fail And Say Detail1 and Detail2");
    VERIFY_FAILS_END
#endif
}

void testThatDetailsCanBeHandleSingleDetailClearingTwoDetails(void)
{
#ifdef UNITY_EXCLUDE_DETAILS
    TEST_IGNORE();
#else
    UNITY_SET_DETAILS("Detail1","Detail2");
    UNITY_SET_DETAIL("DetailNew");

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_MESSAGE("MEH", "GUH", "Should Fail And Say DetailNew");
    VERIFY_FAILS_END
#endif
}
