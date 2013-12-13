/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include <setjmp.h>
#include "unity.h"

#define TEST_RANGE(...)

#define EXPECT_ABORT_BEGIN \
    if (TEST_PROTECT())    \
    {
    
#define VERIFY_FAILS_END                                                       \
    }                                                                          \
    Unity.CurrentTestFailed = (Unity.CurrentTestFailed == 1) ? 0 : 1;          \
    if (Unity.CurrentTestFailed == 1) {                                        \
      SetToOneMeanWeAlreadyCheckedThisGuy = 1;                                 \
      UnityPrint("[[[[ Previous Test Should Have Failed But Did Not ]]]]");    \
      UNITY_OUTPUT_CHAR('\n');                                                 \
    }
    
#define VERIFY_IGNORES_END                                                     \
    }                                                                          \
    Unity.CurrentTestFailed = (Unity.CurrentTestIgnored == 1) ? 0 : 1;         \
    Unity.CurrentTestIgnored = 0;                                              \
    if (Unity.CurrentTestFailed == 1) {                                        \
      SetToOneMeanWeAlreadyCheckedThisGuy = 1;                                 \
      UnityPrint("[[[[ Previous Test Should Have Ignored But Did Not ]]]]");   \
      UNITY_OUTPUT_CHAR('\n');                                                 \
    }

int SetToOneToFailInTearDown;
int SetToOneMeanWeAlreadyCheckedThisGuy;

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
    UnityPrint("[[[[ Previous Test Should Have Passed But Did Not ]]]]");
    UNITY_OUTPUT_CHAR('\n');
  }
}

/********************************************************************* OneArg */
/* [0]=start [1]=stop [2]=increment */
TEST_RANGE([1,5,1])
void test_TheseShouldAllPass_OneArg(int Num)
{
    TEST_ASSERT_TRUE(Num < 100);
}

/* [0]=start [1]=stop [2]=increment */
TEST_RANGE([2,11,2])
void test_TheseShouldAllFail_OneArg(int Num)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(Num > 100);
    VERIFY_FAILS_END
}

/* [0]=start [1]=stop [2]=increment */
TEST_RANGE([1,5,1])
void test_TheseAreEveryOther_OneArg(int Num)
{
    if (Num & 1)
    {
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_FALSE(Num & 1);
        VERIFY_FAILS_END
    }
    else
    {
        TEST_ASSERT_TRUE(Num < 100);
    }
}

/********************************************************************* TwoArg */
/* [0]=start [1]=stop [2]=increment */
TEST_RANGE([1,3,1], [3.0, 4.0, 0.3])
void test_TheseShouldAllPass_TwoArg(int Num, float fNum)
{
    TEST_ASSERT_TRUE(Num < 100);
    TEST_ASSERT_TRUE(fNum < 5.0 );
}

/* [0]=start [1]=stop [2]=increment */
TEST_RANGE([2,7,2], [3.0, 4.0, 0.2])
void test_TheseShouldAllFail_TwoArg(int Num, float fNum)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(Num > 100);
    TEST_ASSERT_TRUE(fNum > 5.0);
    VERIFY_FAILS_END
}

/* [0]=start [1]=stop [2]=increment */
TEST_RANGE([1,5,1], [3.0, 4.0, 0.4])
void test_TheseAreEveryOther_TwoArg(int Num, float fNum)
{
    if (Num & 1)
    {
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_FALSE(Num & 1);
        TEST_ASSERT_TRUE(fNum > 5.0);
        VERIFY_FAILS_END
    }
    else
    {
        TEST_ASSERT_TRUE(Num < 100);
        TEST_ASSERT_TRUE(fNum < 5.0);
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
