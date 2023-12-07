/* Copyright (c) 2010 James Grenning and Contributed to Unity Project
 * ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

// by using unity_easy.h, not need to write the runners, automatic assembly of test cases and test groups.
#include "unity_easy.h"

static int data = -1;

// set test group name
#define __GROUP__ mygroup

TEST_SETUP()
{
    data = 0;
}

TEST_TEAR_DOWN()
{
    data = -1;
}

TEST(test1)
{
    TEST_ASSERT_EQUAL_INT(0, data);
}

TEST(test2)
{
    TEST_ASSERT_EQUAL_INT(0, data);
    data = 5;
}

TEST(test3)
{
    data = 7;
    TEST_ASSERT_EQUAL_INT(7, data);
}

// test group end
#undef __GROUP__

// start next test group
#define __GROUP__ mygroup2

TEST_SETUP()
{
    data = 4;
}

TEST_TEAR_DOWN()
{
    data = -1;
}

TEST(test1)
{
    TEST_ASSERT_EQUAL_INT(0, data);
}

TEST(test2)
{
    TEST_ASSERT_EQUAL_INT(0, data);
    data = 5;
}

TEST(test3)
{
    data = 7;
    TEST_ASSERT_EQUAL_INT(7, data);
}



