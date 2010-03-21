#ifndef _TESTHELPER_H
#define _TESTHELPER_H

#include "Types.h"

void AssertFloatArrayWithin(const float tolerance, const float* expected, const float* actual, const unsigned int length, const unsigned short line);
void AssertEqualExampleStruct(const EXAMPLE_STRUCT_T expected, const EXAMPLE_STRUCT_T actual, const unsigned short line);

#define UNITY_TEST_ASSERT_FLOAT_ARRAY_WITHIN(tolerance, expected, actual, length, line, message) AssertFloatArrayWithin(tolerance, expected, actual, length, line);
#define UNITY_TEST_ASSERT_EQUAL_EXAMPLE_STRUCT_T(expected, actual, line, message) AssertEqualExampleStruct(expected, actual, line);

#define TEST_ASSERT_FLOAT_ARRAY_WITHIN(tolerance, expected, actual, length) UNITY_TEST_ASSERT_FLOAT_ARRAY_WITHIN(tolerance, expected, actual, __LINE__, NULL);
#define TEST_ASSERT_EQUAL_EXAMPLE_STRUCT_T(expected, actual) UNITY_TEST_ASSERT_EQUAL_EXAMPLE_STRUCT_T(expected, actual, __LINE__, NULL);

#endif // _TESTHELPER_H
