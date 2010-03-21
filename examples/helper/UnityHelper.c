#include "unity.h"
#include "UnityHelper.h"
#include <stdio.h>
#include <string.h>

static char message[1024];

void AssertFloatArrayWithin(const float tolerance, const float* expected, const float* actual, const unsigned int length, const unsigned short line)
{
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        sprintf(message, "Array mismatch at index %u, Expected %f, Actual %f, Tolerance %f, Delta %f", i, expected[i], actual[i], tolerance, (expected[i]-actual[i]));
        UNITY_TEST_ASSERT_WITHIN_FLOAT(tolerance, expected[i], actual[i], line, message);
    }
}

void AssertEqualExampleStruct(const EXAMPLE_STRUCT_T expected, const EXAMPLE_STRUCT_T actual, const unsigned short line)
{
    UNITY_TEST_ASSERT_EQUAL_INT(expected.x, actual.x, line, "Example Struct Failed For Field x");
    UNITY_TEST_ASSERT_EQUAL_INT(expected.y, actual.y, line, "Example Struct Failed For Field y");
}
