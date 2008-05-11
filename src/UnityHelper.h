#ifndef _TESTHELPER_H
#define _TESTHELPER_H

void AssertEqualArrayUint(unsigned int* expected, unsigned int* actual, unsigned int length);
void AssertEqualArrayInt(int* expected, int* actual, unsigned int length);
void AssertEqualArrayFloatWithin(float tolerance, float* expected, float* actual, unsigned int length);

#define TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, length) {TEST_WRAP(AssertEqualArrayUint(expected, actual, length));}
#define TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, length) {TEST_WRAP(AssertEqualArrayInt(expected, actual, length));}
#define TEST_ASSERT_FLOAT_ARRAY_WITHIN(tolerance, expected, actual, length) {TEST_WRAP(AssertEqualArrayFloatWithin(tolerance, expected, actual, length));}

#endif // _TESTHELPER_H
