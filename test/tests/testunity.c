/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include <setjmp.h>
#include "unity.h"

// Dividing by these constants produces +/- infinity.
// The rationale is given in UnityAssertFloatIsInf's body.
#ifndef UNITY_EXCLUDE_FLOAT
static const _UF f_zero = 0.0f;
#endif

#ifndef UNITY_EXCLUDE_DOUBLE
static const _UD d_zero = 0.0;
#endif

static const char TestUnityStrIgnoreMessage[] = "This is an expected TEST_IGNORE_MESSAGE string!";
static const char TestUnityStrEmptyMessage[] = "";
static const char TestUnityStrPassMessage[] = "Pass with message";

#define EXPECT_ABORT_BEGIN \
    Unity.CurrentAbortFrame += 1; \
    if (TEST_PROTECT())    \
    {

#define VERIFY_IGNORES_END                                                     \
    }                                                                          \
    Unity.CurrentAbortFrame -= 1;                                              \


#define VERIFY_FAILS_END(expected)                                             \
    }                                                                          \
    Unity.CurrentAbortFrame -= 1;                                              \
    TEST_ASSERT_FAILED(expected);                                              \
    UnityResetMessage();                                                       \
    while(0)


int SetToOneToFailInTearDown;
const char *ExpectedMessage;
int ShouldBeIgnored;

void setUp(void)
{
  SetToOneToFailInTearDown = 0;
  ExpectedMessage = NULL;
  ShouldBeIgnored = 0;
}

void tearDown(void)
{
    if (SetToOneToFailInTearDown == 1)
        TEST_FAIL_MESSAGE("<= Failed in tearDown");

    if (ShouldBeIgnored != Unity.CurrentTestIgnored) {
        Unity.CurrentTestIgnored = 0;
        if (ShouldBeIgnored) {
            TEST_FAIL_MESSAGE("Test should have been ignored but was not");
        } else {
            TEST_FAIL_MESSAGE("Test should not have been ignored");
        }
    }

    if (ExpectedMessage != NULL) {
        Unity.CurrentTestIgnored = 0;
        char CurrentTestMessage[UNITY_MAX_MESSAGE_LEN];
        memcpy(CurrentTestMessage, Unity.CurrentTestMessage, Unity.CurrentTestMessageLen);
        TEST_ASSERT_EQUAL_STRING(ExpectedMessage, CurrentTestMessage);
        Unity.CurrentTestIgnored = ShouldBeIgnored;
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
    struct {
        const char* TestFile;
        const char* CurrentTestName;
        UNITY_LINE_TYPE CurrentTestLineNumber;
        const char	CurrentTestMessage[UNITY_MAX_MESSAGE_LEN];
        UNITY_COUNTER_TYPE CurrentTestMessageLen;
        UNITY_COUNTER_TYPE NumberOfTests;
        UNITY_COUNTER_TYPE TestFailures;
        UNITY_COUNTER_TYPE TestIgnores;
        UNITY_COUNTER_TYPE CurrentTestFailed;
        UNITY_COUNTER_TYPE CurrentTestIgnored;
        jmp_buf AbortFrame[UNITY_MAX_PROTECTION_NESTING];
        uint32_t CurrentAbortFrame;
    } _Expected_Unity;

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
    VERIFY_FAILS_END("Expression Evaluated To FALSE");
}

void testNotTrue(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_TRUE(0);
    VERIFY_FAILS_END("Expected TRUE Was FALSE");
}

void testNotFalse(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FALSE(1);
    VERIFY_FAILS_END("Expected FALSE Was TRUE");
}

void testNotUnless(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UNLESS(1);
    VERIFY_FAILS_END("Expression Evaluated To TRUE");
}

void testNotNotEqual(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL(10, 10);
    VERIFY_FAILS_END("Expected Not-Equal");
}

void testFail(void)
{
    EXPECT_ABORT_BEGIN
    TEST_FAIL_MESSAGE("Expected for testing");
    VERIFY_FAILS_END("Expected for testing");
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
    VERIFY_FAILS_END("Expected NULL");
}

void testNotNullShouldFailIfNULL(void)
{
    char* ptr1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_NULL(ptr1);
    VERIFY_FAILS_END("Expected Non-NULL");
}

void testIgnore(void)
{
    ShouldBeIgnored = 1;
    ExpectedMessage = TestUnityStrEmptyMessage;
    EXPECT_ABORT_BEGIN
    TEST_IGNORE();
    TEST_FAIL_MESSAGE("This should not be reached");
    VERIFY_IGNORES_END
}

void testIgnoreMessage(void)
{
    ShouldBeIgnored = 1;
    ExpectedMessage = TestUnityStrIgnoreMessage;
    EXPECT_ABORT_BEGIN
    TEST_IGNORE_MESSAGE(TestUnityStrIgnoreMessage);
    TEST_FAIL_MESSAGE("This should not be reached");
    VERIFY_IGNORES_END
}

void testNotEqualInts(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(3982, 3983);
    VERIFY_FAILS_END("Expected 3982 Was 3983");
}

void testNotEqualInt8s(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT8(-127, -126);
    VERIFY_FAILS_END("Expected -127 Was -126");
}

void testNotEqualInt16s(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT16(-16383, -16382);
    VERIFY_FAILS_END("Expected -16383 Was -16382");
}

void testNotEqualInt32s(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT32(-2147483647, -2147483646);
    VERIFY_FAILS_END("Expected -2147483647 Was -2147483646");
}

void testNotEqualBits(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS(0xFF00, 0x5555, 0x5A55);
    VERIFY_FAILS_END("Expected XXXXXXXXXXXXXXXX01010101XXXXXXXX Was XXXXXXXXXXXXXXXX01011010XXXXXXXX");
}

void testNotEqualUInts(void)
{
    _UU16 v0, v1;

    v0 = 9000;
    v1 = 9001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT(v0, v1);
    VERIFY_FAILS_END("Expected 9000 Was 9001");
}

void testNotEqualUInt8s(void)
{
    _UU8 v0, v1;

    v0 = 254;
    v1 = 255;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8(v0, v1);
    VERIFY_FAILS_END("Expected 254 Was 255");
}

void testNotEqualUInt16s(void)
{
    _UU16 v0, v1;

    v0 = 65535;
    v1 = 65534;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16(v0, v1);
    VERIFY_FAILS_END("Expected 65535 Was 65534");
}

void testNotEqualUInt32s(void)
{
    _UU32 v0, v1;

    v0 = 4294967295;
    v1 = 4294967294;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32(v0, v1);
    VERIFY_FAILS_END("Expected 4294967295 Was 4294967294");
}

void testNotEqualHex8s(void)
{
    _UU8 v0, v1;

    v0 = 0x23;
    v1 = 0x22;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    VERIFY_FAILS_END("Expected 0x23 Was 0x22");
}

void testNotEqualHex8sIfSigned(void)
{
    _US8 v0, v1;

    v0 = -2;
    v1 = 2;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    VERIFY_FAILS_END("Expected 0xFE Was 0x02");
}

void testNotEqualHex16s(void)
{
    _UU16 v0, v1;

    v0 = 0x1234;
    v1 = 0x1235;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    VERIFY_FAILS_END("Expected 0x1234 Was 0x1235");
}

void testNotEqualHex16sIfSigned(void)
{
    _US16 v0, v1;

    v0 = -1024;
    v1 = -1028;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    VERIFY_FAILS_END("Expected 0xFC00 Was 0xFBFC");
}

void testNotEqualHex32s(void)
{
    _UU32 v0, v1;

    v0 = 900000;
    v1 = 900001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END("Expected 0x000DBBA0 Was 0x000DBBA1");
}

void testNotEqualHex32sIfSigned(void)
{
    _US32 v0, v1;

    v0 = -900000;
    v1 = 900001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END("Expected 0xFFF24460 Was 0x000DBBA1");
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
    VERIFY_FAILS_END("Expected 1XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Was 0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
}

void testNotEqualBitLow(void)
{
    _UU32 v0 = 0xFF55AA00;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BIT_LOW(30, v0);
    VERIFY_FAILS_END("Expected X0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Was X1XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
}

void testNotEqualBitsHigh(void)
{
    _UU32 v0 = 0xFF55AA00;
    _UU32 v1 = 0x55550000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_HIGH(v0, v1);
    VERIFY_FAILS_END("Expected 11111111X1X1X1X11X1X1X1XXXXXXXXX Was 01010101X1X1X1X10X0X0X0XXXXXXXXX");

}

void testNotEqualBitsLow(void)
{
    _UU32 v0 = 0xFF55AA00;
    _UU32 v1 = 0x55550000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_LOW(v0, v1);
    VERIFY_FAILS_END("Expected 00000000X0X0X0X00X0X0X0XXXXXXXXX Was 01010101X1X1X1X10X0X0X0XXXXXXXXX");

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
#ifdef __x86_64__
    VERIFY_FAILS_END("Expected 0x0000000012345678 Was 0x0000000012345677");
#else
    VERIFY_FAILS_END("Expected 0x12345678 Was 0x12345677");
#endif
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
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 5000 Was 5006");
}

void testIntsNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 5000, 5006, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 5000 Was 5006. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected 2147483647 Was 2147483649");
}

void testUIntsNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected 2147483647 Was 2147483649. Custom Message.");
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(5, 1, -1);
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 1 Was 4294967295");
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 1 Was 4294967295. Custom Message.");
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN(5, -1, 1);
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 4294967295 Was 1");
}

void testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, -1, 1, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 4294967295 Was 1. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 0x00000001 Expected 0x7FFFFFFF Was 0x80000001");
}

void testHEX32sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 0x00000001 Expected 0x7FFFFFFF Was 0x80000001. Custom Message.");
}

void testHEX32sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN(5, 1, -1);
    VERIFY_FAILS_END("Values Not Within Delta 0x00000005 Expected 0x00000001 Was 0xFFFFFFFF");
}

void testHEX32sNotWithinDeltaEvenThoughASignedIntWouldPassAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 0x00000005 Expected 0x00000001 Was 0xFFFFFFFF. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 0x0002 Expected 0xFFFF Was 0x0000");
}

void testHEX16sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(2, 65535, 0, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 0x0002 Expected 0xFFFF Was 0x0000. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 0x02 Expected 0xFF Was 0x00");
}

void testHEX8sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(2, 255, 0, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 0x02 Expected 0xFF Was 0x00. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected 2147483647 Was 2147483649");
}

void testUINT32sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected 2147483647 Was 2147483649. Custom Message.");
}

void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(5, 1, -1);
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 1 Was 4294967295");
}

void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 1 Was 4294967295. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected 65535 Was 0");
}

void testUINT16sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(2, 65535, 0, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected 65535 Was 0. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected 255 Was 0");
}

void testUINT8sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(2, 255, 0, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected 255 Was 0. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected -3 Was 1");
}

void testINT32sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN_MESSAGE(1, -2, 1, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected -2 Was 1. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected 4 Was -2");
}

void testINT16sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_WITHIN_MESSAGE(2, 3, 0, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected 3 Was 0. Custom Message.");
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
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected -3 Was 0");
}

void testINT8sNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_WITHIN_MESSAGE(2, -4, 0, "Custom Message.");
    VERIFY_FAILS_END("Values Not Within Delta 2 Expected -4 Was 0. Custom Message.");
}

void testEqualStrings(void)
{
    const char *testString = "foo";

    TEST_ASSERT_EQUAL_STRING(testString, testString);
    TEST_ASSERT_EQUAL_STRING("foo", "foo");
    TEST_ASSERT_EQUAL_STRING("foo", testString);
    TEST_ASSERT_EQUAL_STRING(testString, "foo");
    TEST_ASSERT_EQUAL_STRING("", "");
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
    VERIFY_FAILS_END("Expected 'foo' Was 'bar'");
}

void testNotEqualString2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", "");
    VERIFY_FAILS_END("Expected 'foo' Was ''");
}

void testNotEqualString3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("", "bar");
    VERIFY_FAILS_END("Expected '' Was 'bar'");
}

void testNotEqualString4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("bar\r", "bar\n");
    VERIFY_FAILS_END("Expected 'bar\\r' Was 'bar\\n'");
}

void testNotEqualString5(void)
{
    const char str1[] = { 0x41, 0x42, 0x03, 0x00 };
    const char str2[] = { 0x41, 0x42, 0x04, 0x00 };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(str1, str2);
    VERIFY_FAILS_END("Expected 'AB\\0x03' Was 'AB\\0x04'");
}

void testNotEqualString_ExpectedStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(NULL, "bar");
    VERIFY_FAILS_END("Expected NULL Was 'bar'");
}

void testNotEqualString_ActualStringIsNull(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", NULL);
    VERIFY_FAILS_END("Expected 'foo' Was NULL");
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
    VERIFY_FAILS_END("Element 3 Expected 'zoo' Was 'moo'");
}

void testNotEqualStringArray2(void)
{
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", "boo", "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END("Element 0 Expected 'foo' Was 'zoo'");
}

void testNotEqualStringArray3(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", NULL };
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END("Element 3 Expected 'zoo' Was NULL");
}

void testNotEqualStringArray4(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[] = { "foo", NULL, "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END("Element 1 Expected NULL Was 'boo'");
}

void testNotEqualStringArray5(void)
{
    const char **testStrings = NULL;
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END("Actual pointer was NULL");
}

void testNotEqualStringArray6(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char **expStrings = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END("Expected pointer to be NULL");
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
    VERIFY_FAILS_END("Memory Mismatch. Byte 0 Expected 0x66 Was 0x62");
}

void testNotEqualMemory2(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", "food", 4);
    VERIFY_FAILS_END("Memory Mismatch. Byte 3 Expected 0x6C Was 0x64");
}

void testNotEqualMemory3(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY(NULL, "food", 4);
    VERIFY_FAILS_END("Expected pointer to be NULL");
}

void testNotEqualMemory4(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", NULL, 4);
    VERIFY_FAILS_END("Actual pointer was NULL");
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
    VERIFY_FAILS_END("Expected pointer to be NULL");
}

void testNotEqualIntArraysNullActual(void)
{
    int* p1 = NULL;
    int p0[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Actual pointer was NULL");
}

void testNotEqualIntArrays1(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 3 Expected -2 Was 2");
}

void testNotEqualIntArrays2(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 1 Was 2");
}

void testNotEqualIntArrays3(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 987 Was 986");
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
    VERIFY_FAILS_END("Expected pointer to be NULL");
}

void testNotEqualPtrArraysNullActual(void)
{
    char A = 1;
    char B = 2;
    char** p0 = NULL;
    char* p1[] = {&A, &B};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_PTR_ARRAY(p1, p0, 2);
    VERIFY_FAILS_END("Actual pointer was NULL");
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
    VERIFY_FAILS_END(NULL);
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
    VERIFY_FAILS_END(NULL);
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
    VERIFY_FAILS_END(NULL);
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
    VERIFY_FAILS_END("Element 3 Expected -2 Was 2");
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
    VERIFY_FAILS_END("Element 3 Expected 65132 Was 65131");
}

void testNotEqualUIntArrays2(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 1 Was 2");
}

void testNotEqualUIntArrays3(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 987 Was 986");
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
    VERIFY_FAILS_END("Element 3 Expected 3 Was 2");
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
    VERIFY_FAILS_END("Element 3 Expected 3 Was 2");
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
    VERIFY_FAILS_END("Element 3 Expected 127 Was 255");
}

void testNotEqualUINT8Arrays2(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 3 Expected 127 Was 255");
}

void testNotEqualUINT8Arrays3(void)
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 3 Expected 127 Was 255");
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
    VERIFY_FAILS_END("Element 3 Expected 65132 Was 65131");
}

void testNotEqualUINT16Arrays2(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 1 Was 2");
}

void testNotEqualUINT16Arrays3(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 987 Was 986");
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
    VERIFY_FAILS_END("Element 3 Expected 65132 Was 65131");
}

void testNotEqualUINT32Arrays2(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 1 Was 2");
}

void testNotEqualUINT32Arrays3(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 987 Was 986");
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
    VERIFY_FAILS_END("Element 3 Expected 0x0000FE6C Was 0x0000FE6B");
}

void testNotEqualHEXArrays2(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 0x00000001 Was 0x00000002");
}

void testNotEqualHEXArrays3(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 0x000003DB Was 0x000003DA");
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
    VERIFY_FAILS_END("Element 3 Expected 0x0000FE6C Was 0x0000FE6B");
}

void testNotEqualHEX32Arrays2(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 0x00000001 Was 0x00000002");
}

void testNotEqualHEX32Arrays3(void)
{
    _UU32 p0[] = {1, 8, 987, 65132u};
    _UU32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 0x000003DB Was 0x000003DA");
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
    VERIFY_FAILS_END("Element 3 Expected 0xFE6C Was 0xFE6B");
}

void testNotEqualHEX16Arrays2(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 0x0001 Was 0x0002");
}

void testNotEqualHEX16Arrays3(void)
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 0x03DB Was 0x03DA");
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
    VERIFY_FAILS_END("Element 3 Expected 0xFD Was 0xFC");
}

void testNotEqualHEX8Arrays2(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {2, 8, 254u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 0 Expected 0x01 Was 0x02");
}

void testNotEqualHEX8Arrays3(void)
{
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 255u, 253u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END("Element 2 Expected 0xFE Was 0xFF");
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
    VERIFY_FAILS_END("Expected pointer to be NULL");
}

void testNotEqualMemoryArraysActualNull(void)
{
    int p0[] = {1, 8, 987, -2};
    int* p1 = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END("Actual pointer was NULL");
}

void testNotEqualMemoryArrays1(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END("Memory Mismatch. Element 3 Byte 0 Expected 0xFE Was 0x02");
}

void testNotEqualMemoryArrays2(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END("Memory Mismatch. Element 0 Byte 0 Expected 0x01 Was 0x02");
}

void testNotEqualMemoryArrays3(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END("Memory Mismatch. Element 2 Byte 0 Expected 0xDB Was 0xDA");
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
    ShouldBeIgnored = 1;
    SetToOneToFailInTearDown = 1;
    TEST_IGNORE();
}

// ===================== THESE TEST WILL RUN IF YOUR CONFIG INCLUDES 64 BIT SUPPORT ==================

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
    VERIFY_FAILS_END("Expected 0x0000000218711A00 Was 0x000000021E66FB00");
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
    VERIFY_FAILS_END("Expected 9000000000 Was 9100000000");
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
    VERIFY_FAILS_END("Expected -9000000000 Was 9100000000");
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
    VERIFY_FAILS_END("Expected 0xFFFFFFFDE78EE600 Was 0x0000000218711A00");
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
    VERIFY_FAILS_END("Values Not Within Delta 0x0000000000000001 Expected 0x7FFFFFFFFFFFFFFF Was 0x7FFFFFFFFFFFFFFC");
#endif
}

void testHEX64sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX64_WITHIN(5, 1, -1);
    VERIFY_FAILS_END("Values Not Within Delta 0x0000000000000005 Expected 0x0000000000000001 Was 0xFFFFFFFFFFFFFFFF");
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
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected 9223372036854775807 Was 9223372036854775804");
#endif
}

void testUINT64sNotWithinDeltaEvenThoughASignedIntWouldPass(void)
{
#ifndef UNITY_SUPPORT_64
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT64_WITHIN(5, 1, -1);
    VERIFY_FAILS_END("Values Not Within Delta 5 Expected 1 Was 18446744073709551615");
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
    VERIFY_FAILS_END("Values Not Within Delta 1 Expected 9223372036854775807 Was 9223372036854775804");
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
    VERIFY_FAILS_END("Element 3 Expected 0x000000000000FE6C Was 0x000000000000FE6B");
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
    VERIFY_FAILS_END("Element 0 Expected 0x0000000000000001 Was 0x0000000000000002");
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
    VERIFY_FAILS_END("Element 3 Expected 65132 Was 65131");
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
    VERIFY_FAILS_END("Element 3 Expected -65132 Was -65131");
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected 9273.264648 Was 9273.205078");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected 9273.964844 Was 9273.004883");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualNegative1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(-9273.9649f, -9273.0049f);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected -9273.964844 Was -9273.004883");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualNegative2(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(-9273.0049f, -9273.9649f);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected -9273.004883 Was -9273.964844");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualActualNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(85.963f, 0.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected 85.962997 Was -nan");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualExpectedNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 85.963f);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected -nan Was 85.962997");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualBothNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 0.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected -nan Was -nan");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualInfNaN(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 0.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected inf Was -nan");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualNaNInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(0.0f / f_zero, 1.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected -nan Was inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualActualInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(321.642f, 1.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected 321.641998 Was inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualExpectedInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 321.642f);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected inf Was 321.641998");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualBothInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, 1.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected inf Was inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testFloatsNotEqualPlusMinusInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(1.0f / f_zero, -1.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected inf Was -inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Not Infinity Was inf");
#else
    VERIFY_FAILS_END("Expected Not Infinity Was Infinity");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Not Negative Infinity Was -inf");
#else
    VERIFY_FAILS_END("Expected Not Negative Infinity Was Negative Infinity");
#endif
#endif
}

void testFloatIsNotPosInf1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(2.0f);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Infinity Was 2.000000");
#else
    VERIFY_FAILS_END("Expected Infinity Was Not Infinity");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Negative Infinity Was -999.875977");
#else
    VERIFY_FAILS_END("Expected Negative Infinity Was Not Negative Infinity");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Not NaN Was -nan");
#else
    VERIFY_FAILS_END("Expected Not NaN Was NaN");
#endif
#endif
}

void testFloatIsNotNan1(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NAN(234.9f);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected NaN Was 234.899994");
#else
    VERIFY_FAILS_END("Expected NaN Was Not NaN");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected NaN Was inf");
#else
    VERIFY_FAILS_END("Expected NaN Was Not NaN");
#endif
#endif
}

void testFloatNanIsNotInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(0.0f / f_zero);
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Infinity Was -nan");
#else
    VERIFY_FAILS_END("Expected Infinity Was Not Infinity");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Not Determinate Was -88.300003");
#else
    VERIFY_FAILS_END("Expected Not Determinate Was Determinate");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Expected Determinate Was -inf");
#else
    VERIFY_FAILS_END("Expected Determinate Was Not Determinate");
#endif
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
    VERIFY_FAILS_END("Expected pointer to be NULL");
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
    VERIFY_FAILS_END("Actual pointer was NULL");
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 3 Expected 0.253000 Was 0.252000");
#else
    VERIFY_FAILS_END("Element 3 Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 0 Expected 1.000000 Was 2.000000");
#else
    VERIFY_FAILS_END("Element 0 Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 2 Expected 25.400000 Was 25.500000");
#else
    VERIFY_FAILS_END("Element 2 Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 3 Expected -0.253000 Was -0.252000");
#else
    VERIFY_FAILS_END("Element 3 Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 0 Expected -1.000000 Was -2.000000");
#else
    VERIFY_FAILS_END("Element 0 Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 2 Expected -25.400000 Was -25.500000");
#else
    VERIFY_FAILS_END("Element 2 Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 1 Expected -nan Was -nan");
#else
    VERIFY_FAILS_END("Element 1 Values Not Within Delta");
#endif
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
#ifdef UNITY_FLOAT_VERBOSE
    VERIFY_FAILS_END("Element 1 Expected inf Was inf");
#else
    VERIFY_FAILS_END("Element 1 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected 9273.264648 Was 9273.205078");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected 9273.964844 Was 9273.004883");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualNegative1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(-9273.9649, -9273.0049);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected -9273.964844 Was -9273.004883");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualNegative2(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(-9273.0049, -9273.9649);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected -9273.004883 Was -9273.964844");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualActualNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(85.963, 0.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected 85.962997 Was -nan");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualExpectedNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(0.0 / d_zero, 85.963);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected -nan Was 85.962997");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualBothNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(0.0 / d_zero, 0.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected -nan Was -nan");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualInfNaN(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, 0.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected inf Was -nan");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualNaNInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(0.0 / d_zero, 1.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected -nan Was inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualActualInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(321.642, 1.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected 321.641998 Was inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualExpectedInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, 321.642);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected inf Was 321.641998");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualBothInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, 1.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected inf Was inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
#endif
}

void testDoublesNotEqualPlusMinusInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, -1.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected inf Was -inf");
#else
    VERIFY_FAILS_END("Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Not Infinity Was inf");
#else
    VERIFY_FAILS_END("Expected Not Infinity Was Infinity");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Not Negative Infinity Was -inf");
#else
    VERIFY_FAILS_END("Expected Not Negative Infinity Was Negative Infinity");
#endif
#endif
}

void testDoubleIsNotPosInf1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_INF(2.0);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Infinity Was 2.000000");
#else
    VERIFY_FAILS_END("Expected Infinity Was Not Infinity");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Negative Infinity Was -999.875977");
#else
    VERIFY_FAILS_END("Expected Negative Infinity Was Not Negative Infinity");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Not NaN Was -nan");
#else
    VERIFY_FAILS_END("Expected Not NaN Was NaN");
#endif
#endif
}

void testDoubleIsNotNan1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NAN(234.9);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected NaN Was 234.899994");
#else
    VERIFY_FAILS_END("Expected NaN Was Not NaN");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected NaN Was inf");
#else
    VERIFY_FAILS_END("Expected NaN Was Not NaN");
#endif
#endif
}

void testDoubleNanIsNotInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_INF(0.0 / d_zero);
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Infinity Was -nan");
#else
    VERIFY_FAILS_END("Expected Infinity Was Not Infinity");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Not Determinate Was -88.300003");
#else
    VERIFY_FAILS_END("Expected Not Determinate Was Determinate");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Expected Determinate Was -inf");
#else
    VERIFY_FAILS_END("Expected Determinate Was Not Determinate");
#endif
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
    VERIFY_FAILS_END("Expected pointer to be NULL");
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
    VERIFY_FAILS_END("Actual pointer was NULL");
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 3 Expected 0.256667 Was 0.256667");
#else
    VERIFY_FAILS_END("Element 3 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 0 Expected 1.000000 Was 2.000000");
#else
    VERIFY_FAILS_END("Element 0 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 2 Expected 25.400000 Was 25.500000");
#else
    VERIFY_FAILS_END("Element 2 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 3 Expected -0.256667 Was -0.256667");
#else
    VERIFY_FAILS_END("Element 3 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 0 Expected -1.000000 Was -2.000000");
#else
    VERIFY_FAILS_END("Element 0 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 2 Expected -25.400000 Was -25.500000");
#else
    VERIFY_FAILS_END("Element 2 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 1 Expected -nan Was -nan");
#else
    VERIFY_FAILS_END("Element 1 Values Not Within Delta");
#endif
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
#ifdef UNITY_DOUBLE_VERBOSE
    VERIFY_FAILS_END("Element 1 Expected inf Was inf");
#else
    VERIFY_FAILS_END("Element 1 Values Not Within Delta");
#endif
#endif
}

void testDontLoopInJump(void)
{
    _UU8 v = 0;
    EXPECT_ABORT_BEGIN
        EXPECT_ABORT_BEGIN
        {
            v += 1;
            TEST_FAIL();
        }
        VERIFY_FAILS_END(NULL);
        // this test should fail and bail but not increment v.
        TEST_ASSERT_EQUAL_UINT(2, v);
    }

    Unity.CurrentAbortFrame -= 1;
    TEST_ASSERT_FAILED("Expected 2 Was 1");
}

void testPrintPassMessage(void)
{
    ExpectedMessage = TestUnityStrPassMessage;
    UnityAppendString(TestUnityStrPassMessage);
}
