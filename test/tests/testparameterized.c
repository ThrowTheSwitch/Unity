/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include <setjmp.h>
#include "unity.h"

#define TEST_CASE(...)

void setUp(void)
{
}

void tearDown(void)
{
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
    VERIFY_FAILS_END(NULL);
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
        VERIFY_FAILS_END(NULL);
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
    VERIFY_FAILS_END(NULL);
}
