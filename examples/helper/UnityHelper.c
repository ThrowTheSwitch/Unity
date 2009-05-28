#include "unity.h"
#include "UnityHelper.h"
#include <stdio.h>
#include <string.h>

static char message[1024];

void AssertEqualUintArray(const unsigned int* expected, const unsigned int* actual, const unsigned int length)
{
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        sprintf(message, "Array comparison failed at index %u.", i);
        TEST_ASSERT_EQUAL_UINT_MESSAGE(expected[i], actual[i], message);
    }
}

void AssertEqualIntArray(const int* expected, const int* actual, const unsigned int length)
{
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        sprintf(message, "Array comparison failed at index %u.", i);
        TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], actual[i], message);
    }
}

void AssertFloatArrayWithin(const float tolerance, const float* expected, const float* actual, const unsigned int length)
{
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        sprintf(message, "Array mismatch at index %u, Expected %f, Actual %f, Tolerance %f, Delta %f", i, expected[i], actual[i], tolerance, (expected[i]-actual[i]));
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(tolerance, expected[i], actual[i], message);
    }
}

void AssertEqualExampleStruct(EXAMPLE_STRUCT_T expected, EXAMPLE_STRUCT_T actual)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected.x, actual.x, "Example Struct Failed For Field x");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected.y, actual.y, "Example Struct Failed For Field y");
}
