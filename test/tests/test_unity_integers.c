/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include "unity.h"
#define TEST_INSTANCES
#include "self_assessment_utils.h"

static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;

void setUp(void)
{
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}

void tearDown(void)
{
    endPutcharSpy(); /* Stop suppressing test output */
    if (SetToOneToFailInTearDown == 1)
    {
        /* These will be skipped internally if already failed/ignored */
        TEST_FAIL_MESSAGE("<= Failed in tearDown");
        TEST_IGNORE_MESSAGE("<= Ignored in tearDown");
    }
    if ((SetToOneMeanWeAlreadyCheckedThisGuy == 0) && (Unity.CurrentTestFailed > 0))
    {
        UnityPrint(": [[[[ Test Should Have Passed But Did Not ]]]]");
        UNITY_OUTPUT_CHAR('\n');
    }
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

void testNotEqualChars(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_CHAR('A', 'a');
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
    /*use largest 32 bit negative to test printability*/
    /*note: (-2147483647 - 1) is used instead of -2147483648 because of C90 casting rules */
    TEST_ASSERT_EQUAL_INT32(-2147483647, (-2147483647 - 1));
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
    UNITY_UINT16 v0, v1;

    v0 = 9000;
    v1 = 9001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualUInt8s(void)
{
    UNITY_UINT8 v0, v1;

    v0 = 254;
    v1 = 255;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT8(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualUInt16s(void)
{
    UNITY_UINT16 v0, v1;

    v0 = 65535u;
    v1 = 65534u;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualUInt32s(void)
{
    UNITY_UINT32 v0, v1;

    v0 = 4294967295u;
    v1 = 4294967294u;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex8s(void)
{
    UNITY_UINT8 v0, v1;

    v0 = 0x23;
    v1 = 0x22;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex8sIfSigned(void)
{
    UNITY_INT8 v0, v1;

    v0 = -2;
    v1 = 2;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex16s(void)
{
    UNITY_UINT16 v0, v1;

    v0 = 0x1234;
    v1 = 0x1235;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex16sIfSigned(void)
{
    UNITY_INT16 v0, v1;

    v0 = -1024;
    v1 = -1028;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex32s(void)
{
    UNITY_UINT32 v0, v1;

    v0 = 900000;
    v1 = 900001;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END
}

void testNotEqualHex32sIfSigned(void)
{
    UNITY_INT32 v0, v1;

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
    UNITY_INT8 v0, v1;
    UNITY_INT8 *p0, *p1;

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

void testEqualChars(void)
{
    char v0, v1;
    char *p0, *p1;

    v0 = 'A';
    v1 = 'A';
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_CHAR('A', 'A');
    TEST_ASSERT_EQUAL_CHAR(v0, v1);
    TEST_ASSERT_EQUAL_CHAR('A', v1);
    TEST_ASSERT_EQUAL_CHAR(v0, 'A');
    TEST_ASSERT_EQUAL_CHAR(*p0, v1);
    TEST_ASSERT_EQUAL_CHAR(*p0, *p1);
    TEST_ASSERT_EQUAL_CHAR(*p0, 'A');
}

void testEqualCharsWhenThereAreDifferencesOutside8Bits(void)
{
    TEST_ASSERT_EQUAL_CHAR(0x321,0x421);
    TEST_ASSERT_EQUAL_CHAR(0xFF21,0x0021);
}


void testEqualInt16s(void)
{
    UNITY_INT16 v0, v1;
    UNITY_INT16 *p0, *p1;

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
    UNITY_INT16 v0, v1;
    UNITY_INT16 *p0, *p1;

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
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

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
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

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
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

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
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

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
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

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
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

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
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

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
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

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
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

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
    UNITY_UINT32 v0 = 0xFF55AA00;
    UNITY_UINT32 v1 = 0x55550000;

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
    UNITY_UINT32 v0 = 0x7F55AA00;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BIT_HIGH(31, v0);
    VERIFY_FAILS_END
}

void testNotEqualBitLow(void)
{
    UNITY_UINT32 v0 = 0xFF55AA00;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BIT_LOW(30, v0);
    VERIFY_FAILS_END
}

void testNotEqualBitsHigh(void)
{
    UNITY_UINT32 v0 = 0xFF55AA00;
    UNITY_UINT32 v1 = 0x55550000;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_HIGH(v0, v1);
    VERIFY_FAILS_END

}

void testNotEqualBitsLow(void)
{
    UNITY_UINT32 v0 = 0xFF55AA00;
    UNITY_UINT32 v1 = 0x55550000;

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

void testEqualUInts(void)
{
    unsigned char v0, v1;
    unsigned char *p0, *p1;

    v0 = 109;
    v1 = 109;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_UINT(42, 42);
    TEST_ASSERT_EQUAL_UINT(-116, -116);
    TEST_ASSERT_EQUAL_UINT(v0, v1);
    TEST_ASSERT_EQUAL_UINT(109, v1);
    TEST_ASSERT_EQUAL_UINT(v0, 109);
    TEST_ASSERT_EQUAL_UINT(*p0, v1);
    TEST_ASSERT_EQUAL_UINT(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT(*p0, 109);
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

void testHEX32sWithinDeltaShouldIgnoreSign(void)
{
    TEST_ASSERT_HEX32_WITHIN(1, 0x7FFFFFFF, 0x80000000);
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

void testHEX16sWithinDeltaShouldIgnoreSign(void)
{
    TEST_ASSERT_HEX16_WITHIN(1, 0x7FFF, 0x8000);
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

void testHEX8sWithinDeltaShouldIgnoreSign(void)
{
    TEST_ASSERT_HEX8_WITHIN(1, 0x7F, 0x80);
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

/*-----------------*/

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

void testCHARsWithinDelta(void)
{
    TEST_ASSERT_CHAR_WITHIN(1, 'M', 'L');
    TEST_ASSERT_CHAR_WITHIN(5, -2, 2);
    TEST_ASSERT_CHAR_WITHIN(5, 2, -2);
}

void testCHARsWithinDeltaAndCustomMessage(void)
{
    TEST_ASSERT_CHAR_WITHIN_MESSAGE(5, 1, 4, "Custom Message.");
}

void testCHARsWithinDeltaWhenThereAreDifferenceOutsideOf8Bits(void)
{
    TEST_ASSERT_CHAR_WITHIN(5, 0x123, 0xF23);
}

void testCHARsWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage(void)
{
    TEST_ASSERT_CHAR_WITHIN_MESSAGE(5, 0x123, 0xF23, "Custom Message.");
}

void testCHARsNotWithinDelta(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_WITHIN(2, -3, 0);
    VERIFY_FAILS_END
}

void testCHARsNotWithinDeltaAndCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_CHAR_WITHIN_MESSAGE(2, -4, 0, "Custom Message.");
    VERIFY_FAILS_END
}

void testNotEqualINT(void)
{
    UNITY_INT v0, v1;
    UNITY_INT *p0, *p1;

    v0 = 302;
    v1 = 3334;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_INT(v0, v1);
    TEST_ASSERT_NOT_EQUAL_INT(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_INT(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_INT(*p1, *p0);
}

void testNotNotEqualINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_INT(302, 302);
    VERIFY_FAILS_END
}

void testNotEqualINT8(void)
{
    UNITY_INT8 v0, v1;
    UNITY_INT8 *p0, *p1;

    v0 = -128;
    v1 = 127;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_INT8(v0, v1);
    TEST_ASSERT_NOT_EQUAL_INT8(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_INT8(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_INT8(*p1, *p0);
}

void testNotNotEqualINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_INT8(-128, -128);
    VERIFY_FAILS_END
}

void testNotEqualCHAR(void)
{
    char v0, v1;
    char *p0, *p1;

    v0 = -128;
    v1 = 127;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_CHAR(v0, v1);
    TEST_ASSERT_NOT_EQUAL_CHAR(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_CHAR(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_CHAR(*p1, *p0);
}

void testNotNotEqualCHAR(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_CHAR(127, 127);
    VERIFY_FAILS_END
}

void testNotEqualINT16(void)
{
    UNITY_INT16 v0, v1;
    UNITY_INT16 *p0, *p1;

    v0 = -32768;
    v1 = 32767;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_INT16(v0, v1);
    TEST_ASSERT_NOT_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_INT16(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_INT16(*p1, *p0);
}

void testNotNotEqualINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_INT16(-32768, -32768);
    VERIFY_FAILS_END
}

void testNotEqualINT32(void)
{
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

    v0 = -214783648;
    v1 = 214783647;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_INT32(v0, v1);
    TEST_ASSERT_NOT_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_INT32(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_INT32(*p1, *p0);
}

void testNotNotEqualINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_INT32(-214783648, -214783648);
    VERIFY_FAILS_END
}

void testNotEqualUINT(void)
{
    UNITY_UINT v0, v1;
    UNITY_UINT *p0, *p1;

    v0 = 0;
    v1 = 1;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_UINT(v0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_UINT(*p1, *p0);
}

void testNotNotEqualUINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_UINT(1, 1);
    VERIFY_FAILS_END
}

void testNotEqualUINT8(void)
{
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

    v0 = 0;
    v1 = 255;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_UINT8(v0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT8(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT8(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_UINT8(*p1, *p0);
}

void testNotNotEqualUINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_UINT8(255, 255);
    VERIFY_FAILS_END
}

void testNotEqualUINT16(void)
{
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

    v0 = 0;
    v1 = 65535;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_UINT16(v0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT16(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT16(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_UINT16(*p1, *p0);
}

void testNotNotEqualUINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_UINT16(65535, 65535);
    VERIFY_FAILS_END
}

void testNotEqualUINT32(void)
{
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

    v0 = 0u;
    v1 = 4294967295u;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_UINT32(v0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT32(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_UINT32(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_UINT32(*p1, *p0);
}

void testNotNotEqualUINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_UINT32(4294967295u, 4294967295u);
    VERIFY_FAILS_END
}

void testNotEqualHEX8(void)
{
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

    v0 = 0x00;
    v1 = 0xFF;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_HEX8(v0, v1);
    TEST_ASSERT_NOT_EQUAL_HEX8(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_HEX8(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_HEX8(*p1, *p0);
}

void testNotNotEqualHEX8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_HEX8(0xFF, 0xFF);
    VERIFY_FAILS_END
}

void testNotEqualHEX16(void)
{
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

    v0 = 0x0000;
    v1 = 0xFFFF;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_HEX16(v0, v1);
    TEST_ASSERT_NOT_EQUAL_HEX16(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_HEX16(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_HEX16(*p1, *p0);
}

void testNotNotEqualHEX16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_HEX16(0xFFFF, 0xFFFF);
    VERIFY_FAILS_END
}

void testNotEqualHEX32(void)
{
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

    v0 = 0x00000000;
    v1 = 0xFFFFFFFF;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_NOT_EQUAL_HEX32(v0, v1);
    TEST_ASSERT_NOT_EQUAL_HEX32(*p0, v1);
    TEST_ASSERT_NOT_EQUAL_HEX32(v0, *p1);
    TEST_ASSERT_NOT_EQUAL_HEX32(*p1, *p0);
}

void testNotNotEqualHEX32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NOT_EQUAL_HEX32(0xFFFFFFFF, 0xFFFFFFFF);
    VERIFY_FAILS_END
}

/*-----------------*/

void testGreaterThan(void)
{
    UNITY_INT v0, v1;
    UNITY_INT *p0, *p1;

    v0 = 0;
    v1 = 1;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN(v0, v1);
    TEST_ASSERT_GREATER_THAN(*p0, v1);
    TEST_ASSERT_GREATER_THAN(v0, *p1);
    TEST_ASSERT_GREATER_THAN(*p0, *p1);
}

void testNotGreaterThan(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN(0, -1);
    VERIFY_FAILS_END
}

void testGreaterThanINT(void)
{
    UNITY_INT v0, v1;
    UNITY_INT *p0, *p1;

    v0 = 302;
    v1 = 3334;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_INT(v0, v1);
    TEST_ASSERT_GREATER_THAN_INT(*p0, v1);
    TEST_ASSERT_GREATER_THAN_INT(v0, *p1);
    TEST_ASSERT_GREATER_THAN_INT(*p0, *p1);
}

void testNotGreaterThanINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT(3334, 302);
    VERIFY_FAILS_END
}

void testGreaterThanINT8(void)
{
    UNITY_INT8 v0, v1;
    UNITY_INT8 *p0, *p1;

    v0 = -128;
    v1 = 127;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_INT8(v0, v1);
    TEST_ASSERT_GREATER_THAN_INT8(*p0, v1);
    TEST_ASSERT_GREATER_THAN_INT8(v0, *p1);
    TEST_ASSERT_GREATER_THAN_INT8(*p0, *p1);
}

void testNotGreaterThanINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT8(127, -128);
    VERIFY_FAILS_END
}

void testGreaterThanCHAR(void)
{
    char v0, v1;
    char *p0, *p1;

    v0 = -128;
    v1 = 127;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_CHAR(v0, v1);
    TEST_ASSERT_GREATER_THAN_CHAR(*p0, v1);
    TEST_ASSERT_GREATER_THAN_CHAR(v0, *p1);
    TEST_ASSERT_GREATER_THAN_CHAR(*p0, *p1);
}

void testNotGreaterThanCHAR(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_CHAR(127, -128);
    VERIFY_FAILS_END
}

void testGreaterThanINT16(void)
{
    UNITY_INT16 v0, v1;
    UNITY_INT16 *p0, *p1;

    v0 = -32768;
    v1 = 32767;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_INT16(v0, v1);
    TEST_ASSERT_GREATER_THAN_INT16(*p0, v1);
    TEST_ASSERT_GREATER_THAN_INT16(v0, *p1);
    TEST_ASSERT_GREATER_THAN_INT16(*p0, *p1);
}

void testNotGreaterThanINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT16(32768, -32768);
    VERIFY_FAILS_END
}

void testGreaterThanINT32(void)
{
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

    v0 = -214783648;
    v1 = 214783647;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_INT32(v0, v1);
    TEST_ASSERT_GREATER_THAN_INT32(*p0, v1);
    TEST_ASSERT_GREATER_THAN_INT32(v0, *p1);
    TEST_ASSERT_GREATER_THAN_INT32(*p0, *p1);
}

void testNotGreaterThanINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT32(214783647, -214783648);
    VERIFY_FAILS_END
}

void testGreaterThanUINT(void)
{
    UNITY_UINT v0, v1;
    UNITY_UINT *p0, *p1;

    v0 = 0;
    v1 = 1;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_UINT(v0, v1);
    TEST_ASSERT_GREATER_THAN_UINT(*p0, v1);
    TEST_ASSERT_GREATER_THAN_UINT(v0, *p1);
    TEST_ASSERT_GREATER_THAN_UINT(*p0, *p1);
}

void testNotGreaterThanUINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_UINT(1, 0);
    VERIFY_FAILS_END
}

void testGreaterThanUINT8(void)
{
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

    v0 = 0;
    v1 = 255;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_UINT8(v0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(v0, *p1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, *p1);
}

void testNotGreaterThanUINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_UINT8(255, 0);
    VERIFY_FAILS_END
}

void testGreaterThanUINT16(void)
{
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

    v0 = 0;
    v1 = 65535;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_UINT16(v0, v1);
    TEST_ASSERT_GREATER_THAN_UINT16(*p0, v1);
    TEST_ASSERT_GREATER_THAN_UINT16(v0, *p1);
    TEST_ASSERT_GREATER_THAN_UINT16(*p0, *p1);
}

void testNotGreaterThanUINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_UINT16(65535, 0);
    VERIFY_FAILS_END
}

void testGreaterThanUINT32(void)
{
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

    v0 = 0u;
    v1 = 4294967295u;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_UINT32(v0, v1);
    TEST_ASSERT_GREATER_THAN_UINT32(*p0, v1);
    TEST_ASSERT_GREATER_THAN_UINT32(v0, *p1);
    TEST_ASSERT_GREATER_THAN_UINT32(*p0, *p1);
}

void testNotGreaterThanUINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_UINT32(4294967295u, 0);
    VERIFY_FAILS_END
}

void testGreaterThanHEX8(void)
{
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

    v0 = 0x00;
    v1 = 0xFF;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_HEX8(v0, v1);
    TEST_ASSERT_GREATER_THAN_HEX8(*p0, v1);
    TEST_ASSERT_GREATER_THAN_HEX8(v0, *p1);
    TEST_ASSERT_GREATER_THAN_HEX8(*p0, *p1);
}

void testNotGreaterThanHEX8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_HEX8(0xFF, 0x00);
    VERIFY_FAILS_END
}

void testGreaterThanHEX16(void)
{
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

    v0 = 0x0000;
    v1 = 0xFFFF;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_HEX16(v0, v1);
    TEST_ASSERT_GREATER_THAN_HEX16(*p0, v1);
    TEST_ASSERT_GREATER_THAN_HEX16(v0, *p1);
    TEST_ASSERT_GREATER_THAN_HEX16(*p0, *p1);
}

void testNotGreaterThanHEX16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_HEX16(0xFFFF, 0x00);
    VERIFY_FAILS_END
}

void testGreaterThanHEX32(void)
{
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

    v0 = 0x00000000;
    v1 = 0xFFFFFFFF;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_HEX32(v0, v1);
    TEST_ASSERT_GREATER_THAN_HEX32(*p0, v1);
    TEST_ASSERT_GREATER_THAN_HEX32(v0, *p1);
    TEST_ASSERT_GREATER_THAN_HEX32(*p0, *p1);
}

void testNotGreaterThanHEX32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_HEX32(0xFFFFFFFF, 0x00);
    VERIFY_FAILS_END
}

void testGreaterOrEqual(void)
{
    UNITY_INT v0, v1, v2;
    UNITY_INT *p0, *p1, *p2;

    v0 = 0;
    v1 = 1;
    v2 = 0;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL(*p0, *p2);
}

void testNotGreaterOrEqual(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL(0, -1);
    VERIFY_FAILS_END
}

void testGreaterOrEqualINT(void)
{
    UNITY_INT v0, v1, v2;
    UNITY_INT *p0, *p1, *p2;

    v0 = 302;
    v1 = 3334;
    v2 = 302;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_INT(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(*p0, *p2);
}

void testNotGreaterOrEqualINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_INT(3334, 302);
    VERIFY_FAILS_END
}

void testGreaterOrEqualINT8(void)
{
    UNITY_INT8 v0, v1, v2;
    UNITY_INT8 *p0, *p1, *p2;

    v0 = -128;
    v1 = 127;
    v2 = -128;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_INT8(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(*p0, *p2);
}

void testNotGreaterOrEqualINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_INT8(127, -128);
    VERIFY_FAILS_END
}

void testGreaterOrEqualCHAR(void)
{
    char v0, v1, v2;
    char *p0, *p1, *p2;

    v0 = -128;
    v1 = 127;
    v2 = -128;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(*p0, *p2);
}

void testNotGreaterOrEqualCHAR(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_CHAR(127, -128);
    VERIFY_FAILS_END
}

void testGreaterOrEqualINT16(void)
{
    UNITY_INT16 v0, v1, v2;
    UNITY_INT16 *p0, *p1, *p2;

    v0 = -32768;
    v1 = 32767;
    v2 = -32768;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_INT16(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(*p0, *p2);
}

void testNotGreaterOrEqualINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_INT16(32767, -32768);
    VERIFY_FAILS_END
}

void testGreaterOrEqualINT32(void)
{
    UNITY_INT32 v0, v1, v2;
    UNITY_INT32 *p0, *p1, *p2;

    v0 = -214783648;
    v1 = 214783647;
    v2 = -214783648;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_INT32(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(*p0, *p2);
}

void testNotGreaterOrEqualINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(214783647, -214783648);
    VERIFY_FAILS_END
}

void testGreaterOrEqualUINT(void)
{
    UNITY_UINT v0, v1, v2;
    UNITY_UINT *p0, *p1, *p2;

    v0 = 0;
    v1 = 1;
    v2 = 0;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_UINT(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(*p0, *p2);
}

void testNotGreaterOrEqualUINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(1, 0);
    VERIFY_FAILS_END
}

void testGreaterOrEqualUINT8(void)
{
    UNITY_UINT8 v0, v1, v2;
    UNITY_UINT8 *p0, *p1, *p2;

    v0 = 0;
    v1 = 255;
    v2 = 0;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(*p0, *p2);
}

void testNotGreaterOrEqualUINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(255, 0);
    VERIFY_FAILS_END
}

void testGreaterOrEqualUINT16(void)
{
    UNITY_UINT16 v0, v1, v2;
    UNITY_UINT16 *p0, *p1, *p2;

    v0 = 0;
    v1 = 65535;
    v2 = 0;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(*p0, *p2);
}

void testNotGreaterOrEqualUINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_UINT16(65535, 0);
    VERIFY_FAILS_END
}

void testGreaterOrEqualUINT32(void)
{
    UNITY_UINT32 v0, v1, v2;
    UNITY_UINT32 *p0, *p1, *p2;

    v0 = 0;
    v1 = 4294967295u;
    v2 = 0;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(*p0, *p2);
}

void testNotGreaterOrEqualUINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(4294967295u, 0);
    VERIFY_FAILS_END
}

void testGreaterOrEqualHEX8(void)
{
    UNITY_UINT8 v0, v1, v2;
    UNITY_UINT8 *p0, *p1, *p2;

    v0 = 0x00;
    v1 = 0xFF;
    v2 = 0x00;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(*p0, *p2);
}

void testNotGreaterOrEqualHEX8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_HEX8(0xFF, 0x00);
    VERIFY_FAILS_END
}

void testGreaterOrEqualHEX16(void)
{
    UNITY_UINT16 v0, v1, v2;
    UNITY_UINT16 *p0, *p1, *p2;

    v0 = 0x0000;
    v1 = 0xFFFF;
    v2 = 0x0000;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(*p0, *p2);
}

void testNotGreaterOrEqualHEX16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_HEX16(0xFFFF, 0x00);
    VERIFY_FAILS_END
}

void testGreaterOrEqualHEX32(void)
{
    UNITY_UINT32 v0, v1, v2;
    UNITY_UINT32 *p0, *p1, *p2;

    v0 = 0x00000000;
    v1 = 0xFFFFFFFF;
    v2 = 0x00000000;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(v0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(*p0, v1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(v0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(*p0, *p1);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(v0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(*p0, v2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(v0, *p2);
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(*p0, *p2);
}

void testNotGreaterOrEqualHEX32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_OR_EQUAL_HEX32(0xFFFFFFFF, 0x00);
    VERIFY_FAILS_END
}

/*-----------------*/

void testLessThan(void)
{
    UNITY_INT v0, v1;
    UNITY_INT *p0, *p1;

    v0 = 0;
    v1 = -1;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN(v0, v1);
    TEST_ASSERT_LESS_THAN(*p0, v1);
    TEST_ASSERT_LESS_THAN(v0, *p1);
    TEST_ASSERT_LESS_THAN(*p0, *p1);
}

void testNotLessThan(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(0, 1);
    VERIFY_FAILS_END
}

void testLessThanINT(void)
{
    UNITY_INT v0, v1;
    UNITY_INT *p0, *p1;

    v0 = 3334;
    v1 = 302;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_INT(v0, v1);
    TEST_ASSERT_LESS_THAN_INT(*p0, v1);
    TEST_ASSERT_LESS_THAN_INT(v0, *p1);
    TEST_ASSERT_LESS_THAN_INT(*p0, *p1);
}

void testNotLessThanINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT(302, 3334);
    VERIFY_FAILS_END
}

void testLessThanINT8(void)
{
    UNITY_INT8 v0, v1;
    UNITY_INT8 *p0, *p1;

    v0 = 127;
    v1 = -128;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_INT8(v0, v1);
    TEST_ASSERT_LESS_THAN_INT8(*p0, v1);
    TEST_ASSERT_LESS_THAN_INT8(v0, *p1);
    TEST_ASSERT_LESS_THAN_INT8(*p0, *p1);
}

void testNotLessThanINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT8(-128, 127);
    VERIFY_FAILS_END
}

void testLessThanCHAR(void)
{
    char v0, v1;
    char *p0, *p1;

    v0 = 127;
    v1 = -128;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_CHAR(v0, v1);
    TEST_ASSERT_LESS_THAN_CHAR(*p0, v1);
    TEST_ASSERT_LESS_THAN_CHAR(v0, *p1);
    TEST_ASSERT_LESS_THAN_CHAR(*p0, *p1);
}

void testNotLessThanCHAR(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_CHAR(-128, 127);
    VERIFY_FAILS_END
}

void testLessThanINT16(void)
{
    UNITY_INT16 v0, v1;
    UNITY_INT16 *p0, *p1;

    v0 = 32767;
    v1 = -32768;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_INT16(v0, v1);
    TEST_ASSERT_LESS_THAN_INT16(*p0, v1);
    TEST_ASSERT_LESS_THAN_INT16(v0, *p1);
    TEST_ASSERT_LESS_THAN_INT16(*p0, *p1);
}

void testNotLessThanINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT16(-32768, 32767);
    VERIFY_FAILS_END
}

void testLessThanINT32(void)
{
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

    v0 = 214783647;
    v1 = -214783648;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_INT32(v0, v1);
    TEST_ASSERT_LESS_THAN_INT32(*p0, v1);
    TEST_ASSERT_LESS_THAN_INT32(v0, *p1);
    TEST_ASSERT_LESS_THAN_INT32(*p0, *p1);
}

void testNotLessThanINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT32(-214783648, 214783647);
    VERIFY_FAILS_END
}

void testLessThanUINT(void)
{
    UNITY_UINT v0, v1;
    UNITY_UINT *p0, *p1;

    v0 = 1;
    v1 = 0;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_UINT(v0, v1);
    TEST_ASSERT_LESS_THAN_UINT(*p0, v1);
    TEST_ASSERT_LESS_THAN_UINT(v0, *p1);
    TEST_ASSERT_LESS_THAN_UINT(*p0, *p1);
}

void testNotLessThanUINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_UINT(0, 1);
    VERIFY_FAILS_END
}

void testLessThanUINT8(void)
{
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

    v0 = 255;
    v1 = 0;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_UINT8(v0, v1);
    TEST_ASSERT_LESS_THAN_UINT8(*p0, v1);
    TEST_ASSERT_LESS_THAN_UINT8(v0, *p1);
    TEST_ASSERT_LESS_THAN_UINT8(*p0, *p1);
}

void testNotLessThanUINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_UINT8(0, 255);
    VERIFY_FAILS_END
}

void testLessThanUINT16(void)
{
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

    v0 = 65535;
    v1 = 0;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_UINT16(v0, v1);
    TEST_ASSERT_LESS_THAN_UINT16(*p0, v1);
    TEST_ASSERT_LESS_THAN_UINT16(v0, *p1);
    TEST_ASSERT_LESS_THAN_UINT16(*p0, *p1);
}

void testNotLessThanUINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_UINT16(0, 65535);
    VERIFY_FAILS_END
}

void testLessThanUINT32(void)
{
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

    v0 = 4294967295u;
    v1 = 0;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_UINT32(v0, v1);
    TEST_ASSERT_LESS_THAN_UINT32(*p0, v1);
    TEST_ASSERT_LESS_THAN_UINT32(v0, *p1);
    TEST_ASSERT_LESS_THAN_UINT32(*p0, *p1);
}

void testNotLessThanUINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_UINT32(0, 4294967295u);
    VERIFY_FAILS_END
}

void testLessThanHEX8(void)
{
    UNITY_UINT8 v0, v1;
    UNITY_UINT8 *p0, *p1;

    v0 = 0xFF;
    v1 = 0x00;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_HEX8(v0, v1);
    TEST_ASSERT_LESS_THAN_HEX8(*p0, v1);
    TEST_ASSERT_LESS_THAN_HEX8(v0, *p1);
    TEST_ASSERT_LESS_THAN_HEX8(*p0, *p1);
}

void testNotLessThanHEX8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_HEX8(0x00, 0xFF);
    VERIFY_FAILS_END
}

void testLessThanHEX16(void)
{
    UNITY_UINT16 v0, v1;
    UNITY_UINT16 *p0, *p1;

    v0 = 0xFFFF;
    v1 = 0x0000;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_HEX16(v0, v1);
    TEST_ASSERT_LESS_THAN_HEX16(*p0, v1);
    TEST_ASSERT_LESS_THAN_HEX16(v0, *p1);
    TEST_ASSERT_LESS_THAN_HEX16(*p0, *p1);
}

void testNotLessThanHEX16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_HEX16(0x0000, 0xFFFF);
    VERIFY_FAILS_END
}

void testLessThanHEX32(void)
{
    UNITY_UINT32 v0, v1;
    UNITY_UINT32 *p0, *p1;

    v0 = 0xFFFFFFFF;
    v1 = 0x00000000;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_LESS_THAN_HEX32(v0, v1);
    TEST_ASSERT_LESS_THAN_HEX32(*p0, v1);
    TEST_ASSERT_LESS_THAN_HEX32(v0, *p1);
    TEST_ASSERT_LESS_THAN_HEX32(*p0, *p1);
}

void testNotLessThanHEX32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_HEX32(0x00000000, 0xFFFFFFFF);
    VERIFY_FAILS_END
}

void testLessOrEqual(void)
{
    UNITY_INT v0, v1, v2;
    UNITY_INT *p0, *p1, *p2;

    v0 = 0;
    v1 = -1;
    v2 = 0;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL(*p0, *p2);
}

void testNotLessOrEqual(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL(0, 1);
    VERIFY_FAILS_END
}

void testLessOrEqualINT(void)
{
    UNITY_INT v0, v1, v2;
    UNITY_INT *p0, *p1, *p2;

    v0 = 3334;
    v1 = 302;
    v2 = 3334;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_INT(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_INT(*p0, *p2);
}

void testNotLessOrEqualINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_INT(302, 3334);
    VERIFY_FAILS_END
}

void testLessOrEqualINT8(void)
{
    UNITY_INT8 v0, v1, v2;
    UNITY_INT8 *p0, *p1, *p2;

    v0 = 127;
    v1 = -128;
    v2 = 127;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_INT8(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT8(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT8(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT8(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT8(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT8(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT8(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_INT8(*p0, *p2);
}

void testNotLessOrEqualINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_INT8(-128, 127);
    VERIFY_FAILS_END
}

void testLessOrEqualCHAR(void)
{
    char v0, v1, v2;
    char *p0, *p1, *p2;

    v0 = 127;
    v1 = -128;
    v2 = 127;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_CHAR(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(*p0, *p2);
}

void testNotLessOrEqualCHAR(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_CHAR(-128, 127);
    VERIFY_FAILS_END
}

void testLessOrEqualINT16(void)
{
    UNITY_INT16 v0, v1, v2;
    UNITY_INT16 *p0, *p1, *p2;

    v0 = 32767;
    v1 = -32768;
    v2 = 32767;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_INT16(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT16(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT16(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT16(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT16(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT16(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_INT16(*p0, *p2);
}

void testNotLessOrEqualINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_INT16(-32768, 32767);
    VERIFY_FAILS_END
}

void testLessOrEqualINT32(void)
{
    UNITY_INT32 v0, v1, v2;
    UNITY_INT32 *p0, *p1, *p2;

    v0 = 214783647;
    v1 = -214783648;
    v2 = 214783647;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_INT32(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_INT32(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT32(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_INT32(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT32(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_INT32(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_INT32(*p0, *p2);
}

void testNotLessOrEqualINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_INT32(-214783648, 214783647);
    VERIFY_FAILS_END
}

void testLessOrEqualUINT(void)
{
    UNITY_UINT v0, v1, v2;
    UNITY_UINT *p0, *p1, *p2;

    v0 = 1;
    v1 = 0;
    v2 = 1;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_UINT(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(*p0, *p2);
}

void testNotLessOrEqualUINT(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_UINT(0, 1);
    VERIFY_FAILS_END
}

void testLessOrEqualUINT8(void)
{
    UNITY_UINT8 v0, v1, v2;
    UNITY_UINT8 *p0, *p1, *p2;

    v0 = 255;
    v1 = 0;
    v2 = 255;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_UINT8(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(*p0, *p2);
}

void testNotLessOrEqualUINT8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_UINT8(0, 255);
    VERIFY_FAILS_END
}

void testLessOrEqualUINT16(void)
{
    UNITY_UINT16 v0, v1, v2;
    UNITY_UINT16 *p0, *p1, *p2;

    v0 = 65535;
    v1 = 0;
    v2 = 65535;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_UINT16(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(*p0, *p2);
}

void testNotLessOrEqualUINT16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_UINT16(0, 65535);
    VERIFY_FAILS_END
}

void testLessOrEqualUINT32(void)
{
    UNITY_UINT32 v0, v1, v2;
    UNITY_UINT32 *p0, *p1, *p2;

    v0 = 4294967295u;
    v1 = 0;
    v2 = 4294967295u;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_UINT32(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(*p0, *p2);
}

void testNotLessOrEqualUINT32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(0, 4294967295u);
    VERIFY_FAILS_END
}

void testLessOrEqualHEX8(void)
{
    UNITY_UINT8 v0, v1, v2;
    UNITY_UINT8 *p0, *p1, *p2;

    v0 = 0xFF;
    v1 = 0x00;
    v2 = 0xFF;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_HEX8(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(*p0, *p2);
}

void testNotLessOrEqualHEX8(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_HEX8(0x00, 0xFF);
    VERIFY_FAILS_END
}

void testLessOrEqualHEX16(void)
{
    UNITY_UINT16 v0, v1, v2;
    UNITY_UINT16 *p0, *p1, *p2;

    v0 = 0xFFFF;
    v1 = 0x0000;
    v2 = 0xFFFF;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_HEX16(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(*p0, *p2);
}

void testNotLessOrEqualHEX16(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_HEX16(0x0000, 0xFFFF);
    VERIFY_FAILS_END
}

void testLessOrEqualHEX32(void)
{
    UNITY_UINT32 v0, v1, v2;
    UNITY_UINT32 *p0, *p1, *p2;

    v0 = 0xFFFFFFFF;
    v1 = 0x00000000;
    v2 = 0xFFFFFFFF;
    p0 = &v0;
    p1 = &v1;
    p2 = &v2;

    TEST_ASSERT_LESS_OR_EQUAL_HEX32(v0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(*p0, v1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(v0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(*p0, *p1);
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(v0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(*p0, v2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(v0, *p2);
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(*p0, *p2);
}

void testNotLessOrEqualHEX32(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_OR_EQUAL_HEX32(0x00000000, 0xFFFFFFFF);
    VERIFY_FAILS_END
}

void testHexPrintsUpToMaxNumberOfNibbles(void)
{
#ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
#else
    startPutcharSpy();
    UnityPrintNumberHex(0xBEE, 21);
    endPutcharSpy();
#ifdef UNITY_SUPPORT_64
    TEST_ASSERT_EQUAL_INT(16, strlen(getBufferPutcharSpy()));
#else
    TEST_ASSERT_EQUAL_INT( 8, strlen(getBufferPutcharSpy()));
#endif
#endif
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
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-2147483648", (UNITY_INT32)0x80000000);
    TEST_ASSERT_EQUAL_PRINT_NUMBERS("-1",          (UNITY_INT32)0xFFFFFFFF);
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
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("2147483648", (UNITY_UINT32)0x80000000);
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("4294967295", (UNITY_UINT32)0xFFFFFFFF);
#endif
}
