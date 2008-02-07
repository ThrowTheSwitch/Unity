#define UNITY_ENABLE_EXTERNAL_ASSERTIONS

#include <setjmp.h>
#include "unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testTrue(void)
{
    TEST_ASSERT(1);
    
    TEST_ASSERT_TRUE(1);
}

void testFalse(void)
{
    TEST_ASSERT_FALSE(0);
    
    TEST_ASSERT_UNLESS(0);
}

void testPreviousPass(void)
{
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void vanilla_asserter(int val)
{
    TEST_ASSERT(val);
}

void testNotVanilla(void)
{
    int failed;
    vanilla_asserter(0);

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void true_asserter(int val)
{
    TEST_ASSERT_TRUE(val);
}

void testNotTrue(void)
{
    int failed;
    true_asserter(0);

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void false_asserter(int val)
{
    TEST_ASSERT_FALSE(val);
}

void testNotFalse(void)
{
    int failed;
    false_asserter(1);

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void unless_asserter(int val)
{
    TEST_ASSERT_UNLESS(val);
}

void testNotUnless(void)
{
    int failed;
    unless_asserter(1);

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void failer(void)
{
    TEST_FAIL("Expected for testing");
}

void testFail(void)
{
    int failed;
    failer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void ignorer(void)
{
    TEST_IGNORE();
    TEST_FAIL("This should not be reached");
}

void assertIgnoreInWrapper(void)
{
    TEST_WRAP(ignorer());
    TEST_FAIL("This should not be reached");
}

void testIgnoreInWrapper(void)
{
    unsigned char ignored;
    assertIgnoreInWrapper();
    ignored = Unity.CurrentTestIgnored;
    Unity.CurrentTestIgnored = 0;
    TEST_ASSERT_EQUAL_INT(1, ignored);
}

void testIgnore(void)
{
    int ignored;
    ignorer();

    ignored = Unity.CurrentTestIgnored;
    Unity.CurrentTestIgnored = 0;

    TEST_ASSERT(ignored);
}

void ignorerWithMessage(void)
{
    TEST_IGNORE_MESSAGE("This is an expected TEST_IGNORE_MESSAGE string!");
    TEST_FAIL("This should not be reached");
}

void testIgnoreMessage(void)
{
    int ignored;
    ignorerWithMessage();

    ignored = Unity.CurrentTestIgnored;
    Unity.CurrentTestIgnored = 0;

    TEST_ASSERT(ignored);
}

void assertIgnoreWithMessageInWrapper(void)
{
    TEST_WRAP(ignorerWithMessage());
    TEST_FAIL("This should not be reached");
}

void testIgnoreMessageInWrapper(void)
{
    unsigned char ignored;
    assertIgnoreWithMessageInWrapper();
    ignored = Unity.CurrentTestIgnored;
    Unity.CurrentTestIgnored = 0;
    TEST_ASSERT_EQUAL_INT(1, ignored);
}

void wrapper(void)
{
    TEST_WRAP(failer()); // if this doesn't force a return, then the failures will be incorrectly reset
    Unity.CurrentTestFailed = 0;
}

void testWrap(void)
{
    int failed;
    wrapper();
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0U;

    TEST_ASSERT_EQUAL_INT(1U, failed);
}

void intFailer(void)
{
    TEST_ASSERT_EQUAL_INT(3982, 3983);
}

void testNotEqualInts(void)
{
    int failed;
    intFailer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void bitFailer(void)
{
    TEST_ASSERT_BITS(0xFF00, 0x5555, 0x5A55);
}

void testNotEqualBits(void)
{
    int failed;
    bitFailer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void uintFailer(void)
{
    TEST_ASSERT_EQUAL_UINT(900000, 900001);
}

void testNotEqualUInts(void)
{
    int failed;
    uintFailer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void hex8Failer(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x23,0x22);
}

void testNotEqualHex8s(void)
{
    int failed;
    hex8Failer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void hex16Failer(void)
{
    TEST_ASSERT_EQUAL_HEX16(0x1234, 0x1235);
}

void testNotEqualHex16s(void)
{
    int failed;
    hex16Failer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void hex32Failer(void)
{
    TEST_ASSERT_EQUAL_HEX32(900000, 900001);
}

void testNotEqualHex32s(void)
{
    int failed;
    hex32Failer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void UnwrappedAssertion(int expected, int actual)
{
    TEST_ASSERT_EQUAL(expected,actual);
}

void testMultipleUnwrappedAssertionsHandledAppropriately(void)
{
    int failed;

    UnwrappedAssertion(4,5);
    UnwrappedAssertion(6,6);
    UnwrappedAssertion(19,19);
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testEqualInts(void)
{
    int v0, v1;
    int *p0, *p1;
    v0 = 19467;
    v1 = 19467;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT(1837, 1837);
    TEST_ASSERT_EQUAL_INT(-27365, -27365);
    TEST_ASSERT_EQUAL_INT(v0, v1);
    TEST_ASSERT_EQUAL_INT(19467, v1);
    TEST_ASSERT_EQUAL_INT(v0, 19467);
    TEST_ASSERT_EQUAL_INT(*p0, v1);
    TEST_ASSERT_EQUAL_INT(*p0, *p1);
    TEST_ASSERT_EQUAL_INT(*p0, 19467);
}

void testEqualUints(void)
{
    unsigned int v0, v1;
    unsigned int *p0, *p1;
    v0 = 19467;
    v1 = 19467;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_UINT(1837, 1837);
    TEST_ASSERT_EQUAL_UINT(v0, v1);
    TEST_ASSERT_EQUAL_UINT(19467, v1);
    TEST_ASSERT_EQUAL_UINT(v0, 19467);
    TEST_ASSERT_EQUAL_UINT(*p0, v1);
    TEST_ASSERT_EQUAL_UINT(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT(*p0, 19467);
}

void testEqualHex8s(void)
{
    unsigned int v0, v1;
    unsigned int *p0, *p1;
    v0 = 0x22;
    v1 = 0x22;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_HEX8(0x22, 0x22);
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    TEST_ASSERT_EQUAL_HEX8(0x22, v1);
    TEST_ASSERT_EQUAL_HEX8(v0, 0x22);
    TEST_ASSERT_EQUAL_HEX8(*p0, v1);
    TEST_ASSERT_EQUAL_HEX8(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX8(*p0, 0x22);
}

void testEqualHex16s(void)
{
    unsigned int v0, v1;
    unsigned int *p0, *p1;
    v0 = 0x9876;
    v1 = 0x9876;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_HEX16(0x9876, 0x9876);
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    TEST_ASSERT_EQUAL_HEX16(0x9876, v1);
    TEST_ASSERT_EQUAL_HEX16(v0, 0x9876);
    TEST_ASSERT_EQUAL_HEX16(*p0, v1);
    TEST_ASSERT_EQUAL_HEX16(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX16(*p0, 0x9876);
}

void testEqualHex32s(void)
{
    unsigned int v0, v1;
    unsigned int *p0, *p1;
    v0 = 0x98765432;
    v1 = 0x98765432;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_HEX32(0x98765432, 0x98765432);
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    TEST_ASSERT_EQUAL_HEX32(0x98765432, v1);
    TEST_ASSERT_EQUAL_HEX32(v0, 0x98765432);
    TEST_ASSERT_EQUAL_HEX32(*p0, v1);
    TEST_ASSERT_EQUAL_HEX32(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX32(*p0, 0x98765432);
}

void testEqualBits(void)
{
    unsigned int v0 = 0xFF55AA00;
    unsigned int v1 = 0x55550000;

    TEST_ASSERT_BITS(v1, v0, 0x55550000);
    TEST_ASSERT_BITS(v1, v0, 0xFF55CC00);
    TEST_ASSERT_BITS(0xFFFFFFFF, v0, 0xFF55AA00);
    TEST_ASSERT_BITS(0xFFFFFFFF, v0, v0);
    TEST_ASSERT_BITS(0xF0F0F0F0, v0, 0xFC5DAE0F);
    TEST_ASSERT_BITS_HIGH(v1, v0);
    TEST_ASSERT_BITS_LOW(0x000055FF, v0);
    TEST_ASSERT_BIT_HIGH(30, v0);
    TEST_ASSERT_BIT_LOW(5, v0);
}

void testEqualShorts(void)
{
    short v0, v1;
    short *p0, *p1;
    v0 = 19467;
    v1 = 19467;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT(1837, 1837);
    TEST_ASSERT_EQUAL_INT(-2987, -2987);
    TEST_ASSERT_EQUAL_INT(v0, v1);
    TEST_ASSERT_EQUAL_INT(19467, v1);
    TEST_ASSERT_EQUAL_INT(v0, 19467);
    TEST_ASSERT_EQUAL_INT(*p0, v1);
    TEST_ASSERT_EQUAL_INT(*p0, *p1);
    TEST_ASSERT_EQUAL_INT(*p0, 19467);
}

void testEqualUShorts(void)
{
    unsigned short v0, v1;
    unsigned short *p0, *p1;
    v0 = 19467;
    v1 = 19467;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT(1837, 1837);
    TEST_ASSERT_EQUAL_INT(-2987, -2987);
    TEST_ASSERT_EQUAL_INT(v0, v1);
    TEST_ASSERT_EQUAL_INT(19467, v1);
    TEST_ASSERT_EQUAL_INT(v0, 19467);
    TEST_ASSERT_EQUAL_INT(*p0, v1);
    TEST_ASSERT_EQUAL_INT(*p0, *p1);
    TEST_ASSERT_EQUAL_INT(*p0, 19467);
}

void testEqualChars(void)
{
    signed char v0, v1;
    signed char *p0, *p1;
    v0 = 109;
    v1 = 109;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT(42, 42);
    TEST_ASSERT_EQUAL_INT(-116, -116);
    TEST_ASSERT_EQUAL_INT(v0, v1);
    TEST_ASSERT_EQUAL_INT(109, v1);
    TEST_ASSERT_EQUAL_INT(v0, 109);
    TEST_ASSERT_EQUAL_INT(*p0, v1);
    TEST_ASSERT_EQUAL_INT(*p0, *p1);
    TEST_ASSERT_EQUAL_INT(*p0, 109);
}

void testEqualUChars(void)
{
    unsigned char v0, v1;
    unsigned char *p0, *p1;
    v0 = 251;
    v1 = 251;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT(42, 42);
    TEST_ASSERT_EQUAL_INT(v0, v1);
    TEST_ASSERT_EQUAL_INT(251, v1);
    TEST_ASSERT_EQUAL_INT(v0, 251);
    TEST_ASSERT_EQUAL_INT(*p0, v1);
    TEST_ASSERT_EQUAL_INT(*p0, *p1);
    TEST_ASSERT_EQUAL_INT(*p0, 251);
}

void testEqualPointers(void)
{
    int v0, v1;
    int *p0, *p1, *p2;
    v0 = 19467;
    v1 = 80080;
    p0 = &v0;
    p1 = &v1;
    p2 = &v1;

    TEST_ASSERT_EQUAL_INT(p0, &v0);
    TEST_ASSERT_EQUAL_INT(&v1, p1);
    TEST_ASSERT_EQUAL_INT(p2, p1);
    TEST_ASSERT_EQUAL_INT(&v0, &v0);
}

void testFloatsWithinDelta(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.00003f, 187245.03485f, 187245.03488f);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 187245.0f, 187246.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 9273.2549f, 9273.2049f);
    TEST_ASSERT_FLOAT_WITHIN(0.007f, -726.93724f, -726.94424f);
}

void floatWithinFailer(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 9273.2649f, 9273.2049f);
}

void testFloatsNotWithinDelta(void)
{
    int failed;
    floatWithinFailer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testIntsWithinDelta(void)
{
    TEST_ASSERT_INT_WITHIN(1, 5000, 5001);
    TEST_ASSERT_INT_WITHIN(5, 5000, 4996);
    TEST_ASSERT_INT_WITHIN(5, 5000, 5005);
    TEST_ASSERT_INT_WITHIN(500, 50, -440);
}

void intWithinFailer(void)
{
    TEST_ASSERT_INT_WITHIN(5, 5000, 5006);
}

void testIntsNotWithinDelta(void)
{
    int failed;
    intWithinFailer();

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testEqualStrings(void)
{
    const char *testString = "foo";

    TEST_ASSERT_EQUAL_STRING(testString, testString);
    TEST_ASSERT_EQUAL_STRING("foo", "foo");
    TEST_ASSERT_EQUAL_STRING("foo", testString);
    TEST_ASSERT_EQUAL_STRING(testString, "foo");
    TEST_ASSERT_EQUAL_STRING("", "");
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void stringFailer1(void)
{
    TEST_ASSERT_EQUAL_STRING("foo", "bar");
}

void stringFailer2(void)
{
    TEST_ASSERT_EQUAL_STRING("foo", "");
}

void stringFailer3(void)
{
    TEST_ASSERT_EQUAL_STRING("", "bar");
}

void testNotEqualString1(void)
{
    int failed;
    stringFailer1();
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testNotEqualString2(void)
{
    int failed;
    stringFailer2();
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testNotEqualString3(void)
{
    int failed;
    stringFailer3();
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void stringFailer_ExpectedStringIsNull(void)
{
    TEST_ASSERT_EQUAL_STRING(NULL, "bar");
}

void testNotEqualString_ExpectedStringIsNull(void)
{
    int failed;
    stringFailer_ExpectedStringIsNull();
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void stringFailer_ActualStringIsNull(void)
{
    TEST_ASSERT_EQUAL_STRING("foo", NULL);
}

void testNotEqualString_ActualStringIsNull(void)
{
    int failed;
    stringFailer_ActualStringIsNull();
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testProtection(void)
{
    volatile int mask = 0;
    jmp_buf AbortFrame;
    
    if (TEST_PROTECT())
    {
        mask |= 1;
        TEST_THROW("This throw was expected");
    }
    else
    {
        Unity.CurrentTestFailed = 0;
        mask |= 2;
    }
    
    TEST_ASSERT_EQUAL(3, mask);
}

