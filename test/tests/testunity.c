/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include <setjmp.h>
#include "unity.h"
#include <string.h>

// Dividing by these constants produces +/- infinity.
// The rationale is given in UnityAssertFloatIsInf's body.
#ifndef UNITY_EXCLUDE_FLOAT
static const _UF f_zero = 0.0f;
#endif

#ifndef UNITY_EXCLUDE_DOUBLE
static const _UD d_zero = 0.0;
#endif

#define EXPECT_ABORT_BEGIN \
    if (TEST_PROTECT())    \
    {

#define VERIFY_FAILS_END                                                       \
    }                                                                          \
    Unity.CurrentTestFailed = (Unity.CurrentTestFailed == 1) ? 0 : 1;          \
    if (Unity.CurrentTestFailed == 1) {                                        \
      SetToOneMeanWeAlreadyCheckedThisGuy = 1;                                 \
      UnityPrintNumberUnsigned(Unity.CurrentTestLineNumber);                   \
      UNITY_OUTPUT_CHAR(':');                                                  \
      UnityPrint(Unity.CurrentTestName);                                       \
      UnityPrint("[[[[ Previous Test Should Have Failed But Did Not ]]]]");    \
      UNITY_OUTPUT_CHAR('\n');                                                 \
    }

#define VERIFY_IGNORES_END                                                     \
    }                                                                          \
    Unity.CurrentTestFailed = (Unity.CurrentTestIgnored == 1) ? 0 : 1;         \
    Unity.CurrentTestIgnored = 0;                                              \
    if (Unity.CurrentTestFailed == 1) {                                        \
      SetToOneMeanWeAlreadyCheckedThisGuy = 1;                                 \
      UnityPrint("[[[[ Previous Test Should Have Ignored But Did Not ]]]]");   \
      UNITY_OUTPUT_CHAR('\n');                                                 \
    }

int SetToOneToFailInTearDown;
int SetToOneMeanWeAlreadyCheckedThisGuy;

void setUp(void)
{
  SetToOneToFailInTearDown = 0;
  SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}

void tearDown(void)
{
  if (SetToOneToFailInTearDown == 1)
    TEST_FAIL_MESSAGE("<= Failed in tearDown");
  if ((SetToOneMeanWeAlreadyCheckedThisGuy == 0) && (Unity.CurrentTestFailed > 0))
  {
    UnityPrint("[[[[ Previous Test Should Have Passed But Did Not ]]]]");
    UNITY_OUTPUT_CHAR('\n');
  }
}

void testUnitySizeInitializationReminder(void)
{
    /* This test ensures that sizeof(struct _Unity) doesn't change. If this
     * test breaks, go look at the initialization of the Unity global variable
     * in unity.c and make sure we're filling in the proper fields. */
    const char* message = "Unexpected size for _Unity struct. Please check that "
                     "the initialization of the Unity symbol in unity.c is "
                     "still correct.";

    /* Define a structure with all the same fields as `struct _Unity`. */
#ifdef UNITY_EXCLUDE_DETAILS
    struct {
        const char* TestFile;
        const char* CurrentTestName;
        UNITY_LINE_TYPE CurrentTestLineNumber;
        UNITY_COUNTER_TYPE NumberOfTests;
        UNITY_COUNTER_TYPE TestFailures;
        UNITY_COUNTER_TYPE TestIgnores;
        UNITY_COUNTER_TYPE CurrentTestFailed;
        UNITY_COUNTER_TYPE CurrentTestIgnored;
        jmp_buf AbortFrame;
    } _Expected_Unity;
#else
    struct {
        const char* TestFile;
        const char* CurrentTestName;
        const char* CurrentDetails1;
        const char* CurrentDetails2;
        UNITY_LINE_TYPE CurrentTestLineNumber;
        UNITY_COUNTER_TYPE NumberOfTests;
        UNITY_COUNTER_TYPE TestFailures;
        UNITY_COUNTER_TYPE TestIgnores;
        UNITY_COUNTER_TYPE CurrentTestFailed;
        UNITY_COUNTER_TYPE CurrentTestIgnored;
        jmp_buf AbortFrame;
    } _Expected_Unity;
#endif

    /* Compare our fake structure's size to the actual structure's size. They
     * should be the same.
     *
     * This accounts for alignment, padding, and packing issues that might come
     * up between different architectures. */
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(_Expected_Unity), sizeof(Unity), message);
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
    EXPECT_ABORT_BEGIN
    TEST_ASSERT(0);
    VERIFY_FAILS_END
}

void testNotTrue(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(0);
    VERIFY_FAILS_END
}

void testNotFalse(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FALSE(1);
    VERIFY_FAILS_END
}

void testNotUnless(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UNLESS(1);
    VERIFY_FAILS_END
}

void testNotNotEqual(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL(10, 10);
    VERIFY_FAILS_END
}

void testFail(void)
{
    EXPECT_ABORT_BEGIN
    TEST_FAIL_MESSAGE("Expected for testing");
    VERIFY_FAILS_END
}

void testIsNull(void)
{
    char* ptr1 = NULL;
    const char* ptr2 = "hello";

    TEST_ASSERT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
}

void testIsNullShouldFailIfNot(void)
{
    const char* ptr1 = "hello";

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NULL(ptr1);
    VERIFY_FAILS_END
}

void testNotNullShouldFailIfNULL(void)
{
    char* ptr1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_NULL(ptr1);
    VERIFY_FAILS_END
}

void testIgnore(void)
{
    EXPECT_ABORT_BEGIN
    TEST_IGNORE();
    TEST_FAIL_MESSAGE("This should not be reached");
    VERIFY_IGNORES_END
}

void testIgnoreMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_IGNORE_MESSAGE("This is an expected TEST_IGNORE_MESSAGE string!");
    TEST_FAIL_MESSAGE("This should not be reached");
    VERIFY_IGNORES_END
}

void testNotEqualInts(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(3982, 3983);
    VERIFY_FAILS_END
}

void testNotEqualInt8s(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT8(-127, -126);
    VERIFY_FAILS_END
}

void testNotEqualInt16s(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT16(-16383, -16382);
    VERIFY_FAILS_END
}

void testNotEqualInt32s(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT32(-2147483647, -2147483648); //use largest 32 bit negative to test printability
    VERIFY_FAILS_END
}

void testNotEqualBits(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS(0xFF00, 0x5555, 0x5A55);
    VERIFY_FAILS_END
}

void testNotEqualUInts(void)
{
    _UU16 v0, v1;

    v0 = 9000;
    v1 = 9001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualUInt8s(void)
{
    _UU8 v0, v1;

    v0 = 254;
    v1 = 255;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualUInt16s(void)
{
    _UU16 v0, v1;

    v0 = 65535;
    v1 = 65534;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualUInt32s(void)
{
    _UU32 v0, v1;

    v0 = 4294967295;
    v1 = 4294967294;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex8s(void)
{
    _UU8 v0, v1;

    v0 = 0x23;
    v1 = 0x22;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex8sIfSigned(void)
{
    _US8 v0, v1;

    v0 = -2;
    v1 = 2;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex16s(void)
{
    _UU16 v0, v1;

    v0 = 0x1234;
    v1 = 0x1235;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex16sIfSigned(void)
{
    _US16 v0, v1;

    v0 = -1024;
    v1 = -1028;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex32s(void)
{
    _UU32 v0, v1;

    v0 = 900000;
    v1 = 900001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex32sIfSigned(void)
{
    _US32 v0, v1;

    v0 = -900000;
    v1 = 900001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END
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

void testEqualInt8s(void)
{
    _US8 v0, v1;
    _US8 *p0, *p1;

    v0 = 0x22;
    v1 = 0x22;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT8(0x22, 0x22);
    TEST_ASSERT_EQUAL_INT8(v0, v1);
    TEST_ASSERT_EQUAL_INT8(0x22, v1);
    TEST_ASSERT_EQUAL_INT8(v0, 0x22);
    TEST_ASSERT_EQUAL_INT8(*p0, v1);
    TEST_ASSERT_EQUAL_INT8(*p0, *p1);
    TEST_ASSERT_EQUAL_INT8(*p0, 0x22);
}

void testEqualInt8sWhenThereAreDifferencesOutside8Bits(void)
{
    TEST_ASSERT_EQUAL_INT8(0x321,0x421);
    TEST_ASSERT_EQUAL_INT8(0xFF21,0x0021);
}

void testEqualInt16s(void)
{
    _US16 v0, v1;
    _US16 *p0, *p1;

    v0 = 0x7876;
    v1 = 0x7876;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT16(0x7876, 0x7876);
    TEST_ASSERT_EQUAL_INT16(v0, v1);
    TEST_ASSERT_EQUAL_INT16(0x7876, v1);
    TEST_ASSERT_EQUAL_INT16(v0, 0x7876);
    TEST_ASSERT_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_EQUAL_INT16(*p0, *p1);
    TEST_ASSERT_EQUAL_INT16(*p0, 0x7876);
}

void testEqualInt16sNegatives(void)
{
    _US16 v0, v1;
    _US16 *p0, *p1;

    v0 = -7876;
    v1 = -7876;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT16(-7876, -7876);
    TEST_ASSERT_EQUAL_INT16(v0, v1);
    TEST_ASSERT_EQUAL_INT16(-7876, v1);
    TEST_ASSERT_EQUAL_INT16(v0, -7876);
    TEST_ASSERT_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_EQUAL_INT16(*p0, *p1);
    TEST_ASSERT_EQUAL_INT16(*p0, -7876);
}

void testEqualInt16sWhenThereAreDifferencesOutside16Bits(void)
{
    TEST_ASSERT_EQUAL_INT16(0x54321,0x64321);
    TEST_ASSERT_EQUAL_INT16(0xFFFF4321,0x00004321);
}

void testEqualInt32s(void)
{
    _US32 v0, v1;
    _US32 *p0, *p1;

    v0 = 0x78760000;
    v1 = 0x78760000;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT32(0x78760000, 0x78760000);
    TEST_ASSERT_EQUAL_INT32(v0, v1);
    TEST_ASSERT_EQUAL_INT32(0x78760000, v1);
    TEST_ASSERT_EQUAL_INT32(v0, 0x78760000);
    TEST_ASSERT_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_EQUAL_INT32(*p0, *p1);
    TEST_ASSERT_EQUAL_INT32(*p0, 0x78760000);
}

void testEqualInt32sNegatives(void)
{
    _US32 v0, v1;
    _US32 *p0, *p1;

    v0 = -123456789;
    v1 = -123456789;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT32(-123456789, -123456789);
    TEST_ASSERT_EQUAL_INT32(v0, v1);
    TEST_ASSERT_EQUAL_INT32(-123456789, v1);
    TEST_ASSERT_EQUAL_INT32(v0, -123456789);
    TEST_ASSERT_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_EQUAL_INT32(*p0, *p1);
    TEST_ASSERT_EQUAL_INT32(*p0, -123456789);
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


void testEqualUint8s(void)
{
    _UU8 v0, v1;
    _UU8 *p0, *p1;

    v0 = 0x22;
    v1 = 0x22;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_UINT8(0x22, 0x22);
    TEST_ASSERT_EQUAL_UINT8(v0, v1);
    TEST_ASSERT_EQUAL_UINT8(0x22, v1);
    TEST_ASSERT_EQUAL_UINT8(v0, 0x22);
    TEST_ASSERT_EQUAL_UINT8(*p0, v1);
    TEST_ASSERT_EQUAL_UINT8(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT8(*p0, 0x22);
}

void testEqualUint8sWhenThereAreDifferencesOutside8Bits(void)
{
    TEST_ASSERT_EQUAL_UINT8(0x321,0x421);
    TEST_ASSERT_EQUAL_UINT8(0xFF21,0x0021);
}

void testEqualUint16s(void)
{
    _UU16 v0, v1;
    _UU16 *p0, *p1;

    v0 = 0x9876;
    v1 = 0x9876;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_UINT16(0x9876, 0x9876);
    TEST_ASSERT_EQUAL_UINT16(v0, v1);
    TEST_ASSERT_EQUAL_UINT16(0x9876, v1);
    TEST_ASSERT_EQUAL_UINT16(v0, 0x9876);
    TEST_ASSERT_EQUAL_UINT16(*p0, v1);
    TEST_ASSERT_EQUAL_UINT16(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT16(*p0, 0x9876);
}

void testEqualUint16sWhenThereAreDifferencesOutside16Bits(void)
{
    TEST_ASSERT_EQUAL_UINT16(0x54321,0x64321);
    TEST_ASSERT_EQUAL_UINT16(0xFFFF4321,0x00004321);
}

void testEqualUint32s(void)
{
    _UU32 v0, v1;
    _UU32 *p0, *p1;

    v0 = 0x98760000;
    v1 = 0x98760000;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_UINT32(0x98760000, 0x98760000);
    TEST_ASSERT_EQUAL_UINT32(v0, v1);
    TEST_ASSERT_EQUAL_UINT32(0x98760000, v1);
    TEST_ASSERT_EQUAL_UINT32(v0, 0x98760000);
    TEST_ASSERT_EQUAL_UINT32(*p0, v1);
    TEST_ASSERT_EQUAL_UINT32(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT32(*p0, 0x98760000);
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

void testEqualHex8sWhenThereAreDifferencesOutside8Bits(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x321,0x421);
    TEST_ASSERT_EQUAL_HEX8(0xFF21,0x0021);
}

void testEqualHex8sNegatives(void)
{
    _UU8 v0, v1;
    _UU8 *p0, *p1;

    v0 = 0xDD;
    v1 = 0xDD;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_HEX8(0xDD, 0xDD);
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    TEST_ASSERT_EQUAL_HEX8(0xDD, v1);
    TEST_ASSERT_EQUAL_HEX8(v0, 0xDD);
    TEST_ASSERT_EQUAL_HEX8(*p0, v1);
    TEST_ASSERT_EQUAL_HEX8(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX8(*p0, 0xDD);
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

void testEqualHex16sWhenThereAreDifferencesOutside16Bits(void)
{
    TEST_ASSERT_EQUAL_HEX16(0x54321,0x64321);
    TEST_ASSERT_EQUAL_HEX16(0xFFFF4321,0x00004321);
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

void testNotEqualBitHigh(void)
{
    _UU32 v0 = 0x7F55AA00;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BIT_HIGH(31, v0);
    VERIFY_FAILS_END
}

void testNotEqualBitLow(void)
{
    _UU32 v0 = 0xFF55AA00;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BIT_LOW(30, v0);
    VERIFY_FAILS_END
}

void testNotEqualBitsHigh(void)
{
    _UU32 v0 = 0xFF55AA00;
    _UU32 v1 = 0x55550000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_HIGH(v0, v1);
    VERIFY_FAILS_END

}

void testNotEqualBitsLow(void)
{
    _UU32 v0 = 0xFF55AA00;
    _UU32 v1 = 0x55550000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_LOW(v0, v1);
    VERIFY_FAILS_END

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

    TEST_ASSERT_EQUAL_PTR(p0, &v0);
    TEST_ASSERT_EQUAL_PTR(&v1, p1);
    TEST_ASSERT_EQUAL_PTR(p2, p1);
    TEST_ASSERT_EQUAL_PTR(&v0, &v0);
}

void testNotEqualPointers(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR(0x12345678, 0x12345677);
    VERIFY_FAILS_END
}

void testIntsWithinDelta(void)
{
    TEST_ASSERT_INT_WITHIN(1, 5000, 5001);
    TEST_ASSERT_INT_WITHIN(5, 5000, 4996);
    TEST_ASSERT_INT_WITHIN(5, 5000, 5005);
    TEST_ASSERT_INT_WITHIN(500, 50, -440);

    TEST_ASSERT_INT_WITHIN(2, -1, -1);
    TEST_ASSERT_INT_WITHIN(5, 1, -1);
    TEST_ASSERT_INT_WITHIN(5, -1, 1);
}

void testIntsWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_INT_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 5000, 4996, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 5000, 5005, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(500, 50, -440, "Custom Message.");

    TEST_ASSERT_INT_WITHIN_MESSAGE(2, -1, -1, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, -1, 1, "Custom Message.");
}

void testIntsNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_WITHIN(5, 5000, 5006);
    VERIFY_FAILS_END
}

void testIntsNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 5000, 5006, "Custom Message.");
    VERIFY_FAILS_END
}

void testUIntsWithinDelta(void)
{
    TEST_ASSERT_UINT_WITHIN(1, 5000, 5001);
    TEST_ASSERT_UINT_WITHIN(5, 5000, 4996);
    TEST_ASSERT_UINT_WITHIN(5, 5000, 5005);
}

void testUIntsWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_UINT_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 5000, 4996, "Custom Message.");
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 5000, 5005, "Custom Message.");
}

void testUIntsNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(1, 2147483647u, 2147483649u);
    VERIFY_FAILS_END
}

void testUIntsNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(5, -1, 1);
    VERIFY_FAILS_END
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, -1, 1, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX32sWithinDelta(void)
{
    TEST_ASSERT_HEX32_WITHIN(1, 5000, 5001);
    TEST_ASSERT_HEX32_WITHIN(5, 5000, 4996);
    TEST_ASSERT_HEX32_WITHIN(5, 5000, 5005);
}

void testHEX32sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(5, 5000, 4996, "Custom Message.");
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(5, 5000, 5005, "Custom Message.");
}

void testHEX32sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN(1, 2147483647u, 2147483649u);
    VERIFY_FAILS_END
}

void testHEX32sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX32sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
}

void testHEX32sNotWithinDeltaEvenThoughASignedIntWouldPassAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX16sWithinDelta(void)
{
    TEST_ASSERT_HEX16_WITHIN(1, 5000, 5001);
    TEST_ASSERT_HEX16_WITHIN(5, 5000, 4996);
    TEST_ASSERT_HEX16_WITHIN(5, 5000, 5005);
}

void testHEX16sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(5, 5000, 4996, "Custom Message.");
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(5, 5000, 5005, "Custom Message.");
}

void testHEX16sWithinDeltaWhenThereAreDifferenceOutsideOf16Bits(void)
{
    TEST_ASSERT_HEX16_WITHIN(5, 0x54321, 0x44321);
}

void testHEX16sWithinDeltaWhenThereAreDifferenceOutsideOf16BitsAndCustomMessage(void)
{
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(5, 0x54321, 0x44321, "Custom Message.");
}

void testHEX16sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN(2, 65535, 0);
    VERIFY_FAILS_END
}

void testHEX16sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(2, 65535, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testHEX8sWithinDelta(void)
{
    TEST_ASSERT_HEX8_WITHIN(1, 254, 255);
    TEST_ASSERT_HEX8_WITHIN(5, 251, 255);
    TEST_ASSERT_HEX8_WITHIN(5, 1, 4);
}

void testHEX8sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(1, 254, 255, "Custom Message.");
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(5, 251, 255, "Custom Message.");
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(5, 1, 4, "Custom Message.");
}

void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits(void)
{
    TEST_ASSERT_HEX8_WITHIN(5, 0x123, 0xF23);
}

void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage(void)
{
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(5, 0x123, 0xF23, "Custom Message.");
}

void testHEX8sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN(2, 255, 0);
    VERIFY_FAILS_END
}

void testHEX8sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(2, 255, 0, "Custom Message.");
    VERIFY_FAILS_END
}

//-----------------

void testUINT32sWithinDelta(void)
{
    TEST_ASSERT_UINT32_WITHIN(1, 5000, 5001);
    TEST_ASSERT_UINT32_WITHIN(5, 5000, 4996);
    TEST_ASSERT_UINT32_WITHIN(5, 5000, 5005);
}

void testUINT32sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, 5000, 4996, "Custom Message.");
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, 5000, 5005, "Custom Message.");
}

void testUINT32sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(1, 2147483647u, 2147483649u);
    VERIFY_FAILS_END
}

void testUINT32sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END
}

void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
}

void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END
}

void testUINT16sWithinDelta(void)
{
    TEST_ASSERT_UINT16_WITHIN(1, 5000, 5001);
    TEST_ASSERT_UINT16_WITHIN(5, 5000, 4996);
    TEST_ASSERT_UINT16_WITHIN(5, 5000, 5005);
}

void testUINT16sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(5, 5000, 4996, "Custom Message.");
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(5, 5000, 5005, "Custom Message.");
}

void testUINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16Bits(void)
{
    TEST_ASSERT_UINT16_WITHIN(5, 0x54321, 0x44321);
}

void testUINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16BitsAndCustomMessage(void)
{
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(5, 0x54321, 0x44321, "Custom Message.");
}

void testUINT16sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_WITHIN(2, 65535, 0);
    VERIFY_FAILS_END
}

void testUINT16sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(2, 65535, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testUINT8sWithinDelta(void)
{
    TEST_ASSERT_UINT8_WITHIN(1, 254, 255);
    TEST_ASSERT_UINT8_WITHIN(5, 251, 255);
    TEST_ASSERT_UINT8_WITHIN(5, 1, 4);
}

void testUINT8sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, 254, 255, "Custom Message.");
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(5, 251, 255, "Custom Message.");
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(5, 1, 4, "Custom Message.");
}

void testUINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits(void)
{
    TEST_ASSERT_UINT8_WITHIN(5, 0x123, 0xF23);
}

void testUINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage(void)
{
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(5, 0x123, 0xF23, "Custom Message.");
}

void testUINT8sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_WITHIN(2, 255, 0);
    VERIFY_FAILS_END
}

void testUINT8sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(2, 255, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testINT32sWithinDelta(void)
{
    TEST_ASSERT_INT32_WITHIN(1, 5000, 5001);
    TEST_ASSERT_INT32_WITHIN(5, 1, -2);
    TEST_ASSERT_INT32_WITHIN(5, -2, 1);
}

void testINT32sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_INT32_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
}

void testINT32sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN(1, -3, 1);
    VERIFY_FAILS_END
}

void testINT32sNotWithinDeltaAndDifferenceOverflows(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN(1, -1, 0x7FFFFFFF);
    VERIFY_FAILS_END
}
void testINT32sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN_MESSAGE(1, -2, 1, "Custom Message.");
    VERIFY_FAILS_END
}

void testINT16sWithinDelta(void)
{
    TEST_ASSERT_INT16_WITHIN(1, 5000, 5001);
    TEST_ASSERT_INT16_WITHIN(5, 2, -2);
    TEST_ASSERT_INT16_WITHIN(5, -2, 2);
}

void testINT16sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_INT16_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
}

void testINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16Bits(void)
{
    TEST_ASSERT_INT16_WITHIN(5, 0x54321, 0x44321);
}

void testINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16BitsAndCustomMessage(void)
{
    TEST_ASSERT_INT16_WITHIN_MESSAGE(5, 0x54321, 0x44321, "Custom Message.");
}

void testINT16sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_WITHIN(2, 4, -2);
    VERIFY_FAILS_END
}

void testINT16sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_WITHIN_MESSAGE(2, 3, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testINT8sWithinDelta(void)
{
    TEST_ASSERT_INT8_WITHIN(1, 127, 126);
    TEST_ASSERT_INT8_WITHIN(5, -2, 2);
    TEST_ASSERT_INT8_WITHIN(5, 2, -2);
}

void testINT8sWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_INT8_WITHIN_MESSAGE(5, 1, 4, "Custom Message.");
}

void testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits(void)
{
    TEST_ASSERT_INT8_WITHIN(5, 0x123, 0xF23);
}

void testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage(void)
{
    TEST_ASSERT_INT8_WITHIN_MESSAGE(5, 0x123, 0xF23, "Custom Message.");
}

void testINT8sNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_WITHIN(2, -3, 0);
    VERIFY_FAILS_END
}

void testINT8sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_WITHIN_MESSAGE(2, -4, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testEqualStrings(void)
{
    const char *testString = "foo";

    TEST_ASSERT_EQUAL_STRING(testString, testString);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("foo", "foo", "foo isn't foo");
    TEST_ASSERT_EQUAL_STRING("foo", testString);
    TEST_ASSERT_EQUAL_STRING(testString, "foo");
    TEST_ASSERT_EQUAL_STRING("", "");
}

void testEqualStringsLen(void)
{
    const char *testString = "foobar";
    TEST_ASSERT_EQUAL_STRING_LEN(testString, testString, strlen(testString));
    TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE("foobar", "foobaz", 5, "fooba isn't fooba");
    TEST_ASSERT_EQUAL_STRING_LEN("foo", testString, 3);
    TEST_ASSERT_EQUAL_STRING_LEN(testString, "foo", 3);
    TEST_ASSERT_EQUAL_STRING_LEN("", "", 3);
}

void testEqualStringsWithCarriageReturnsAndLineFeeds(void)
{
    const char *testString = "foo\r\nbar";

    TEST_ASSERT_EQUAL_STRING(testString, testString);
    TEST_ASSERT_EQUAL_STRING("foo\r\nbar", "foo\r\nbar");
    TEST_ASSERT_EQUAL_STRING("foo\r\nbar", testString);
    TEST_ASSERT_EQUAL_STRING(testString, "foo\r\nbar");
    TEST_ASSERT_EQUAL_STRING("", "");
}

void testNotEqualString1(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "bar");
    VERIFY_FAILS_END
}

void testNotEqualStringLen1(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foobar", "foobaz", 6);
    VERIFY_FAILS_END
}

void testNotEqualString2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "");
    VERIFY_FAILS_END
}

void testNotEqualStringLen2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foo", "", 3);
    VERIFY_FAILS_END
}

void testNotEqualString3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("", "bar");
    VERIFY_FAILS_END
}

void testNotEqualStringLen3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("", "bar", 3);
    VERIFY_FAILS_END
}

void testNotEqualString4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("bar\r", "bar\n");
    VERIFY_FAILS_END
}

void testNotEqualStringLen4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("bar\r", "bar\n", 4);
    VERIFY_FAILS_END
}

void testNotEqualString5(void)
{
    const char str1[] = { 0x41, 0x42, 0x03, 0x00 };
    const char str2[] = { 0x41, 0x42, 0x04, 0x00 };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(str1, str2);
    VERIFY_FAILS_END
}

void testNotEqualString_ExpectedStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(NULL, "bar");
    VERIFY_FAILS_END
}

void testNotEqualString_ActualStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", NULL);
    VERIFY_FAILS_END
}

void testEqualStringArrays(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, expStrings, 3);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 3);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 2);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 1);
}

void testNotEqualStringArray1(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray2(void)
{
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", "boo", "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray3(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", NULL };
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray4(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", NULL, "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray5(void)
{
    const char **testStrings = NULL;
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testNotEqualStringArray6(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char **expStrings = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

void testEqualStringArrayIfBothNulls(void)
{
    const char **testStrings = NULL;
    const char **expStrings = NULL;

    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
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
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("foo", "bar", 3);
    VERIFY_FAILS_END
}

void testNotEqualMemory2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", "food", 4);
    VERIFY_FAILS_END
}

void testNotEqualMemory3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY(NULL, "food", 4);
    VERIFY_FAILS_END
}

void testNotEqualMemory4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", NULL, 4);
    VERIFY_FAILS_END
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

void testNotEqualIntArraysNullExpected(void)
{
    int* p0 = NULL;
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArraysNullActual(void)
{
    int* p1 = NULL;
    int p0[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArrays1(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArrays2(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualIntArrays3(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualPtrArrays(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&A, &B, &C};
    char* p1[] = {&A, &B, &C, &A};
    char* p2[] = {&A, &B};
    char* p3[] = {&A};

    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p0, 3);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 3);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p1, p2, 2);
    TEST_ASSERT_EQUAL_PTR_ARRAY(p3, p0, 1);
}

void testNotEqualPtrArraysNullExpected(void)
{
    char A = 1;
    char B = 2;
    char** p0 = NULL;
    char* p1[] = {&A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 2);
    VERIFY_FAILS_END
}

void testNotEqualPtrArraysNullActual(void)
{
    char A = 1;
    char B = 2;
    char** p0 = NULL;
    char* p1[] = {&A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p1, p0, 2);
    VERIFY_FAILS_END
}

void testNotEqualPtrArrays1(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&A, &B, &C, &B};
    char* p1[] = {&A, &B, &C, &A};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualPtrArrays2(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&B, &B, &C, &A};
    char* p1[] = {&A, &B, &C, &A};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualPtrArrays3(void)
{
    char A = 1;
    char B = 2;
    char C = 3;
    char* p0[] = {&A, &B, &B, &A};
    char* p1[] = {&A, &B, &C, &A};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualInt8Arrays(void)
{
    _US8 p0[] = {1, 8, 117, -2};
    _US8 p1[] = {1, 8, 117, -2};
    _US8 p2[] = {1, 8, 117, 2};
    _US8 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p3, 1);
}

void testNotEqualInt8Arrays(void)
{
    _US8 p0[] = {1, 8, 36, -2};
    _US8 p1[] = {1, 8, 36, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
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

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUIntArrays2(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUIntArrays3(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualInt16Arrays(void)
{
    _US16 p0[] = {1, 8, 117, 3};
    _US16 p1[] = {1, 8, 117, 3};
    _US16 p2[] = {1, 8, 117, 2};
    _US16 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p3, 1);
}

void testNotEqualInt16Arrays(void)
{
    _US16 p0[] = {1, 8, 127, 3};
    _US16 p1[] = {1, 8, 127, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualInt32Arrays(void)
{
    _US32 p0[] = {1, 8, 117, 3};
    _US32 p1[] = {1, 8, 117, 3};
    _US32 p2[] = {1, 8, 117, 2};
    _US32 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p3, 1);
}

void testNotEqualInt32Arrays(void)
{
    _US32 p0[] = {1, 8, 127, 3};
    _US32 p1[] = {1, 8, 127, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualUINT8Arrays(void)
{
    _UU8 p0[] = {1, 8, 100, 127};
    _UU8 p1[] = {1, 8, 100, 127};
    _UU8 p2[] = {1, 8, 100, 2};
    _UU8 p3[] = {1, 50, 60, 70};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p3, 1);
}

void testNotEqualUINT8Arrays1(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT8Arrays2(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT8Arrays3(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}


void testEqualUINT16Arrays(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65132u};
    unsigned short p2[] = {1, 8, 987, 2};
    unsigned short p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p3, 1);
}

void testNotEqualUINT16Arrays1(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT16Arrays2(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT16Arrays3(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualUINT32Arrays(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 987, 65132u};
    _UU32 p2[] = {1, 8, 987, 2};
    _UU32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p3, 1);
}

void testNotEqualUINT32Arrays1(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT32Arrays2(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualUINT32Arrays3(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualHEXArrays(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 987, 65132u};
    _UU32 p2[] = {1, 8, 987, 2};
    _UU32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p3, 1);
}

void testNotEqualHEXArrays1(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEXArrays2(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEXArrays3(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualHEX32Arrays(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 987, 65132u};
    _UU32 p2[] = {1, 8, 987, 2};
    _UU32 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p3, 1);
}

void testNotEqualHEX32Arrays1(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX32Arrays2(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX32Arrays3(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
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

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX16Arrays2(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX16Arrays3(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
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

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX8Arrays2(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {2, 8, 254u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testNotEqualHEX8Arrays3(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 255u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

void testEqualMemoryArrays(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    int p2[] = {1, 8, 987, 2};
    int p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p0, sizeof(int), 1);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p0, sizeof(int), 4);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p2, sizeof(int), 3);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p3, sizeof(int), 1);
}

void testNotEqualMemoryArraysExpectedNull(void)
{
    int* p0 = NULL;
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END
}

void testNotEqualMemoryArraysActualNull(void)
{
    int p0[] = {1, 8, 987, -2};
    int* p1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END
}

void testNotEqualMemoryArrays1(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END
}

void testNotEqualMemoryArrays2(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END
}

void testNotEqualMemoryArrays3(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END
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

void testIgnoredAndThenFailInTearDown(void)
{
    SetToOneToFailInTearDown = 1;
    TEST_IGNORE();
}

// Tricky series of macros to set USING_OUTPUT_SPY
#define USING_SPY_AS(a)           EXPAND_AND_USE_2ND(ASSIGN_VALUE(a), 0)
#define ASSIGN_VALUE(a)           VAL_FUNC_##a
#define VAL_FUNC_putcharSpy()     0, 1
#define EXPAND_AND_USE_2ND(a, b)  SECOND_PARAM(a, b, throwaway)
#define SECOND_PARAM(a, b, ...)   b
#if USING_SPY_AS(UNITY_OUTPUT_CHAR())
  #define USING_OUTPUT_SPY // true only if UNITY_OUTPUT_CHAR = putchar_Spy
#endif

#ifdef USING_OUTPUT_SPY
#include <stdio.h>
#define SPY_BUFFER_MAX 40
static char putcharSpyBuffer[SPY_BUFFER_MAX];
#endif
static int indexSpyBuffer;
static int putcharSpyEnabled;

void startPutcharSpy(void) {indexSpyBuffer = 0; putcharSpyEnabled = 1;}

void endPutcharSpy(void) {putcharSpyEnabled = 0;}

char* getBufferPutcharSpy(void)
{
#ifdef USING_OUTPUT_SPY
    putcharSpyBuffer[indexSpyBuffer] = '\0';
    return putcharSpyBuffer;
#else
    return NULL;
#endif
}

int putcharSpy(int c)
{
#ifdef USING_OUTPUT_SPY
    if (putcharSpyEnabled)
    {
        if (indexSpyBuffer < SPY_BUFFER_MAX - 1)
            putcharSpyBuffer[indexSpyBuffer++] = (char)c;
    } else
        c = putchar(c);
#endif
    return c;
}

#define TEST_ASSERT_EQUAL_PRINT_NUMBERS(expected, actual) {             \
        startPutcharSpy(); UnityPrintNumber((actual)); endPutcharSpy(); \
        TEST_ASSERT_EQUAL_STRING((expected), getBufferPutcharSpy());    \
        }

#define TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS(expected, actual) {            \
        startPutcharSpy(); UnityPrintNumberUnsigned((actual)); endPutcharSpy(); \
        TEST_ASSERT_EQUAL_STRING((expected), getBufferPutcharSpy());            \
        }

void testPrintNumbers32(void)
{
#ifndef USING_OUTPUT_SPY
    TEST_IGNORE_MESSAGE("Compile with '-D UNITY_OUTPUT_CHAR=putcharSpy' to enable print testing");
#else
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("0", 0);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("1", 1);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-1", -1);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("2000000000", 2000000000);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-2147483648", (_US32)0x80000000);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-1",          (_US32)0xFFFFFFFF);
#endif
}

void testPrintNumbersUnsigned32(void)
{
#ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("0", 0);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("1", 1);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("1500000000", 1500000000);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("2147483648", (_UU32)0x80000000);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("4294967295", (_UU32)0xFFFFFFFF);
#endif
}

// ===================== THESE TEST WILL RUN IF YOUR CONFIG INCLUDES 64 BIT SUPPORT ==================

void testPrintNumbersInt64(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
  #ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
  #else
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("0", 0);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("10000000000", 10000000000);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-9223372036854775808", (_U_SINT)0x8000000000000000);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-1", (_U_SINT)0xFFFFFFFFFFFFFFFF);
  #endif
#endif
}

void testPrintNumbersUInt64(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
  #ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
  #else
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("0", 0);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("70000000000", 70000000000);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("9223372036854775808",  (_U_UINT)0x8000000000000000);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("18446744073709551615", (_U_UINT)0xFFFFFFFFFFFFFFFF);
  #endif
#endif
}

void testEqualHex64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 v0, v1;
    _UU64 *p0, *p1;

    v0 = 0x9876543201234567;
    v1 = 0x9876543201234567;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_HEX64(0x9876543201234567, 0x9876543201234567);
    TEST_ASSERT_EQUAL_HEX64(v0, v1);
    TEST_ASSERT_EQUAL_HEX64(0x9876543201234567, v1);
    TEST_ASSERT_EQUAL_HEX64(v0, 0x9876543201234567);
    TEST_ASSERT_EQUAL_HEX64(*p0, v1);
    TEST_ASSERT_EQUAL_HEX64(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX64(*p0, 0x9876543201234567);
#endif
}

void testEqualUint64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 v0, v1;
    _UU64 *p0, *p1;

    v0 = 0x9876543201234567;
    v1 = 0x9876543201234567;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_UINT64(0x9876543201234567, 0x9876543201234567);
    TEST_ASSERT_EQUAL_UINT64(v0, v1);
    TEST_ASSERT_EQUAL_UINT64(0x9876543201234567, v1);
    TEST_ASSERT_EQUAL_UINT64(v0, 0x9876543201234567);
    TEST_ASSERT_EQUAL_UINT64(*p0, v1);
    TEST_ASSERT_EQUAL_UINT64(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT64(*p0, 0x9876543201234567);
#endif
}

void testEqualInt64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _US64 v0, v1;
    _US64 *p0, *p1;

    v0 = (_US64)0x9876543201234567;
    v1 = (_US64)0x9876543201234567;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT64(0x9876543201234567, 0x9876543201234567);
    TEST_ASSERT_EQUAL_INT64(v0, v1);
    TEST_ASSERT_EQUAL_INT64(0x9876543201234567, v1);
    TEST_ASSERT_EQUAL_INT64(v0, 0x9876543201234567);
    TEST_ASSERT_EQUAL_INT64(*p0, v1);
    TEST_ASSERT_EQUAL_INT64(*p0, *p1);
    TEST_ASSERT_EQUAL_INT64(*p0, 0x9876543201234567);
#endif
}


void testNotEqualHex64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 v0, v1;

    v0 = 9000000000;
    v1 = 9100000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testNotEqualUint64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 v0, v1;

    v0 = 9000000000;
    v1 = 9100000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testNotEqualInt64s(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _US64 v0, v1;

    v0 = -9000000000;
    v1 = 9100000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testNotEqualHex64sIfSigned(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _US64 v0, v1;

    v0 = -9000000000;
    v1 = 9000000000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64(v0, v1);
    VERIFY_FAILS_END
#endif
}

void testHEX64sWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    TEST_ASSERT_HEX64_WITHIN(1, 0x7FFFFFFFFFFFFFFF,0x7FFFFFFFFFFFFFFE);
    TEST_ASSERT_HEX64_WITHIN(5, 5000, 4996);
    TEST_ASSERT_HEX64_WITHIN(5, 5000, 5005);
#endif
}

void testHEX64sNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_WITHIN(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC);
    VERIFY_FAILS_END
#endif
}

void testHEX64sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
#endif
}

void testUINT64sWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    TEST_ASSERT_UINT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF,0x7FFFFFFFFFFFFFFE);
    TEST_ASSERT_UINT64_WITHIN(5, 5000, 4996);
    TEST_ASSERT_UINT64_WITHIN(5, 5000, 5005);
#endif
}

void testUINT64sNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC);
    VERIFY_FAILS_END
#endif
}

void testUINT64sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
#endif
}

void testINT64sWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    TEST_ASSERT_INT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF,0x7FFFFFFFFFFFFFFE);
    TEST_ASSERT_INT64_WITHIN(5, 5000, 4996);
    TEST_ASSERT_INT64_WITHIN(5, 5000, 5005);
#endif
}

void testINT64sNotWithinDelta(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC);
    VERIFY_FAILS_END
#endif
}

void testINT64sNotWithinDeltaAndDifferenceOverflows(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT64_WITHIN(1, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF);
    VERIFY_FAILS_END
#endif
}

void testEqualHEX64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 p0[] = {1, 8, 987, 65132u};
    _UU64 p1[] = {1, 8, 987, 65132u};
    _UU64 p2[] = {1, 8, 987, 2};
    _UU64 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p3, 1);
#endif
}

void testEqualUint64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 p0[] = {1, 8, 987, 65132u};
    _UU64 p1[] = {1, 8, 987, 65132u};
    _UU64 p2[] = {1, 8, 987, 2};
    _UU64 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p3, 1);
#endif
}

void testEqualInt64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _US64 p0[] = {1, 8, 987, -65132};
    _US64 p1[] = {1, 8, 987, -65132};
    _US64 p2[] = {1, 8, 987, -2};
    _US64 p3[] = {1, 500, 600, 700};

    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p3, 1);
#endif
}


void testNotEqualHEX64Arrays1(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 p0[] = {1, 8, 987, 65132u};
    _UU64 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualHEX64Arrays2(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 p0[] = {1, 8, 987, 65132u};
    _UU64 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualUint64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _UU64 p0[] = {1, 8, 987, 65132u};
    _UU64 p1[] = {1, 8, 987, 65131u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualInt64Arrays(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    _US64 p0[] = {1, 8, 987, -65132};
    _US64 p1[] = {1, 8, 987, -65131};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}
// ===================== THESE TEST WILL RUN IF YOUR CONFIG INCLUDES FLOAT SUPPORT ==================

void testFloatsWithinDelta(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_WITHIN(0.00003f, 187245.03485f, 187245.03488f);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 187245.0f, 187246.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 9273.2549f, 9273.2049f);
    TEST_ASSERT_FLOAT_WITHIN(0.007f, -726.93724f, -726.94424f);
#endif
}

void testFloatsNotWithinDelta(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 9273.2649f, 9273.2049f);
    VERIFY_FAILS_END
#endif
}

void testFloatsEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_FLOAT(187245.0f, 187246.0f);
    TEST_ASSERT_EQUAL_FLOAT(18724.5f, 18724.6f);
    TEST_ASSERT_EQUAL_FLOAT(9273.2549f, 9273.2599f);
    TEST_ASSERT_EQUAL_FLOAT(-726.93724f, -726.9374f);
#endif
}

void testFloatsNotEqual(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(9273.9649f, 9273.0049f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualNegative1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(-9273.9649f, -9273.0049f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualNegative2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(-9273.0049f, -9273.9649f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualActualNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(85.963f, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualExpectedNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 85.963f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualBothNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualInfNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualNaNInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualActualInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(321.642f, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualExpectedInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 321.642f);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualBothInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatsNotEqualPlusMinusInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, -1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsPosInf1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_INF(2.0f / f_zero);
#endif
}

void testFloatIsPosInf2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_INF(2.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNegInf1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NEG_INF(-3.0f / f_zero);
#endif
}

void testFloatIsNegInf2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_NEG_INF(-3.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotPosInf1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(2.0f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotPosInf2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NOT_INF(2.0f);
#endif
}

void testFloatIsNotNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NEG_INF(-999.876f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNan1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NAN(0.0f / f_zero);
#endif
}

void testFloatIsNan2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_NAN(0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotNan1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NAN(234.9f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotNan2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NOT_NAN(234.9f);
#endif
}

void testFloatInfIsNotNan(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NAN(1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatNanIsNotInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(0.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testFloatIsDeterminate1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_DETERMINATE(0.0f);
    TEST_ASSERT_FLOAT_IS_DETERMINATE(123.3f);
    TEST_ASSERT_FLOAT_IS_DETERMINATE(-88.3f);
#endif
}

void testFloatIsDeterminate2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(-88.3f);
    VERIFY_FAILS_END
#endif
}

void testFloatIsNotDeterminate1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(1.0f / f_zero);
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(-1.0f / f_zero);
    TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(0.0f / f_zero);
#endif
}

void testFloatIsNotDeterminate2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_DETERMINATE(-1.0f / f_zero);
    VERIFY_FAILS_END
#endif
}

void testEqualFloatArrays(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, -8.0f,  25.4f, -0.123f};
    float p1[] = {1.0f, -8.0f,  25.4f, -0.123f};
    float p2[] = {1.0f, -8.0f,  25.4f, -0.2f};
    float p3[] = {1.0f, -23.0f, 25.0f, -0.26f};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p3, 1);
#endif
}

void testNotEqualFloatArraysExpectedNull(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float* p0 = NULL;
    float p1[] = {1.0f, 8.0f, 25.4f, 0.252f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysActualNull(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float* p1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArrays1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float p1[] = {1.0f, 8.0f, 25.4f, 0.252f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArrays2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float p1[] = {2.0f, 8.0f, 25.4f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArrays3(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float p1[] = {1.0f, 8.0f, 25.5f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysNegative1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-1.0f, -8.0f, -25.4f, -0.253f};
    float p1[] = {-1.0f, -8.0f, -25.4f, -0.252f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysNegative2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-1.0f, -8.0f, -25.4f, -0.253f};
    float p1[] = {-2.0f, -8.0f, -25.4f, -0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysNegative3(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {-1.0f, -8.0f, -25.4f, -0.253f};
    float p1[] = {-1.0f, -8.0f, -25.5f, -0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 0.0f / f_zero, 25.4f, 0.253f};
    float p1[] = {1.0f, 0.0f / f_zero, 25.4f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualFloatArraysInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 1.0f / f_zero, 25.4f, 0.253f};
    float p1[] = {1.0f, 1.0f / f_zero, 25.4f, 0.253f};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

// ===================== THESE TEST WILL RUN IF YOUR CONFIG INCLUDES DOUBLE SUPPORT ==================

void testDoublesWithinDelta(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_WITHIN(0.00003, 187245.03485, 187245.03488);
    TEST_ASSERT_DOUBLE_WITHIN(1.0, 187245.0, 187246.0);
    TEST_ASSERT_DOUBLE_WITHIN(0.05, 9273.2549, 9273.2049);
    TEST_ASSERT_DOUBLE_WITHIN(0.007, -726.93725, -726.94424);
#endif
}

void testDoublesNotWithinDelta(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_WITHIN(0.05, 9273.2649, 9273.2049);
    VERIFY_FAILS_END
#endif
}


void testDoublesEqual(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_EQUAL_DOUBLE(187245123456.0, 187245123456.0);
    TEST_ASSERT_EQUAL_DOUBLE(187241234567.5, 187241234567.6);
    TEST_ASSERT_EQUAL_DOUBLE(9273.2512345649, 9273.25123455699);
    TEST_ASSERT_EQUAL_DOUBLE(-726.12345693724, -726.1234569374);
#endif
}

void testDoublesNotEqual(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(9273.9649, 9273.0049);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualNegative1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(-9273.9649, -9273.0049);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualNegative2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(-9273.0049, -9273.9649);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualActualNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(85.963, 0.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualExpectedNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(0.0 / d_zero, 85.963);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualBothNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(0.0 / d_zero, 0.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualInfNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, 0.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualNaNInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(0.0 / d_zero, 1.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualActualInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(321.642, 1.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualExpectedInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, 321.642);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualBothInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, 1.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoublesNotEqualPlusMinusInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, -1.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsPosInf1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_IS_INF(2.0 / d_zero);
#endif
}

void testDoubleIsPosInf2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NOT_INF(2.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsNegInf1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_IS_NEG_INF(-3.0 / d_zero);
#endif
}

void testDoubleIsNegInf2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NOT_NEG_INF(-3.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsNotPosInf1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_INF(2.0);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsNotPosInf2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_IS_NOT_INF(2.0);
#endif
}

void testDoubleIsNotNegInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NEG_INF(-999.876);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsNan1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_IS_NAN(0.0 / d_zero);
#endif
}

void testDoubleIsNan2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NOT_NAN(0.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsNotNan1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NAN(234.9);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsNotNan2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_IS_NOT_NAN(234.9);
#endif
}

void testDoubleInfIsNotNan(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NAN(1.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoubleNanIsNotInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_INF(0.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsDeterminate1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_IS_DETERMINATE(0.0);
    TEST_ASSERT_DOUBLE_IS_DETERMINATE(123.3);
    TEST_ASSERT_DOUBLE_IS_DETERMINATE(-88.3);
#endif
}

void testDoubleIsDeterminate2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(-88.3);
    VERIFY_FAILS_END
#endif
}

void testDoubleIsNotDeterminate1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(1.0 / d_zero);
    TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(-1.0 / d_zero);
    TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(0.0 / d_zero);
#endif
}

void testDoubleIsNotDeterminate2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_DETERMINATE(-1.0 / d_zero);
    VERIFY_FAILS_END
#endif
}

void testEqualDoubleArrays(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, -8.0,  25.4, -0.123};
    double p1[] = {1.0, -8.0,  25.4, -0.123};
    double p2[] = {1.0, -8.0,  25.4, -0.2};
    double p3[] = {1.0, -23.0, 25.0, -0.26};

    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p0, 4);
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p2, 3);
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p3, 1);
#endif
}

void testNotEqualDoubleArraysExpectedNull(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double* p0 = NULL;
    double p1[] = {1.0, 8.0, 25.4, 0.252};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArraysActualNull(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, 8.0, 25.4, 0.253};
    double* p1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArrays1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, 8.0, 25.4, 0.25666666667};
    double p1[] = {1.0, 8.0, 25.4, 0.25666666666};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArrays2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, 8.0, 25.4, 0.253};
    double p1[] = {2.0, 8.0, 25.4, 0.253};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArrays3(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, 8.0, 25.4, 0.253};
    double p1[] = {1.0, 8.0, 25.5, 0.253};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArraysNegative1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {-1.0, -8.0, -25.4, -0.2566666667};
    double p1[] = {-1.0, -8.0, -25.4, -0.2566666666};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArraysNegative2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {-1.0, -8.0, -25.4, -0.253};
    double p1[] = {-2.0, -8.0, -25.4, -0.253};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArraysNegative3(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {-1.0, -8.0, -25.4, -0.253};
    double p1[] = {-1.0, -8.0, -25.5, -0.253};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArraysNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, 0.0 / d_zero, 25.4, 0.253};
    double p1[] = {1.0, 0.0 / d_zero, 25.4, 0.253};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testNotEqualDoubleArraysInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, 1.0 / d_zero, 25.4, 0.253};
    double p1[] = {1.0, 1.0 / d_zero, 25.4, 0.253};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

void testThatDetailsCanBeHandleOneDetail(void)
{
#ifdef UNITY_EXCLUDE_DETAILS
    TEST_IGNORE();
#else
    UNITY_SET_DETAIL("Detail1");

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, 6, "Should Fail And Say Detail1");
    VERIFY_FAILS_END
#endif
}

void testThatDetailsCanBeHandleTwoDetails(void)
{
#ifdef UNITY_EXCLUDE_DETAILS
    TEST_IGNORE();
#else
    UNITY_SET_DETAILS("Detail1","Detail2");

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(7, 8, "Should Fail And Say Detail1 and Detail2");
    VERIFY_FAILS_END
#endif
}

void testThatDetailsCanBeHandleSingleDetailClearingTwoDetails(void)
{
#ifdef UNITY_EXCLUDE_DETAILS
    TEST_IGNORE();
#else
    UNITY_SET_DETAILS("Detail1","Detail2");
    UNITY_SET_DETAIL("DetailNew");

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_MESSAGE("MEH", "GUH", "Should Fail And Say DetailNew");
    VERIFY_FAILS_END
#endif
}
