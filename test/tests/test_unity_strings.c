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

void testEqualStrings(void)
{
    const char *testString = "foo";

    TEST_ASSERT_EQUAL_STRING(testString, testString);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("foo", "foo", "foo isn't foo");
    TEST_ASSERT_EQUAL_STRING("foo", testString);
    TEST_ASSERT_EQUAL_STRING(testString, "foo");
    TEST_ASSERT_EQUAL_STRING("", "");
}

void testEqualStringsLen(void)
{
    const char *testString = "foobar";
    TEST_ASSERT_EQUAL_STRING_LEN(testString, testString, strlen(testString));
    TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE("foobar", "foobaz", 5, "fooba isn't fooba");
    TEST_ASSERT_EQUAL_STRING_LEN("foo", testString, 3);
    TEST_ASSERT_EQUAL_STRING_LEN(testString, "foo", 3);
    TEST_ASSERT_EQUAL_STRING_LEN("", "", 3);
}

void testEqualStringsWithCarriageReturnsAndLineFeeds(void)
{
    const char *testString = "foo\r\nbar";

    TEST_ASSERT_EQUAL_STRING(testString, testString);
    TEST_ASSERT_EQUAL_STRING("foo\r\nbar", "foo\r\nbar");
    TEST_ASSERT_EQUAL_STRING("foo\r\nbar", testString);
    TEST_ASSERT_EQUAL_STRING(testString, "foo\r\nbar");
    TEST_ASSERT_EQUAL_STRING("", "");
}

void testNotEqualString1(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "bar");
    VERIFY_FAILS_END
}

void testNotEqualStringLen1(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foobar", "foobaz", 6);
    VERIFY_FAILS_END
}

void testNotEqualString2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "");
    VERIFY_FAILS_END
}

void testNotEqualStringLen2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foo", "", 3);
    VERIFY_FAILS_END
}

void testNotEqualString3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("", "bar");
    VERIFY_FAILS_END
}

void testNotEqualStringLen3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("", "bar", 3);
    VERIFY_FAILS_END
}

void testNotEqualString4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("bar\r", "bar\n");
    VERIFY_FAILS_END
}

void testNotEqualStringLen4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("ba\r\x16", "ba\r\n", 4);
    VERIFY_FAILS_END
}

void testNotEqualString5(void)
{
    const char str1[] = { 0x41, 0x42, 0x03, 0x00 };
    const char str2[] = { 0x41, 0x42, 0x04, 0x00 };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(str1, str2);
    VERIFY_FAILS_END
}

void testNotEqualString_ExpectedStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(NULL, "bar");
    VERIFY_FAILS_END
}

void testNotEqualStringLen_ExpectedStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN(NULL, "bar", 1);
    VERIFY_FAILS_END
}

void testNotEqualString_ActualStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", NULL);
    VERIFY_FAILS_END
}

void testNotEqualStringLen_ActualStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foo", NULL, 1);
    VERIFY_FAILS_END
}

void testNotEqualString_ExpectedStringIsLonger(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo2", "foo");
    VERIFY_FAILS_END
}

void testNotEqualString_ActualStringIsLonger(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "foo2");
    VERIFY_FAILS_END
}

void testEqualStringArrays(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, expStrings, 3);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 3);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 2);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 1);
}

void testNotEqualStringArray1(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray2(void)
{
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", "boo", "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray3(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", NULL };
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray4(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", NULL, "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray5(void)
{
    const char **testStrings = NULL;
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray6(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char **expStrings = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testEqualStringArrayIfBothNulls(void)
{
    const char **testStrings = NULL;
    const char **expStrings = NULL;

    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
}

void testNotEqualStringArrayLengthZero(void)
{
    const char *testStrings[] = {NULL};
    const char **expStrings = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 0);
    VERIFY_FAILS_END
}

void testEqualStringEachEqual(void)
{
    const char *testStrings1[] = { "foo", "foo", "foo", "foo" };
    const char *testStrings2[] = { "boo", "boo", "boo", "zoo" };
    const char *testStrings3[] = { "", "", "", "" };

    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings1, 4);
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings1, 1);
    TEST_ASSERT_EACH_EQUAL_STRING("boo", testStrings2, 3);
    TEST_ASSERT_EACH_EQUAL_STRING("", testStrings3, 4);
}

void testNotEqualStringEachEqual1(void)
{
    const char *testStrings[] = { "foo", "foo", "foo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringEachEqual2(void)
{
    const char *testStrings[] = { "boo", "foo", "foo", "foo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringEachEqual3(void)
{
    const char *testStrings[] = { "foo", "foo", "foo", NULL };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringEachEqual4(void)
{
    const char *testStrings[] = { "foo", "foo", "woo", "foo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringEachEqual5(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", NULL, 1);
    VERIFY_FAILS_END
}

void testCstringsEscapeSequence(void)
{
#ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
#else
    startPutcharSpy();
    UnityPrint("\x16\x10");
    endPutcharSpy();
    TEST_ASSERT_EQUAL_STRING("\\x16\\x10", getBufferPutcharSpy());
#endif
}
