/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

/* This Test File Is Used To Verify Name Filters Applied To Parameterized Tests With More Than One Argument */

#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

TEST_CASE(0,0)
TEST_CASE(1,1)
TEST_CASE(10,10)
void paratest_First(int a, int b)
{
    TEST_ASSERT_EQUAL_INT(a, b);
}

TEST_CASE(0,0)
TEST_CASE(2,2)
TEST_CASE(20,20)
void paratest_Second(int a, int b)
{
    TEST_ASSERT_EQUAL_INT(a, b);
}

void paratest_Plain(void)
{
    TEST_PASS();
}
