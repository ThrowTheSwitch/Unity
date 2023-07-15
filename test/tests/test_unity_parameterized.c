/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include <setjmp.h>
#include <stdio.h>
#include "unity.h"
#include "types_for_test.h"

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

static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;
static unsigned NextExpectedStringIndex;
static unsigned NextExpectedCharIndex;
static unsigned NextExpectedSpaceIndex;

void suiteSetUp(void)
{
    NextExpectedStringIndex = 0;
    NextExpectedCharIndex = 0;
    NextExpectedSpaceIndex = 0;
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

TEST_RANGE([0, 10, 2])
void test_SingleRange(unsigned value)
{
  TEST_ASSERT_EQUAL(0, value % 2);
  TEST_ASSERT_LESS_OR_EQUAL(10, value);
}

TEST_RANGE([1, 2, 1], [2, 1, -1])
void test_TwoRanges(unsigned first, unsigned second)
{
  TEST_ASSERT_LESS_OR_EQUAL(4, first * second);
}

TEST_RANGE(<0, 10, 2>)
void test_SingleExclusiveRange(unsigned value)
{
  TEST_ASSERT_EQUAL(0, value % 2);
  TEST_ASSERT_LESS_THAN(10, value);
}

TEST_RANGE([2, 4, 1], <1, 2, 1>)
void test_BothInclusiveAndExclusiveRange(unsigned first, unsigned second)
{
  TEST_ASSERT_LESS_THAN(first, second);
}

TEST_CASE(0,

          1)
TEST_CASE(1,

          2

  )
TEST_RANGE([2,
            5  ,
            1], [6, 6, 1])
TEST_CASE(

  6 , 7)
TEST_MATRIX([7,
             8   ,

             9, 10],
    [
      11]

      )
void test_SpaceInTestCase(unsigned index, unsigned bigger)
{
  TEST_ASSERT_EQUAL_UINT32(NextExpectedSpaceIndex, index);
  TEST_ASSERT_LESS_THAN(bigger, index);

  NextExpectedSpaceIndex++;
}

TEST_MATRIX([1, 5, (2*2)+1, 4])
void test_SingleMatix(unsigned value)
{
  TEST_ASSERT_LESS_OR_EQUAL(10, value);
}

TEST_MATRIX([2, 5l, 4u+3, 4ul], [-2, 3])
void test_TwoMatrices(unsigned first, signed second)
{
  static unsigned idx = 0;
  static const unsigned expected[] =
  {
  // -2   3
     -4,  6, // 2
    -10, 15, // 5
    -14, 21, // 7
     -8, 12, // 4
  };
  TEST_ASSERT_EQUAL_INT(expected[idx++], first * second);
}

TEST_MATRIX(["String1", "String,2", "Stri" "ng3", "String[4]", "String\"5\""], [-5, 12.5f])
void test_StringsAndNumbersMatrices(const char* str, float number)
{
  static unsigned idx = 0;
  static const char* expected[] =
  {
    "String1_-05.00",
    "String1_+12.50",
    "String,2_-05.00",
    "String,2_+12.50",
    "String3_-05.00",
    "String3_+12.50",
    "String[4]_-05.00",
    "String[4]_+12.50",
    "String\"5\"_-05.00",
    "String\"5\"_+12.50",
  };
  char buf[200] = {0};
  snprintf(buf, sizeof(buf), "%s_%+06.2f", str, number);
  TEST_ASSERT_EQUAL_STRING(expected[idx++], buf);
}

TEST_MATRIX(
  [ENUM_A, ENUM_4, ENUM_C],
  [test_arr[0], 7.8f, test_arr[2]],
  ['a', 'f', '[', ']', '\'', '"'],
)
void test_EnumCharAndArrayMatrices(test_enum_t e, float n, char c)
{
  static unsigned enum_idx = 0;
  static const test_enum_t exp_enum[3] = {
    ENUM_A, ENUM_4, ENUM_C,
  };

  static unsigned float_idx = 0;
  float exp_float[3] = {0};
  exp_float[0] = test_arr[0];
  exp_float[1] = 7.8f;
  exp_float[2] = test_arr[2];

  static unsigned char_idx = 0;
  static const test_enum_t exp_char[] = {
    'a', 'f', '[', ']', '\'', '"'
  };

  TEST_ASSERT_EQUAL_INT(exp_enum[enum_idx], e);
  TEST_ASSERT_EQUAL_FLOAT(exp_float[float_idx], n);
  TEST_ASSERT_EQUAL_CHAR(exp_char[char_idx], c);

  char_idx = (char_idx + 1) % 6;
  if (char_idx == 0.0f)
  {
    float_idx = (float_idx + 1) % 3;
    if (float_idx == 0.0f)
    {
      enum_idx = (enum_idx + 1) % 3;
    }
  }
}
