#include "unity.h"

/* Support for Meta Test Rig */
#ifndef TEST_CASE
#define TEST_CASE(...)
#endif
#ifndef TEST_RANGE
#define TEST_RANGE(...)
#endif
#ifndef TEST_MATRIX
#define TEST_MATRIX(...)
#endif

TEST_CASE(1, 2, 5)
TEST_CASE(10, 7, 20)
TEST_RANGE([3, 4, 1], [10, 5, -2], <30, 31, 1>)
TEST_MATRIX([3, 4, 7], [10, 8, 2, 1],[30u, 20.0f])
void test_demoParamFunction(int a, int b, int c)
{
  TEST_ASSERT_GREATER_THAN_INT(a + b, c);
}
