#define UNITY_ENABLE_EXTERNAL_ASSERTIONS

#include <setjmp.h>
#include "unity.h"

#define EXPECT_ABORT_BEGIN \
  { \
    if (TEST_PROTECT()) \
    {

#define EXPECT_ABORT_END \
    } \
  }

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

void testNotVanilla(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT(0);
    EXPECT_ABORT_END
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void testNotTrue(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(0);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void testNotFalse(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FALSE(1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void testNotUnless(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UNLESS(1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void testFail(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_FAIL("Expected for testing");
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void testIgnore(void)
{
    int ignored;
    
    EXPECT_ABORT_BEGIN
    TEST_IGNORE();
    TEST_FAIL("This should not be reached");
    EXPECT_ABORT_END

    ignored = Unity.CurrentTestIgnored;
    Unity.CurrentTestIgnored = 0;

    TEST_ASSERT(ignored);
}

void testIgnoreMessage(void)
{
    int ignored;
    
    EXPECT_ABORT_BEGIN
    TEST_IGNORE_MESSAGE("This is an expected TEST_IGNORE_MESSAGE string!");
    TEST_FAIL("This should not be reached");
    EXPECT_ABORT_END

    ignored = Unity.CurrentTestIgnored;
    Unity.CurrentTestIgnored = 0;

    TEST_ASSERT(ignored);
}

void testNotEqualInts(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(3982, 3983);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualBits(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS(0xFF00, 0x5555, 0x5A55);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}


void testNotEqualUInts(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT(900000, 900001);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex8s(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(0x23,0x22);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex16s(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(0x1234, 0x1235);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex32s(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(900000, 900001);
    EXPECT_ABORT_END

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

void testFloatsNotWithinDelta(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 9273.2649f, 9273.2049f);
    EXPECT_ABORT_END

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

void testIntsNotWithinDelta(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_WITHIN(5, 5000, 5006);
    EXPECT_ABORT_END

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

void testNotEqualString1(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "bar");
    EXPECT_ABORT_END
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testNotEqualString2(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "");
    EXPECT_ABORT_END
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testNotEqualString3(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("", "bar");
    EXPECT_ABORT_END
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testNotEqualString_ExpectedStringIsNull(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(NULL, "bar");
    EXPECT_ABORT_END
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testNotEqualString_ActualStringIsNull(void)
{
    int failed;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", NULL);
    EXPECT_ABORT_END
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is also expected");
}

void testProtection(void)
{
    volatile int mask = 0;
    
    if (TEST_PROTECT())
    {
        mask |= 1;
        TEST_ABORT();
    }
    else
    {
        Unity.CurrentTestFailed = 0;
        mask |= 2;
    }
    
    TEST_ASSERT_EQUAL(3, mask);
}

