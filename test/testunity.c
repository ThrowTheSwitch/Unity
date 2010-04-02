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

#define VERIFY_FAILURE_WAS_CAUGHT \
    EXPECT_ABORT_BEGIN \
      TEST_ASSERT_MESSAGE((1u == failed), "<---- [ This Test Should Have Failed But Did Not ]"); \
    EXPECT_ABORT_END

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

void testNotNotEqual(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL(10, 10);
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

void testIsNull(void)
{
    char* ptr1 = NULL;
    char* ptr2 = "hello";
    
    TEST_ASSERT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
}

void testIsNullShouldFailIfNot(void)
{
    int failed;
    char* ptr1 = "hello";
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NULL(ptr1);
    EXPECT_ABORT_END
    
    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_EQUAL_INT(1U, failed);
    TEST_ASSERT_EQUAL_INT(0U, Unity.TestFailures);
}

void testNotNullShouldFailIfNULL(void)
{
    int failed;
    char* ptr1 = NULL;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_NULL(ptr1);
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
    _UU16 v0, v1;
    
    v0 = 9000;
    v1 = 9001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT(v0, v1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex8s(void)
{
    int failed;
    _UU8 v0, v1;
    
    v0 = 0x23;
    v1 = 0x22;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex8sIfSigned(void)
{
    int failed;
    _US8 v0, v1;
    
    v0 = -2;
    v1 = 2;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex16s(void)
{
    int failed;
    _UU16 v0, v1;
    
    v0 = 0x1234;
    v1 = 0x1235;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex16sIfSigned(void)
{
    int failed;
    _US16 v0, v1;
    
    v0 = -1024;
    v1 = -1028;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex32s(void)
{
    int failed;
    _UU32 v0, v1;
    
    v0 = 900000;
    v1 = 900001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    TEST_ASSERT_MESSAGE(1U == failed, "This is expected");
}

void testNotEqualHex32sIfSigned(void)
{
    int failed;
    _US32 v0, v1;
    
    v0 = -900000;
    v1 = 900001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
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
    TEST_ASSERT_EQUAL_UINT(60872u, 60872u);
}

void testNotEqual(void)
{
    TEST_ASSERT_NOT_EQUAL(0, 1);
    TEST_ASSERT_NOT_EQUAL(1, 0);
    TEST_ASSERT_NOT_EQUAL(100, 101);
    TEST_ASSERT_NOT_EQUAL(0, -1);
    TEST_ASSERT_NOT_EQUAL(65535, -65535);
    TEST_ASSERT_NOT_EQUAL(75, 900);
    TEST_ASSERT_NOT_EQUAL(-100, -101);
}

void testEqualHex8s(void)
{
    _UU8 v0, v1;
    _UU8 *p0, *p1;
    
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
    _UU16 v0, v1;
    _UU16 *p0, *p1;
    
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
    _UU32 v0, v1;
    _UU32 *p0, *p1;
    
    v0 = 0x98765432ul;
    v1 = 0x98765432ul;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_HEX32(0x98765432ul, 0x98765432ul);
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    TEST_ASSERT_EQUAL_HEX32(0x98765432ul, v1);
    TEST_ASSERT_EQUAL_HEX32(v0, 0x98765432ul);
    TEST_ASSERT_EQUAL_HEX32(*p0, v1);
    TEST_ASSERT_EQUAL_HEX32(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX32(*p0, 0x98765432ul);
}

void testEqualBits(void)
{
    _UU32 v0 = 0xFF55AA00;
    _UU32 v1 = 0x55550000;

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

    TEST_ASSERT_EQUAL_UINT(1837, 1837);
    TEST_ASSERT_EQUAL_UINT(2987, 2987);
    TEST_ASSERT_EQUAL_UINT(v0, v1);
    TEST_ASSERT_EQUAL_UINT(19467, v1);
    TEST_ASSERT_EQUAL_UINT(v0, 19467);
    TEST_ASSERT_EQUAL_UINT(*p0, v1);
    TEST_ASSERT_EQUAL_UINT(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT(*p0, 19467);
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
    v1 = 18271;
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

    VERIFY_FAILURE_WAS_CAUGHT
}

void testFloatsEqual(void)
{
    TEST_ASSERT_EQUAL_FLOAT(187245.0f, 187246.0f);
    TEST_ASSERT_EQUAL_FLOAT(18724.5f, 18724.6f);
    TEST_ASSERT_EQUAL_FLOAT(9273.2549f, 9273.2599f);
    TEST_ASSERT_EQUAL_FLOAT(-726.93724f, -726.9374f);
}

void testFloatsNotEqual(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(9273.9649f, 9273.0049f);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testIntsWithinDelta(void)
{
    TEST_ASSERT_INT_WITHIN(1, 5000, 5001);
    TEST_ASSERT_INT_WITHIN(5, 5000, 4996);
    TEST_ASSERT_INT_WITHIN(5, 5000, 5005);
    TEST_ASSERT_INT_WITHIN(500, 50, -440);
    
    TEST_ASSERT_INT_WITHIN(2, 2147483647, -1);
    TEST_ASSERT_INT_WITHIN(5, 1, -1);
    TEST_ASSERT_INT_WITHIN(5, -1, 1);
}

void testIntsNotWithinDelta(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_WITHIN(5, 5000, 5006);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testUIntsWithinDelta(void)
{
    TEST_ASSERT_UINT_WITHIN(1, 5000, 5001);
    TEST_ASSERT_UINT_WITHIN(5, 5000, 4996);
    TEST_ASSERT_UINT_WITHIN(5, 5000, 5005);
}

void testUIntsNotWithinDelta(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(1, 2147483647, 2147483649);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(5, 1, -1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(5, -1, 1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testHEX32sWithinDelta(void)
{
    TEST_ASSERT_HEX32_WITHIN(1, 5000, 5001);
    TEST_ASSERT_HEX32_WITHIN(5, 5000, 4996);
    TEST_ASSERT_HEX32_WITHIN(5, 5000, 5005);
}

void testHEX32sNotWithinDelta(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN(1, 2147483647, 2147483649);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testHEX32sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN(5, 1, -1);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testHEX16sWithinDelta(void)
{
    TEST_ASSERT_HEX16_WITHIN(1, 5000, 5001);
    TEST_ASSERT_HEX16_WITHIN(5, 5000, 4996);
    TEST_ASSERT_HEX16_WITHIN(5, 5000, 5005);
}

void testHEX16sNotWithinDelta(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN(2, 65535, 0);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testHEX8sWithinDelta(void)
{
    TEST_ASSERT_HEX8_WITHIN(1, 254, 255);
    TEST_ASSERT_HEX8_WITHIN(5, 251, 255);
    TEST_ASSERT_HEX8_WITHIN(5, 1, 4);
}

void testHEX8sNotWithinDelta(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN(2, 255, 0);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
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

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualString2(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "");
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualString3(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("", "bar");
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualString_ExpectedStringIsNull(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(NULL, "bar");
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualString_ActualStringIsNull(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", NULL);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualMemory(void)
{
    const char *testString = "whatever";
    
    TEST_ASSERT_EQUAL_MEMORY(testString, testString, 8);
    TEST_ASSERT_EQUAL_MEMORY("whatever", "whatever", 8);
    TEST_ASSERT_EQUAL_MEMORY("whatever", testString, 8);
    TEST_ASSERT_EQUAL_MEMORY(testString, "whatever", 8);
    TEST_ASSERT_EQUAL_MEMORY(testString, "whatever", 2);
    TEST_ASSERT_EQUAL_MEMORY(NULL, NULL, 1);
}

void testNotEqualMemory1(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("foo", "bar", 3);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualMemory2(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", "food", 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualMemory3(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY(NULL, "food", 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualMemory4(void)
{
    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", NULL, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualIntArrays(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    int p2[] = {1, 8, 987, 2};
    int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p3, 1);
}

void testNotEqualIntArrays1(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualIntArrays2(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualIntArrays3(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualUIntArrays(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65132u};
    unsigned int p2[] = {1, 8, 987, 2};
    unsigned int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p3, 1);
}

void testNotEqualUIntArrays1(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65131u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualUIntArrays2(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {2, 8, 987, 65132u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualUIntArrays3(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 986, 65132u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualHEXArrays(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65132u};
    unsigned int p2[] = {1, 8, 987, 2};
    unsigned int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p3, 1);
}

void testNotEqualHEXArrays1(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65131u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualHEXArrays2(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {2, 8, 987, 65132u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualHEXArrays3(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 986, 65132u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualHEX16Arrays(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65132u};
    unsigned short p2[] = {1, 8, 987, 2};
    unsigned short p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p3, 1);
}

void testNotEqualHEX16Arrays1(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65131u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualHEX16Arrays2(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualHEX16Arrays3(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualHEX8Arrays(void)
{
    unsigned short p0[] = {1, 8, 254u, 123};
    unsigned short p1[] = {1, 8, 254u, 123};
    unsigned short p2[] = {1, 8, 254u, 2};
    unsigned short p3[] = {1, 23, 25, 26};

    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p3, 1);
}

void testNotEqualHEX8Arrays1(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 254u, 252u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualHEX8Arrays2(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {2, 8, 254u, 253u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualHEX8Arrays3(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 255u, 253u};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualFloatArrays(void)
{
    float p0[] = {1.0, 8.0,  25.4, 0.123};
    float p1[] = {1.0, 8.0,  25.4, 0.123};
    float p2[] = {1.0, 8.0,  25.4, 0.2};
    float p3[] = {1.0, 23.0, 25.0, 0.26};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p3, 1);
}

void testNotEqualFloatArrays1(void)
{
    float p0[] = {1.0, 8.0, 25.4, 0.253};
    float p1[] = {1.0, 8.0, 25.4, 0.252};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualFloatArrays2(void)
{
    float p0[] = {1.0, 8.0, 25.4, 0.253};
    float p1[] = {2.0, 8.0, 25.4, 0.253};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualFloatArrays3(void)
{
    float p0[] = {1.0, 8.0, 25.4, 0.253};
    float p1[] = {1.0, 8.0, 25.5, 0.253};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testEqualMemoryArrays(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    int p2[] = {1, 8, 987, 2};
    int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p0, 4, 1);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p0, 4, 4);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, 4, 4);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p2, 4, 3);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p3, 4, 1);
}

void testNotEqualMemoryArrays1(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, 4, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualMemoryArrays2(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, 4, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
}

void testNotEqualMemoryArrays3(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};

    int failed;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, 4, 4);
    EXPECT_ABORT_END

    failed = Unity.CurrentTestFailed;
    Unity.CurrentTestFailed = 0;

    VERIFY_FAILURE_WAS_CAUGHT
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

