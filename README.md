# Unity Test ![CI][]

__Copyright (c) 2007 - 2021 Unity Project by Mike Karlesky, Mark VanderVoord, Greg Williams, and David Koch__

Welcome to the Unity Test Project, one of the main projects of ThrowTheSwitch.org.
Unity Test is a unit testing framework built for C, with a focus on working with embedded toolchains.

This project is made to test code targetting microcontrollers big and small.
The core project is a single C file and a pair of headers, allowing it to the added to your existing build setup without too much headache.
You may use any compiler you wish, and may use most existing build systems including Make, CMake, etc.
If you'd like to leave the hard work to us, you might be interested in Ceedling, a build tool also by ThrowTheSwitch.org.

If you're new to Unity, we encourage you to tour the [getting started guide][].

## Getting Started

The [docs][] folder contains a [getting started guide][] and much more tips about using Unity.

## Unity Assertion Summary

For the full list, see [UnityAssertionsReference.md][].

### Basic Validity Tests

    TEST_ASSERT__TRUE(condition)

Evaluates whatever code is in condition and fails if it evaluates to false

    TEST_ASSERT__FALSE(condition)

Evaluates whatever code is in condition and fails if it evaluates to true

    TEST_ASSERT(condition)

Another way of calling `TEST_ASSERT__TRUE`

    TEST_ASSERT__UNLESS(condition)

Another way of calling `TEST_ASSERT__FALSE`

    TEST_FAIL()
    TEST_FAIL__MESSAGE(message)

This test is automatically marked as a failure.
The message is output stating why.

### Numerical Assertions: Integers

    TEST_ASSERT__EQUAL__INT(expected, actual)
    TEST_ASSERT__EQUAL__INT8(expected, actual)
    TEST_ASSERT__EQUAL__INT16(expected, actual)
    TEST_ASSERT__EQUAL__INT32(expected, actual)
    TEST_ASSERT__EQUAL__INT64(expected, actual)

Compare two integers for equality and display errors as signed integers.
A cast will be performed to your natural integer size so often this can just be used.
When you need to specify the exact size, like when comparing arrays, you can use a specific version:

    TEST_ASSERT__EQUAL__UINT(expected, actual)
    TEST_ASSERT__EQUAL__UINT8(expected, actual)
    TEST_ASSERT__EQUAL__UINT16(expected, actual)
    TEST_ASSERT__EQUAL__UINT32(expected, actual)
    TEST_ASSERT__EQUAL__UINT64(expected, actual)

Compare two integers for equality and display errors as unsigned integers.
Like INT, there are variants for different sizes also.

    TEST_ASSERT__EQUAL__HEX(expected, actual)
    TEST_ASSERT__EQUAL__HEX8(expected, actual)
    TEST_ASSERT__EQUAL__HEX16(expected, actual)
    TEST_ASSERT__EQUAL__HEX32(expected, actual)
    TEST_ASSERT__EQUAL__HEX64(expected, actual)

Compares two integers for equality and display errors as hexadecimal.
Like the other integer comparisons, you can specify the size... here the size will also effect how many nibbles are shown (for example, `HEX16` will show 4 nibbles).

    TEST_ASSERT__EQUAL(expected, actual)

Another way of calling TEST_ASSERT__EQUAL__INT

    TEST_ASSERT__WITHIN__INT(delta, expected, actual)

Asserts that the actual value is within plus or minus delta of the expected value.
This also comes in size specific variants.

    TEST_ASSERT__GREATER_THAN(threshold, actual)

Asserts that the actual value is greater than the threshold.
This also comes in size specific variants.

    TEST_ASSERT__LESS_THAN(threshold, actual)

Asserts that the actual value is less than the threshold.
This also comes in size specific variants.

### Arrays

    _ARRAY

You can append `_ARRAY` to any of these macros to make an array comparison of that type.
Here you will need to care a bit more about the actual size of the value being checked.
You will also specify an additional argument which is the number of elements to compare.
For example:

    TEST_ASSERT__EQUAL_ARRAY__HEX8(expected, actual, elements)

    _EQUAL_EACH

Another array comparison option is to check that EVERY element of an array is equal to a single expected value.
You do this by specifying the EQUAL_EACH macro.
For example:

    TEST_ASSERT__EQUAL_EACH__INT32(expected, actual, elements)

### Numerical Assertions: Bitwise

    TEST_ASSERT__EQUAL__BITS(mask, expected, actual)

Use an integer mask to specify which bits should be compared between two other integers.
High bits in the mask are compared, low bits ignored.

    TEST_ASSERT__EQUAL__BITS_HIGH(mask, actual)

Use an integer mask to specify which bits should be inspected to determine if they are all set high.
High bits in the mask are compared, low bits ignored.

    TEST_ASSERT__EQUAL__BITS_LOW(mask, actual)

Use an integer mask to specify which bits should be inspected to determine if they are all set low.
High bits in the mask are compared, low bits ignored.

    TEST_ASSERT__EQUAL__BIT_HIGH(bit, actual)

Test a single bit and verify that it is high.
The bit is specified 0-31 for a 32-bit integer.

    TEST_ASSERT__EQUAL__BIT_LOW(bit, actual)

Test a single bit and verify that it is low.
The bit is specified 0-31 for a 32-bit integer.

### Numerical Assertions: Floats

    TEST_ASSERT__WITHIN__FLOAT(delta, expected, actual)

Asserts that the actual value is within plus or minus delta of the expected value.

    TEST_ASSERT__EQUAL__FLOAT(expected, actual)
    TEST_ASSERT__EQUAL__DOUBLE(expected, actual)

Asserts that two floating point values are "equal" within a small % delta of the expected value.

### String Assertions

    TEST_ASSERT__EQUAL__STRING(expected, actual)

Compare two null-terminate strings.
Fail if any character is different or if the lengths are different.

    TEST_ASSERT__EQUAL__STRING_LEN(expected, actual, len)

Compare two strings.
Fail if any character is different, stop comparing after len characters.

    TEST_ASSERT__EQUAL__STRING_MESSAGE(expected, actual, message)

Compare two null-terminate strings.
Fail if any character is different or if the lengths are different.
Output a custom message on failure.

    TEST_ASSERT__EQUAL__STRING_LEN_MESSAGE(expected, actual, len, message)

Compare two strings.
Fail if any character is different, stop comparing after len characters.
Output a custom message on failure.

### Pointer Assertions

Most pointer operations can be performed by simply using the integer comparisons above.
However, a couple of special cases are added for clarity.

    TEST_ASSERT__NULL(pointer)

Fails if the pointer is not equal to NULL

    TEST_ASSERT__NOT_NULL(pointer)

Fails if the pointer is equal to NULL

### Memory Assertions

    TEST_ASSERT__EQUAL__MEMORY(expected, actual, len)

Compare two blocks of memory.
This is a good generic assertion for types that can't be coerced into acting like standard types... but since it's a memory compare, you have to be careful that your data types are packed.

### \_MESSAGE

You can append `\_MESSAGE` to any of the macros to make them take an additional argument.
This argument is a string that will be printed at the end of the failure strings.
This is useful for specifying more information about the problem.


### Integrated framework

The integrated framework is able to construct a test plan out of test fixtures including test cases.
To do so, it is required to TEST_PLAN__OPEN(plan), add test fixtures with TEST_PLAN__FIXT_ADD(fixt) before closing using TEST_PLAN__CLOSE(name).
The same for TEST_FIXT__OPEN(fixt) and TEST_CASE__OPEN(test).
For the test cases, it is required to add testing code like a normal function, yet finished with TEST_CASE__CLOSE(test).

    TEST_CASE__OPEN(MyTestCase)
        TEST_ASSERT__TRUE_MESSAGE(FALSE, "Gotcha!");
    TEST_CASE__CLOSE(MyTestCase)

    TEST_FIXT__OPEN(MyTestFixture)
        TEST_FIXT__CASE_ADD(MyTestCase)
    TEST_FIXT__CLOSE(MyTestFixture

    TEST_PLAN__OPEN(MyTestPlan)
        TEST_PLAN__FIXT_ADD(MyTestFixt)
    TEST_PLAN__CLOSE(MyTestPlan)

    void main(void)
    {
        TEST_PLAN__RUN(MyTestPlan);
    }

It is not required to finish lines with a semicolon, but TEST_ASSERT__ and __RUN lines.
Conditional compilation can be used to modify the test plan/fixture/case sequencing.

[CI]: https://github.com/ThrowTheSwitch/Unity/workflows/CI/badge.svg
[getting started guide]: docs/UnityGettingStartedGuide.md
[docs]: docs/
[UnityAssertionsReference.md]: docs/UnityAssertionsReference.md
