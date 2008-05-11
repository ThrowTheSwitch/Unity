#include "unity.h"
#include "UnityHelper.h"
#include <stdio.h>
#include <string.h>

static char message[1024];

void AssertEqualArrayUint(unsigned int* expected, unsigned int* actual, unsigned int length)
{
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        sprintf(message, "Array comparison failed at index %u.", i);
        TEST_ASSERT_EQUAL_UINT_MESSAGE(expected[i], actual[i], message);
    }
}

void AssertEqualArrayInt(int* expected, int* actual, unsigned int length)
{
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        sprintf(message, "Array comparison failed at index %u.", i);
        TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], actual[i], message);
    }
}

void AssertEqualArrayFloatWithin(float tolerance, float* expected, float* actual, unsigned int length)
{
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        sprintf(message, "Array mismatch at index %u, Expected %f, Actual %f, Tolerance %f, Delta %f", i, expected[i], actual[i], tolerance, (expected[i]-actual[i]));
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(tolerance, expected[i], actual[i], message);
    }
}
