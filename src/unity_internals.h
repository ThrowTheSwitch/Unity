/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

#include <stdio.h>
#include <setjmp.h>

//-------------------------------------------------------
// Int Support
//-------------------------------------------------------

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
// Float Support
//-------------------------------------------------------

#ifdef UNITY_EXCLUDE_FLOAT 

//No Floating Point Support
#undef UNITY_FLOAT_PRECISION
#undef UNITY_FLOAT_TYPE
#undef UNITY_FLOAT_VERBOSE

#else

//Floating Point Support
#ifndef UNITY_FLOAT_PRECISION
#define UNITY_FLOAT_PRECISION (0.00001f)
#endif
#ifndef UNITY_FLOAT_TYPE
#define UNITY_FLOAT_TYPE float
#endif
typedef UNITY_FLOAT_TYPE _UF;
    
#endif

//-------------------------------------------------------
// Output Method
//-------------------------------------------------------

#ifndef UNITY_OUTPUT_CHAR
#define UNITY_OUTPUT_CHAR(a) putchar(a)
#endif

//-------------------------------------------------------
// Footprint
//-------------------------------------------------------

#ifndef UNITY_LINE_TYPE
#define UNITY_LINE_TYPE unsigned short
#endif

#ifndef UNITY_COUNTER_TYPE
#define UNITY_COUNTER_TYPE unsigned short
#endif

//-------------------------------------------------------
// Internal Structs Needed
//-------------------------------------------------------

typedef void (*UnityTestFunction)(void);

typedef enum
{
    UNITY_DISPLAY_STYLE_INT,
    UNITY_DISPLAY_STYLE_INT8,
    UNITY_DISPLAY_STYLE_INT16,
    UNITY_DISPLAY_STYLE_INT32,
    UNITY_DISPLAY_STYLE_UINT,
    UNITY_DISPLAY_STYLE_UINT8,
    UNITY_DISPLAY_STYLE_UINT16,
    UNITY_DISPLAY_STYLE_UINT32,
    UNITY_DISPLAY_STYLE_HEX8,
    UNITY_DISPLAY_STYLE_HEX16,
    UNITY_DISPLAY_STYLE_HEX32
} UNITY_DISPLAY_STYLE_T;

struct _Unity
{
    const char* TestFile;
    const char* CurrentTestName;
    _UU32 CurrentTestLineNumber;
    UNITY_COUNTER_TYPE NumberOfTests;
    UNITY_COUNTER_TYPE TestFailures;
    UNITY_COUNTER_TYPE TestIgnores;
    UNITY_COUNTER_TYPE CurrentTestFailed;
    UNITY_COUNTER_TYPE CurrentTestIgnored;
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
void UnityPrintMask(const _UU32 mask, const _UU32 number);
void UnityPrintNumberByStyle(const _US32 number, const UNITY_DISPLAY_STYLE_T style);
void UnityPrintNumber(const _US32 number);
void UnityPrintNumberUnsigned(const _UU32 number);
void UnityPrintNumberHex(const _UU32 number, const char nibbles);

#ifdef UNITY_FLOAT_VERBOSE
void UnityPrintFloat(const _UF number);
#endif

//-------------------------------------------------------
// Test Assertion Fuctions
//-------------------------------------------------------
//  Use the macros below this section instead of calling
//  these directly. The macros have a consistent naming
//  convention and will pull in file and line information
//  for you.

void UnityAssertEqualNumber(const _US32 expected,
                            const _US32 actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber,
                            const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualIntArray(const int* expected,
                              const int* actual,
                              const _UU32 num_elements,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber,
                              const UNITY_DISPLAY_STYLE_T style);

void UnityAssertBits(const _US32 mask,
                     const _US32 expected,
                     const _US32 actual,
                     const char* msg,
                     const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualStringArray( const char** expected,
                                  const char** actual,
                                  const _UU32 num_elements,
                                  const char* msg,
                                  const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualMemory( const void* expected,
                             const void* actual,
                             const _UU32 length,
                             const _UU32 num_elements,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber);

void UnityAssertNumbersWithin(const _US32 delta,
                              const _US32 expected,
                              const _US32 actual,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber,
                              const UNITY_DISPLAY_STYLE_T style);

void UnityFail(const char* message, const UNITY_LINE_TYPE line);

void UnityIgnore(const char* message, const UNITY_LINE_TYPE line);

#ifndef UNITY_EXCLUDE_FLOAT
void UnityAssertFloatsWithin(const _UF delta,
                             const _UF expected,
                             const _UF actual,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualFloatArray(const _UF* expected,
                                const _UF* actual,
                                const _UU32 num_elements,
                                const char* msg,
                                const UNITY_LINE_TYPE lineNumber);
#endif

//-------------------------------------------------------
// Basic Fail and Ignore
//-------------------------------------------------------

#define UNITY_TEST_FAIL(line, message)   { UnityFail(   (message), (UNITY_LINE_TYPE)line); }
#define UNITY_TEST_IGNORE(line, message) { UnityIgnore( (message), (UNITY_LINE_TYPE)line); }

//-------------------------------------------------------
// Test Asserts
//-------------------------------------------------------

#define UNITY_TEST_ASSERT(condition, line, message)                                              if (condition) {} else {UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, message);}
#define UNITY_TEST_ASSERT_NULL(pointer, line, message)                                           UNITY_TEST_ASSERT(((pointer) == NULL),  (UNITY_LINE_TYPE)line, message)
#define UNITY_TEST_ASSERT_NOT_NULL(pointer, line, message)                                       UNITY_TEST_ASSERT(((pointer) != NULL),  (UNITY_LINE_TYPE)line, message)

#define UNITY_TEST_ASSERT_EQUAL_INT(expected, actual, line, message)                             UnityAssertEqualNumber((_US32)(expected), (_US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_EQUAL_UINT(expected, actual, line, message)                            UnityAssertEqualNumber((_US32)(expected), (_US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_EQUAL_HEX8(expected, actual, line, message)                            UnityAssertEqualNumber((_US32)(expected), (_US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX8)
#define UNITY_TEST_ASSERT_EQUAL_HEX16(expected, actual, line, message)                           UnityAssertEqualNumber((_US32)(expected), (_US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX16)
#define UNITY_TEST_ASSERT_EQUAL_HEX32(expected, actual, line, message)                           UnityAssertEqualNumber((_US32)(expected), (_US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX32)
#define UNITY_TEST_ASSERT_BITS(mask, expected, actual, line, message)                            UnityAssertBits((_US32)(mask), (_US32)(expected), (_US32)(actual), (message), (UNITY_LINE_TYPE)line)

#define UNITY_TEST_ASSERT_INT_WITHIN(delta, expected, actual, line, message)                     UnityAssertNumbersWithin((_US32)(delta), (_US32)(expected), (_US32)(actual), NULL, (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_UINT_WITHIN(delta, expected, actual, line, message)                    UnityAssertNumbersWithin((_US32)(delta), (_US32)(expected), (_US32)(actual), NULL, (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_HEX8_WITHIN(delta, expected, actual, line, message)                    UnityAssertNumbersWithin((_US32)(delta), (_US32)(expected), (_US32)(actual), NULL, (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX8)
#define UNITY_TEST_ASSERT_HEX16_WITHIN(delta, expected, actual, line, message)                   UnityAssertNumbersWithin((_US32)(delta), (_US32)(expected), (_US32)(actual), NULL, (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX16)
#define UNITY_TEST_ASSERT_HEX32_WITHIN(delta, expected, actual, line, message)                   UnityAssertNumbersWithin((_US32)(delta), (_US32)(expected), (_US32)(actual), NULL, (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX32)

#define UNITY_TEST_ASSERT_EQUAL_STRING(expected, actual, line, message)                          UnityAssertEqualString((const char*)(expected), (const char*)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_EQUAL_MEMORY(expected, actual, len, line, message)                     UnityAssertEqualMemory((void*)(expected), (void*)(actual), (_UU32)(len), 1, (message), (UNITY_LINE_TYPE)line)

#define UNITY_TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, num_elements, line, message)         UnityAssertEqualIntArray((const _US32*)(expected), (const _US32*)(actual), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, num_elements, line, message)        UnityAssertEqualIntArray((const _US32*)(expected), (const _US32*)(actual), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, num_elements, line, message)        UnityAssertEqualIntArray((const _US32*)(expected), (const _US32*)(actual), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX8)
#define UNITY_TEST_ASSERT_EQUAL_HEX16_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const _US32*)(expected), (const _US32*)(actual), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX16)
#define UNITY_TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const _US32*)(expected), (const _US32*)(actual), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX32)
#define UNITY_TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, num_elements, line, message)      UnityAssertEqualStringArray((const char**)(expected), (const char**)(actual), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, actual, len, num_elements, line, message) UnityAssertEqualMemory((void*)(expected), (void*)(actual), (_UU32)(len), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line)

#ifdef UNITY_EXCLUDE_FLOAT
#define UNITY_TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual, line, message)                   UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#define UNITY_TEST_ASSERT_EQUAL_FLOAT(expected, actual, line, message)                           UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#define UNITY_TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements, line, message)       UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#else
#define UNITY_TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual, line, message)                   UnityAssertFloatsWithin((_UF)(delta), (_UF)(expected), (_UF)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_EQUAL_FLOAT(expected, actual, line, message)                           UNITY_TEST_ASSERT_FLOAT_WITHIN((_UF)(expected) * (_UF)UNITY_FLOAT_PRECISION, (_UF)expected, (_UF)actual, (UNITY_LINE_TYPE)line, message)
#define UNITY_TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualFloatArray((_UF*)(expected), (_UF*)(actual), (_UU32)(num_elements), (message), (UNITY_LINE_TYPE)line)
#endif

#endif
