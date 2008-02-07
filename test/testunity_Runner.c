#define UNITY_ENABLE_EXTERNAL_ASSERTIONS

#include <setjmp.h>
#include "unity.h"

void setUp(void);
void tearDown(void);

void testTrue(void);
void testFalse(void);
void testPreviousPass(void);
void testNotVanilla(void);
void testNotTrue(void);
void testNotFalse(void);
void testNotUnless(void);
void testFail(void);
void testIgnoreInWrapper(void);
void testIgnore(void);
void testIgnoreMessage(void);
void testIgnoreMessageInWrapper(void);
void testWrap(void);
void testNotEqualInts(void);
void testNotEqualBits(void);
void testNotEqualUInts(void);
void testNotEqualHex8s(void);
void testNotEqualHex16s(void);
void testNotEqualHex32s(void);
void testMultipleUnwrappedAssertionsHandledAppropriately(void);
void testEqualInts(void);
void testEqualUints(void);
void testEqualHex8s(void);
void testEqualHex16s(void);
void testEqualHex32s(void);
void testEqualBits(void);
void testEqualShorts(void);
void testEqualUShorts(void);
void testEqualChars(void);
void testEqualUChars(void);
void testEqualPointers(void);
void testFloatsWithinDelta(void);
void testFloatsNotWithinDelta(void);
void testIntsWithinDelta(void);
void testIntsNotWithinDelta(void);
void testEqualStrings(void);
void testNotEqualString1(void);
void testNotEqualString2(void);
void testNotEqualString3(void);
void testNotEqualString_ExpectedStringIsNull(void);
void testNotEqualString_ActualStringIsNull(void);
void testProtection(void);


static void runTest(UnityTestFunction test)
{
    setUp();
    test();
    tearDown();
}

int main(void)
{
    Unity.TestFile = __FILE__;
    UnityBegin();

    // RUN_TEST calls runTest
    RUN_TEST(testTrue);
    RUN_TEST(testFalse);
    RUN_TEST(testNotVanilla);
    RUN_TEST(testNotTrue);
    RUN_TEST(testNotFalse);
    RUN_TEST(testNotUnless);
    RUN_TEST(testPreviousPass);
    RUN_TEST(testFail);
    RUN_TEST(testWrap);
    RUN_TEST(testIgnoreInWrapper);
    RUN_TEST(testIgnore);
    RUN_TEST(testIgnoreMessage);
    RUN_TEST(testIgnoreMessageInWrapper);
    RUN_TEST(testNotEqualBits);
    RUN_TEST(testNotEqualInts);
    RUN_TEST(testNotEqualUInts);
    RUN_TEST(testNotEqualHex8s);
    RUN_TEST(testNotEqualHex16s);
    RUN_TEST(testNotEqualHex32s);
    RUN_TEST(testMultipleUnwrappedAssertionsHandledAppropriately);
    RUN_TEST(testEqualBits);
    RUN_TEST(testEqualInts);
    RUN_TEST(testEqualUints);
    RUN_TEST(testEqualHex8s);
    RUN_TEST(testEqualHex16s);
    RUN_TEST(testEqualHex32s);
    RUN_TEST(testEqualShorts);
    RUN_TEST(testEqualUShorts);
    RUN_TEST(testEqualChars);
    RUN_TEST(testEqualUChars);
    RUN_TEST(testEqualPointers);
    RUN_TEST(testEqualStrings);
    RUN_TEST(testIntsWithinDelta);
    RUN_TEST(testIntsNotWithinDelta);
    RUN_TEST(testFloatsWithinDelta);
    RUN_TEST(testFloatsNotWithinDelta);
    RUN_TEST(testNotEqualString1);
    RUN_TEST(testNotEqualString2);
    RUN_TEST(testNotEqualString3);
    RUN_TEST(testNotEqualString_ExpectedStringIsNull);
    RUN_TEST(testNotEqualString_ActualStringIsNull);
    RUN_TEST(testProtection);

    UnityEnd();
    return 0;
}
