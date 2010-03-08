#ifndef UNITY_FRAMEWORK_H
#define UNITY_FRAMEWORK_H

#define UNITY

#include <stdio.h>
#include <setjmp.h>

//-------------------------------------------------------
// Float Support
//-------------------------------------------------------
// define UNITY_EXCLUDE_FLOAT to disallow floating point comparisons
// define UNITY_FLOAT_DELTA to specify the precision to use when doing TEST_ASSERT_EQUAL_FLOAT
// define UNITY_FLOAT_TYPE to specify doubles instead of single precision floats

#ifndef UNITY_EXCLUDE_FLOAT
#ifndef UNITY_FLOAT_PRECISION
#define UNITY_FLOAT_PRECISION (0.00001f)
#endif
#ifndef UNITY_FLOAT_TYPE
#define UNITY_FLOAT_TYPE float
#endif
    typedef UNITY_FLOAT_TYPE _UF;
#endif

//-------------------------------------------------------
// Int Support
//-------------------------------------------------------
// Unity assumes 32 bit integers by default
// If your compiler treats ints of a different size, define UNITY_INT_WIDTH

#ifndef UNITY_INT_WIDTH
#define UNITY_INT_WIDTH (32)
#endif
#if (UNITY_INT_WIDTH == 32)
    typedef unsigned char   _UU8;
    typedef unsigned short  _UU16;
    typedef unsigned int    _UU32;
    typedef signed char     _US8;
    typedef signed short    _US16;
    typedef signed int      _US32;
#elif (UNITY_INT_WIDTH == 16)
    typedef unsigned char   _UU8;
    typedef unsigned int    _UU16;
    typedef unsigned long   _UU32;
    typedef signed char     _US8;
    typedef signed int      _US16;
    typedef signed long     _US32;               
#else
    #error Invalid UNITY_INT_WIDTH specified! (32 or 16 only are currently supported)
#endif

//-------------------------------------------------------
// Output Method
//-------------------------------------------------------

#ifndef UNITY_OUTPUT_CHAR
#define UNITY_OUTPUT_CHAR(a) putchar(a)
#endif

//-------------------------------------------------------
// Internal Structs Needed
//-------------------------------------------------------

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
    const char* TestFile;
    const char* CurrentTestName;
    unsigned char NumberOfTests;
    unsigned char TestFailures;
    unsigned char TestIgnores;
    unsigned char CurrentTestFailed;
    unsigned char CurrentTestIgnored;
    jmp_buf AbortFrame;
};

extern struct _Unity Unity;

//-------------------------------------------------------
// Test Suite Management
//-------------------------------------------------------

void UnityBegin(void);
void UnityEnd(void);
void UnityConcludeTest(void);

//-------------------------------------------------------
// Test Output
//-------------------------------------------------------

void UnityPrint(const char* string);
void UnityPrintMask(const unsigned long mask, const unsigned long number);
void UnityPrintNumberByStyle(const long number, const UNITY_DISPLAY_STYLE_T style);
void UnityPrintNumber(const long number);
void UnityPrintNumberUnsigned(const unsigned long number);
void UnityPrintNumberHex(const unsigned long number, const char nibbles);

//-------------------------------------------------------
// Test Assertion Fuctions
//-------------------------------------------------------
//  Use the macros below this section instead of calling
//  these directly. The macros have a consistent naming
//  convention and will pull in file and line information
//  for you.

void UnityAssertEqualNumber(const long expected,
                            const long actual,
                            const char* msg,
                            const unsigned short lineNumber,
                            const UNITY_DISPLAY_STYLE_T style);
                         
void UnityAssertEqualNumberUnsigned(const unsigned long expected,
                                    const unsigned long actual,
                                    const char* msg,
                                    const unsigned short lineNumber,
                                    const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualIntArray(const int* expected,
                              const int* actual,
                              const unsigned long num_elements,
                              const char* msg,
                              const unsigned short lineNumber,
                              const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualUnsignedIntArray(const unsigned int* expected,
                              const unsigned int* actual,
                              const unsigned long num_elements,
                              const char* msg,
                              const unsigned short lineNumber,
                              const UNITY_DISPLAY_STYLE_T style);

void UnityAssertBits(const long mask,
                     const long expected,
                     const long actual,
                     const char* msg,
                     const unsigned short lineNumber);

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const unsigned short lineNumber );

void UnityAssertEqualMemory(const void* expected,
                            const void* actual,
                            unsigned long length,
                            const char* msg,
                            const unsigned short lineNumber );

void UnityAssertEqualMemoryArray(const void* expected,
                                 const void* actual,
                                 unsigned long length,
                                 unsigned long num_elements,
                                 const char* msg,
                                 const unsigned short lineNumber );

void UnityAssertNumbersWithin(const long delta,
                              const long expected,
                              const long actual,
                              const char* msg,
                              const unsigned short lineNumber);

void UnityAssertNumbersUnsignedWithin(const unsigned long delta,
                                      const unsigned long expected,
                                      const unsigned long actual,
                                      const char* msg,
                                      const unsigned short lineNumber);

void UnityFail(const char* message, const long line);

void UnityIgnore(const char* message, const long line);

#ifndef UNITY_EXCLUDE_FLOAT
void UnityAssertFloatsWithin(const _UF delta,
                             const _UF expected,
                             const _UF actual,
                             const char* msg,
                             const unsigned short lineNumber);
#endif

//-------------------------------------------------------
// Test Running Macros
//-------------------------------------------------------

#define TEST_PROTECT() (setjmp(Unity.AbortFrame) == 0)

#define TEST_ABORT() {longjmp(Unity.AbortFrame, 1);}

#define ABORT_IF_NECESSARY() \
    if( Unity.CurrentTestFailed || Unity.CurrentTestIgnored ) {TEST_ABORT();}

#define RUN_TEST(func) \
    Unity.CurrentTestName = #func; \
    Unity.NumberOfTests ++; \
    runTest(func); \
    UnityConcludeTest();
    
//-------------------------------------------------------
// Test Asserts
//-------------------------------------------------------
// these are the macros you are looking for

#define TEST_ASSERT_MESSAGE(condition, message) if (condition) {} else {TEST_FAIL(message);}
#define TEST_ASSERT(condition) TEST_ASSERT_MESSAGE(condition, "Expectation Failed.")

#define TEST_ASSERT_TRUE_MESSAGE(condition, message) TEST_ASSERT_MESSAGE(condition, message)
#define TEST_ASSERT_TRUE(condition) TEST_ASSERT_MESSAGE(condition, "Expected TRUE was FALSE.")

#define TEST_ASSERT_UNLESS_MESSAGE(condition, message) TEST_ASSERT_MESSAGE(!(condition), message)
#define TEST_ASSERT_UNLESS(condition) TEST_ASSERT(!(condition))

#define TEST_ASSERT_FALSE_MESSAGE(condition, message) TEST_ASSERT_MESSAGE(!(condition), message)
#define TEST_ASSERT_FALSE(condition) TEST_ASSERT_MESSAGE(!(condition), "Expected FALSE was TRUE.")

#define TEST_ASSERT_NULL(pointer) TEST_ASSERT_MESSAGE(pointer == NULL, #pointer " was not null.")
#define TEST_ASSERT_NULL_MESSAGE(pointer, message) TEST_ASSERT_MESSAGE(pointer == NULL, message)
#define TEST_ASSERT_NOT_NULL(pointer) TEST_ASSERT_MESSAGE(pointer != NULL, #pointer " was null.")
#define TEST_ASSERT_NOT_NULL_MESSAGE(pointer, message) TEST_ASSERT_MESSAGE(pointer != NULL, message)

#define TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualNumber((long)(expected), (long)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_INT); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_INT(expected, actual)	TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected, actual, num_elements, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualIntArray((const int*)(expected), (const int*)(actual), (unsigned long)(num_elements), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_INT); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, num_elements) TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected, actual, num_elements, NULL)

#define TEST_ASSERT_EQUAL_MESSAGE(expected, actual, message) TEST_ASSERT_EQUAL_INT_MESSAGE((expected), (actual), (message))
#define TEST_ASSERT_EQUAL(expected, actual) TEST_ASSERT_EQUAL_INT(expected, actual)

#define TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, message) TEST_ASSERT_FALSE_MESSAGE(((expected) == (actual)), (message))
#define TEST_ASSERT_NOT_EQUAL(expected, actual) TEST_ASSERT_FALSE((expected) == (actual))

#define TEST_ASSERT_INT_WITHIN_MESSAGE(delta, expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertNumbersWithin((long)(delta), (long)(expected), (long)(actual), NULL, (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_INT_WITHIN(delta, expected, actual) TEST_ASSERT_INT_WITHIN_MESSAGE(delta, expected, actual, NULL)

#define TEST_ASSERT_EQUAL_UINT_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualNumberUnsigned((unsigned long)(expected), (unsigned long)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_UINT); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_UINT(expected, actual) TEST_ASSERT_EQUAL_UINT_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_UINT_ARRAY_MESSAGE(expected, actual, num_elements, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualUnsignedIntArray((const unsigned int*)(expected), (const unsigned int*)(actual), (unsigned long)(num_elements), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_UINT); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, num_elements) TEST_ASSERT_EQUAL_UINT_ARRAY_MESSAGE(expected, actual, num_elements, NULL)

#define TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualNumberUnsigned((unsigned long)(expected), (unsigned long)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX8); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX8(expected, actual) TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_HEX16_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualNumberUnsigned((unsigned long)(expected), (unsigned long)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX16); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX16(expected, actual)	TEST_ASSERT_EQUAL_HEX16_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualNumber((long)(expected), (long)(actual), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX32); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX32(expected, actual)	TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_HEX32_ARRAY_MESSAGE(expected, actual, num_elements, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualIntArray((const int*)(expected), (const int*)(actual), (unsigned long)(num_elements), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX32); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements) TEST_ASSERT_EQUAL_HEX32_ARRAY_MESSAGE(expected, actual, num_elements, NULL)

#define TEST_ASSERT_EQUAL_HEX_MESSAGE(expected, actual, message) TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected, actual, message)
#define TEST_ASSERT_EQUAL_HEX(expected, actual) TEST_ASSERT_EQUAL_HEX32(expected, actual)

#define TEST_ASSERT_EQUAL_HEX_ARRAY_MESSAGE(expected, actual, num_elements, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualIntArray((const int*)(expected), (const int*)(actual), (unsigned long)(num_elements), (message), (unsigned short)__LINE__, UNITY_DISPLAY_STYLE_HEX32); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_HEX_ARRAY(expected, actual, num_elements) TEST_ASSERT_EQUAL_HEX32_ARRAY_MESSAGE(expected, actual, num_elements, NULL)

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
    UnityAssertBits(((_UU32)1 << bit), (-1), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_BIT_HIGH(bit, actual) TEST_ASSERT_BIT_HIGH_MESSAGE(bit, actual, NULL)

#define TEST_ASSERT_BIT_LOW_MESSAGE(bit, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertBits(((_UU32)1 << bit), (0), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_BIT_LOW(bit, actual) TEST_ASSERT_BIT_LOW_MESSAGE(bit, actual, NULL)

#define TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualString((expected), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_STRING(expected, actual) TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, NULL)

#define TEST_ASSERT_EQUAL_MEMORY_MESSAGE(expected, actual, len, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualMemory((expected), (actual), (len), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_MEMORY(expected, actual, len) TEST_ASSERT_EQUAL_MEMORY_MESSAGE(expected, actual, len, NULL)

#define TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected, actual, len, num_elements, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertEqualMemoryArray((expected), (actual), (len), (num_elements), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, actual, len, num_elements) TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected, actual, len, num_elements, NULL)

#define TEST_FAIL(message) { Unity.TestFile=__FILE__; UnityFail((message), (unsigned short)__LINE__); TEST_ABORT(); }
#define TEST_IGNORE_MESSAGE(message) { Unity.TestFile=__FILE__; UnityIgnore((message), (unsigned short)__LINE__); TEST_ABORT(); }
#define TEST_IGNORE() TEST_IGNORE_MESSAGE(NULL)

#ifdef UNITY_EXCLUDE_FLOAT
#define TEST_ASSERT_FLOAT_WITHIN_MESSAGE(delta, expected, actual, message)  TEST_FAIL("Unity Floating Point Disabled");
#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual)                   TEST_FAIL("Unity Floating Point Disabled");
#define TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, message)          TEST_FAIL("Unity Floating Point Disabled");
#define TEST_ASSERT_EQUAL_FLOAT(expected, actual)                           TEST_FAIL("Unity Floating Point Disabled");
#else
#define TEST_ASSERT_FLOAT_WITHIN_MESSAGE(delta, expected, actual, message) \
    Unity.TestFile=__FILE__; \
    UnityAssertFloatsWithin((delta), (expected), (actual), (message), (unsigned short)__LINE__); \
    ABORT_IF_NECESSARY();
#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual) TEST_ASSERT_FLOAT_WITHIN_MESSAGE(delta, expected, actual, NULL)
#define TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, message) TEST_ASSERT_FLOAT_WITHIN_MESSAGE((expected) * UNITY_FLOAT_PRECISION, expected, actual, message)
#define TEST_ASSERT_EQUAL_FLOAT(expected, actual) TEST_ASSERT_FLOAT_WITHIN_MESSAGE((expected) * UNITY_FLOAT_PRECISION, expected, actual, NULL)
#endif

#endif
