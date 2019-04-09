/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include <setjmp.h>
#include <stdio.h>
#include "unity.h"

/* Support for Meta Test Rig */
#define TEST_CASE(...)

/* Include Passthroughs for Linking Tests */
void putcharSpy(int c) { (void)putchar(c);}
void flushSpy(void) {}

#define EXPECT_ABORT_BEGIN \
    if (TEST_PROTECT())    \
    {

#define VERIFY_FAILS_END                                                       \
    }                                                                          \
    Unity.CurrentTestFailed = (Unity.CurrentTestFailed != 0) ? 0 : 1;          \
    if (Unity.CurrentTestFailed == 1) {                                        \
      SetToOneMeanWeAlreadyCheckedThisGuy = 1;                                 \
      UnityPrintNumberUnsigned(Unity.CurrentTestLineNumber);                   \
      UNITY_OUTPUT_CHAR(':');                                                  \
      UnityPrint(Unity.CurrentTestName);                                       \
      UnityPrint(":FAIL: [[[[ Test Should Have Failed But Did Not ]]]]");      \
      UNITY_OUTPUT_CHAR('\n');                                                 \
    }

#define VERIFY_IGNORES_END                                                     \
    }                                                                          \
    Unity.CurrentTestFailed = (Unity.CurrentTestIgnored != 0) ? 0 : 1;         \
    Unity.CurrentTestIgnored = 0;                                              \
    if (Unity.CurrentTestFailed == 1) {                                        \
      SetToOneMeanWeAlreadyCheckedThisGuy = 1;                                 \
      UnityPrintNumberUnsigned(Unity.CurrentTestLineNumber);                   \
      UNITY_OUTPUT_CHAR(':');                                                  \
      UnityPrint(Unity.CurrentTestName);                                       \
      UnityPrint(":FAIL: [[[[ Test Should Have Ignored But Did Not ]]]]");     \
      UNITY_OUTPUT_CHAR('\n');                                                 \
    }

int SetToOneToFailInTearDown;
int SetToOneMeanWeAlreadyCheckedThisGuy;
static unsigned NextExpectedStringIndex;
static unsigned NextExpectedCharIndex;

void suiteSetUp(void)
{
    NextExpectedStringIndex = 0;
    NextExpectedCharIndex = 0;
}

void setUp(void)
{
  SetToOneToFailInTearDown = 0;
  SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}

void tearDown(void)
{
  if (SetToOneToFailInTearDown == 1)
    TEST_FAIL_MESSAGE("<= Failed in tearDown");
  if ((SetToOneMeanWeAlreadyCheckedThisGuy == 0) && (Unity.CurrentTestFailed > 0))
  {
    UnityPrint(": [[[[ Test Should Have Passed But Did Not ]]]]");
    UNITY_OUTPUT_CHAR('\n');
  }
}

TEST_CASE(0)
TEST_CASE(44)
TEST_CASE((90)+9)
void test_TheseShouldAllPass(int Num)
{
    TEST_ASSERT_TRUE(Num < 100);
}

TEST_CASE(3)
TEST_CASE(77)
TEST_CASE( (99) + 1 - (1))
void test_TheseShouldAllFail(int Num)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(Num > 100);
    VERIFY_FAILS_END
}

TEST_CASE(1)
TEST_CASE(44)
TEST_CASE(99)
TEST_CASE(98)
void test_TheseAreEveryOther(int Num)
{
    if (Num & 1)
    {
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_TRUE(Num > 100);
        VERIFY_FAILS_END
    }
    else
    {
        TEST_ASSERT_TRUE(Num < 100);
    }
}

void test_NormalPassesStillWork(void)
{
    TEST_ASSERT_TRUE(1);
}

void test_NormalFailsStillWork(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(0);
    VERIFY_FAILS_END
}

TEST_CASE(0, "abc")
TEST_CASE(1, "{")
TEST_CASE(2, "}")
TEST_CASE(3, ";")
TEST_CASE(4, "\"quoted\"")
void test_StringsArePreserved(unsigned index, const char * str)
{
    static const char * const expected[] = 
    {
        "abc",
        "{",
        "}",
        ";",
        "\"quoted\""
    };

    /* Ensure that no test cases are skipped by tracking the next expected index */
    TEST_ASSERT_EQUAL_UINT32(NextExpectedStringIndex, index);
    TEST_ASSERT_LESS_THAN(sizeof(expected) / sizeof(expected[0]), index);
    TEST_ASSERT_EQUAL_STRING(expected[index], str);

    NextExpectedStringIndex++;
}

TEST_CASE(0, 'x')
TEST_CASE(1, '{')
TEST_CASE(2, '}')
TEST_CASE(3, ';')
TEST_CASE(4, '\'')
TEST_CASE(5, '"')
void test_CharsArePreserved(unsigned index, char c)
{
    static const char expected[] =
    {
        'x',
        '{',
        '}',
        ';',
        '\'',
        '"'
    };

    /* Ensure that no test cases are skipped by tracking the next expected index */
    TEST_ASSERT_EQUAL_UINT32(NextExpectedCharIndex, index);
    TEST_ASSERT_LESS_THAN(sizeof(expected) / sizeof(expected[0]), index);
    TEST_ASSERT_EQUAL(expected[index], c);

    NextExpectedCharIndex++;
}



