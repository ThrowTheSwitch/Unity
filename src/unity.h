#ifndef UNITY_FRAMEWORK_H
#define UNITY_FRAMEWORK_H

#define UNITY

#include <stdio.h>
#include <setjmp.h>

typedef void (*UnityTestFunction)(void);

typedef enum 
{
    UNITY_DISPLAY_STYLE_INT,
    UNITY_DISPLAY_STYLE_UINT,
    UNITY_DISPLAY_STYLE_HEX8,
    UNITY_DISPLAY_STYLE_HEX16,
    UNITY_DISPLAY_STYLE_HEX32
} UNITY_DISPLAY_STYLE_T;

struct _Unity
{
    char* CurrentTestName;
    unsigned char NumberOfTests;
    unsigned char TestFailures;
    unsigned char TestIgnores;
    unsigned char CurrentTestFailed;
    unsigned char CurrentTestIgnored;
    const char *TestFile;
    float DefaultDelta;
    jmp_buf AbortFrame;
};

extern struct _Unity Unity;

void CreateResults();

void UnityBegin();
void UnityEnd(void);
int  UnityGetNumFailures(void);

void UnityPrintChar(char ch);
void UnityPrint(const char *string);
void UnityPrintMask(unsigned long mask, unsigned long number);
void UnityPrintNumberByStyle(long number, UNITY_DISPLAY_STYLE_T style);
void UnityPrintNumber(long number);
void UnityPrintNumberUnsigned(unsigned long number);
void UnityPrintNumberHex(unsigned long number, char nibbles);
void UnityConcludeTest();

void UnityAssertEqualInt(int expected, int actual,
const char *msg, unsigned short lineNumber, UNITY_DISPLAY_STYLE_T style);

void UnityAssertBits(int mask, int expected, int actual,
const char *msg, unsigned short lineNumber);

void UnityAssertEqualString(const char *expected, const char *actual,
const char *msg, unsigned short lineNumber );

void UnityAssertFloatsWithin(float delta, float expected, float actual,
const char *msg, unsigned short lineNumber);

void UnityAssertIntsWithin(int delta, int expected, int actual,
const char *msg, unsigned short lineNumber);

void UnityFail(const char *message, int line);

void UnityIgnore(const char *message, int line);

#define TEST_PROTECT() (setjmp(Unity.AbortFrame) == 0)

#define TEST_ABORT() {longjmp(Unity.AbortFrame, 1);}

#define ABORT_IF_NECESSARY() \
    if( Unity.CurrentTestFailed || Unity.CurrentTestIgnored ) {TEST_ABORT();}

#define RUN_TEST(func) \
    Unity.CurrentTestName = #func; \
    Unity.NumberOfTests ++; \
    runTest(func); \
    UnityConcludeTest();

#define TEST_ASSERT_MESSAGE(condition, message) if (condition) {} else {TEST_FAIL(message);}
#define TEST_ASSERT(condition) TEST_ASSERT_MESSAGE(condition, NULL)

#define TEST_ASSERT_TRUE_MESSAGE(condition) TEST_ASSERT_MESSAGE(condition, message)
#define TEST_ASSERT_TRUE(condition) TEST_ASSERT(condition)

#define TEST_ASSERT_UNLESS_MESSAGE(condition) TEST_ASSERT_MESSAGE(!(condition), message)
#define TEST_ASSERT_UNLESS(condition) TEST_ASSERT(!(condition))

#define TEST_ASSERT_FALSE_MESSAGE(condition) TEST_ASSERT_MESSAGE(!(condition), message)
#define TEST_ASSERT_FALSE(condition) TEST_ASSERT(!(condition))

#define TEST_ASSERT_NULL(pointer) TEST_ASSERT_MESSAGE(pointer == NULL, #pointer " was not null.")

#define TEST_ASSERT_NOT_NULL(pointer) TEST_ASSERT_MESSAGE(pointer != NULL, #pointer " was null.")

#define TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualInt((int)(expected), (int)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_INT); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_INT(expected, actual)	TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_MESSAGE(expected, actual, message) TEST_ASSERT_EQUAL_INT_MESSAGE((expected), (actual), (message))
#define TEST_ASSERT_EQUAL(expected, actual) TEST_ASSERT_EQUAL_INT(expected, actual)

#define TEST_ASSERT_INT_WITHIN_MESSAGE(delta, expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertIntsWithin((delta), (expected), (actual), NULL, (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_INT_WITHIN(delta, expected, actual) TEST_ASSERT_INT_WITHIN_MESSAGE(delta, expected, actual, NULL)

#define TEST_ASSERT_EQUAL_UINT_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualInt((int)(expected), (int)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_UINT); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_UINT(expected, actual) TEST_ASSERT_EQUAL_UINT_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualInt((int)(expected), (int)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX8); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX8(expected, actual) TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_HEX16_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualInt((int)(expected), (int)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX16); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX16(expected, actual)	TEST_ASSERT_EQUAL_HEX16_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualInt((int)(expected), (int)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX32); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX32(expected, actual)	TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_HEX_MESSAGE(expected, actual, message) TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected, actual, message)
#define TEST_ASSERT_EQUAL_HEX(expected, actual) TEST_ASSERT_EQUAL_HEX32(expected, actual)

#define TEST_ASSERT_BITS_MESSAGE(mask, expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertBits((mask), (expected), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_BITS(mask, expected, actual) TEST_ASSERT_BITS_MESSAGE(mask, expected, actual, NULL)

#define TEST_ASSERT_BITS_HIGH_MESSAGE(mask, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertBits((mask), (-1), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_BITS_HIGH(mask, actual) TEST_ASSERT_BITS_HIGH_MESSAGE(mask, actual, NULL)

#define TEST_ASSERT_BITS_LOW_MESSAGE(mask, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertBits((mask), (0), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_BITS_LOW(mask, actual) TEST_ASSERT_BITS_LOW_MESSAGE(mask, actual, NULL)

#define TEST_ASSERT_BIT_HIGH_MESSAGE(bit, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertBits((1 << bit), (-1), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_BIT_HIGH(bit, actual) TEST_ASSERT_BIT_HIGH_MESSAGE(bit, actual, NULL)

#define TEST_ASSERT_BIT_LOW_MESSAGE(bit, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertBits((1 << bit), (0), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_BIT_LOW(bit, actual) TEST_ASSERT_BIT_LOW_MESSAGE(bit, actual, NULL)

#define TEST_ASSERT_FLOAT_WITHIN_MESSAGE(delta, expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertFloatsWithin((delta), (expected), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual) TEST_ASSERT_FLOAT_WITHIN_MESSAGE(delta, expected, actual, NULL)

#define TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualString((expected), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_STRING(expected, actual) TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, NULL)

#define TEST_FAIL(message) { Unity.TestFile=__FILE__; UnityFail((message), (unsigned short)__LINE__); TEST_ABORT(); }
#define TEST_IGNORE_MESSAGE(message) { Unity.TestFile=__FILE__; UnityIgnore((message), (unsigned short)__LINE__); TEST_ABORT(); }
#define TEST_IGNORE() TEST_IGNORE_MESSAGE(NULL)

#endif

