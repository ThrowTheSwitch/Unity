/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

#include <stdio.h>
#include <setjmp.h>

// Unity attempts to determine sizeof(various types)
// based on UINT_MAX, ULONG_MAX, etc. These are typically
// defined in limits.h.
#ifdef UNITY_USE_LIMITS_H
#include <limits.h>
#endif
// As a fallback, hope that including stdint.h will
// provide this information.
#ifndef UNITY_EXCLUDE_STDINT_H
#include <stdint.h>
#endif

//-------------------------------------------------------
// Guess Widths If Not Specified
//-------------------------------------------------------

// Determine the size of an int, if not already specificied.
// We cannot use sizeof(int), because it is not yet defined
// at this stage in the trnslation of the C program.
// Therefore, infer it from UINT_MAX if possible.
#ifndef UNITY_INT_WIDTH
  #ifdef UINT_MAX
    #if (UINT_MAX == 0xFFFF)
      #define UNITY_INT_WIDTH (16)
    #elif (UINT_MAX == 0xFFFFFFFF)
      #define UNITY_INT_WIDTH (32)
    #elif (UINT_MAX == 0xFFFFFFFFFFFFFFFF)
      #define UNITY_INT_WIDTH (64)
      #ifndef UNITY_SUPPORT_64
      #define UNITY_SUPPORT_64
      #endif
    #endif
  #endif
#endif
#ifndef UNITY_INT_WIDTH
  #define UNITY_INT_WIDTH (32)
#endif

// Determine the size of a long, if not already specified,
// by following the process used above to define
// UNITY_INT_WIDTH.
#ifndef UNITY_LONG_WIDTH
  #ifdef ULONG_MAX
    #if (ULONG_MAX == 0xFFFF)
      #define UNITY_LONG_WIDTH (16)
    #elif (ULONG_MAX == 0xFFFFFFFF)
      #define UNITY_LONG_WIDTH (32)
    #elif (ULONG_MAX == 0xFFFFFFFFFFFFFFFF)
      #define UNITY_LONG_WIDTH (64)
      #ifndef UNITY_SUPPORT_64
      #define UNITY_SUPPORT_64
      #endif
    #endif
  #endif
#endif
#ifndef UNITY_LONG_WIDTH
  #define UNITY_LONG_WIDTH (32)
#endif

// Determine the size of a pointer, if not already specified,
// by following the process used above to define
// UNITY_INT_WIDTH.
#ifndef UNITY_POINTER_WIDTH
  #ifdef UINTPTR_MAX
    #if (UINTPTR_MAX <= 0xFFFF)
      #define UNITY_POINTER_WIDTH (16)
    #elif (UINTPTR_MAX <= 0xFFFFFFFF)
      #define UNITY_POINTER_WIDTH (32)
    #elif (UINTPTR_MAX <= 0xFFFFFFFFFFFFFFFF)
      #define UNITY_POINTER_WIDTH (64)
      #ifndef UNITY_SUPPORT_64
      #define UNITY_SUPPORT_64
      #endif
    #endif
  #endif
#endif
#ifndef UNITY_POINTER_WIDTH
  #ifdef INTPTR_MAX
    #if (INTPTR_MAX <= 0x7FFF)
      #define UNITY_POINTER_WIDTH (16)
    #elif (INTPTR_MAX <= 0x7FFFFFFF)
      #define UNITY_POINTER_WIDTH (32)
    #elif (INTPTR_MAX <= 0x7FFFFFFFFFFFFFFF)
      #define UNITY_POINTER_WIDTH (64)
      #ifndef UNITY_SUPPORT_64
      #define UNITY_SUPPORT_64
      #endif
    #endif
  #endif
#endif
#ifndef UNITY_POINTER_WIDTH
  #define UNITY_POINTER_WIDTH (32)
#endif

//-------------------------------------------------------
// Int Support
//-------------------------------------------------------

#if (UNITY_INT_WIDTH == 32)
    typedef unsigned char   UU8;
    typedef unsigned short  UU16;
    typedef unsigned int    UU32;
    typedef signed char     US8;
    typedef signed short    US16;
    typedef signed int      US32;
#elif (UNITY_INT_WIDTH == 16)
    typedef unsigned char   UU8;
    typedef unsigned int    UU16;
    typedef unsigned long   UU32;
    typedef signed char     US8;
    typedef signed int      US16;
    typedef signed long     US32;
#else
    #error Invalid UNITY_INT_WIDTH specified! (16 or 32 are supported)
#endif

//-------------------------------------------------------
// 64-bit Support
//-------------------------------------------------------

#ifndef UNITY_SUPPORT_64

//No 64-bit Support
typedef UU32 U_UINT;
typedef US32 U_SINT;

#else

//64-bit Support
#if (UNITY_LONG_WIDTH == 32)
    typedef unsigned long long UU64;
    typedef signed long long   US64;
#elif (UNITY_LONG_WIDTH == 64)
    typedef unsigned long      UU64;
    typedef signed long        US64;
#else
    #error Invalid UNITY_LONG_WIDTH specified! (32 or 64 are supported)
#endif
typedef UU64 U_UINT;
typedef US64 U_SINT;

#endif

//-------------------------------------------------------
// Pointer Support
//-------------------------------------------------------

#if (UNITY_POINTER_WIDTH == 32)
    typedef UU32 UP;
#define UNITY_DISPLAY_STYLE_POINTER UNITY_DISPLAY_STYLE_HEX32
#elif (UNITY_POINTER_WIDTH == 64)
#ifndef UNITY_SUPPORT_64
#error "You've Specified 64-bit pointers without enabling 64-bit Support. Define UNITY_SUPPORT_64"
#endif
    typedef UU64 UP;
#define UNITY_DISPLAY_STYLE_POINTER UNITY_DISPLAY_STYLE_HEX64
#elif (UNITY_POINTER_WIDTH == 16)
    typedef UU16 UP;
#define UNITY_DISPLAY_STYLE_POINTER UNITY_DISPLAY_STYLE_HEX16
#else
    #error Invalid UNITY_POINTER_WIDTH specified! (16, 32 or 64 are supported)
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
typedef UNITY_FLOAT_TYPE UF;
    
#endif

//-------------------------------------------------------
// Double Float Support
//-------------------------------------------------------

//unlike FLOAT, we DON'T include by default
#ifndef UNITY_EXCLUDE_DOUBLE
#ifndef UNITY_INCLUDE_DOUBLE
#define UNITY_EXCLUDE_DOUBLE
#endif
#endif

#ifdef UNITY_EXCLUDE_DOUBLE 

//No Floating Point Support
#undef UNITY_DOUBLE_PRECISION
#undef UNITY_DOUBLE_TYPE
#undef UNITY_DOUBLE_VERBOSE

#else

//Floating Point Support
#ifndef UNITY_DOUBLE_PRECISION
#define UNITY_DOUBLE_PRECISION (1e-12f) 
#endif
#ifndef UNITY_DOUBLE_TYPE
#define UNITY_DOUBLE_TYPE double
#endif
typedef UNITY_DOUBLE_TYPE UD;
    
#endif

//-------------------------------------------------------
// Output Method
//-------------------------------------------------------

#ifndef UNITY_OUTPUT_CHAR
//Default to using putchar, which is defined in stdio.h above
#define UNITY_OUTPUT_CHAR(a) putchar(a)
#else
//If defined as something else, make sure we declare it here so it's ready for use
extern int UNITY_OUTPUT_CHAR(int);
#endif

//-------------------------------------------------------
// Footprint
//-------------------------------------------------------

#ifndef UNITY_LINE_TYPE
#define UNITY_LINE_TYPE U_UINT
#endif

#ifndef UNITY_COUNTER_TYPE
#define UNITY_COUNTER_TYPE U_UINT
#endif

//-------------------------------------------------------
// Internal Structs Needed
//-------------------------------------------------------

typedef void (*UnityTestFunction)(void);

#define UNITY_DISPLAY_RANGE_INT  (0x10)
#define UNITY_DISPLAY_RANGE_UINT (0x20)
#define UNITY_DISPLAY_RANGE_HEX  (0x40)
#define UNITY_DISPLAY_RANGE_AUTO (0x80)

typedef enum
{
#if (UNITY_INT_WIDTH == 16)
    UNITY_DISPLAY_STYLE_INT      = 2 + UNITY_DISPLAY_RANGE_INT + UNITY_DISPLAY_RANGE_AUTO,
#elif (UNITY_INT_WIDTH  == 32)
    UNITY_DISPLAY_STYLE_INT      = 4 + UNITY_DISPLAY_RANGE_INT + UNITY_DISPLAY_RANGE_AUTO,
#elif (UNITY_INT_WIDTH  == 64)
    UNITY_DISPLAY_STYLE_INT      = 8 + UNITY_DISPLAY_RANGE_INT + UNITY_DISPLAY_RANGE_AUTO,
#endif
    UNITY_DISPLAY_STYLE_INT8     = 1 + UNITY_DISPLAY_RANGE_INT,
    UNITY_DISPLAY_STYLE_INT16    = 2 + UNITY_DISPLAY_RANGE_INT,
    UNITY_DISPLAY_STYLE_INT32    = 4 + UNITY_DISPLAY_RANGE_INT,
#ifdef UNITY_SUPPORT_64
    UNITY_DISPLAY_STYLE_INT64    = 8 + UNITY_DISPLAY_RANGE_INT,
#endif

#if (UNITY_INT_WIDTH == 16)
    UNITY_DISPLAY_STYLE_UINT     = 2 + UNITY_DISPLAY_RANGE_UINT + UNITY_DISPLAY_RANGE_AUTO,
#elif (UNITY_INT_WIDTH  == 32)
    UNITY_DISPLAY_STYLE_UINT     = 4 + UNITY_DISPLAY_RANGE_UINT + UNITY_DISPLAY_RANGE_AUTO,
#elif (UNITY_INT_WIDTH  == 64)
    UNITY_DISPLAY_STYLE_UINT     = 8 + UNITY_DISPLAY_RANGE_UINT + UNITY_DISPLAY_RANGE_AUTO,
#endif
    UNITY_DISPLAY_STYLE_UINT8    = 1 + UNITY_DISPLAY_RANGE_UINT,
    UNITY_DISPLAY_STYLE_UINT16   = 2 + UNITY_DISPLAY_RANGE_UINT,
    UNITY_DISPLAY_STYLE_UINT32   = 4 + UNITY_DISPLAY_RANGE_UINT,
#ifdef UNITY_SUPPORT_64
    UNITY_DISPLAY_STYLE_UINT64   = 8 + UNITY_DISPLAY_RANGE_UINT,
#endif
    UNITY_DISPLAY_STYLE_HEX8     = 1 + UNITY_DISPLAY_RANGE_HEX,
    UNITY_DISPLAY_STYLE_HEX16    = 2 + UNITY_DISPLAY_RANGE_HEX,
    UNITY_DISPLAY_STYLE_HEX32    = 4 + UNITY_DISPLAY_RANGE_HEX,
#ifdef UNITY_SUPPORT_64
    UNITY_DISPLAY_STYLE_HEX64    = 8 + UNITY_DISPLAY_RANGE_HEX,
#endif
    UNITY_DISPLAY_STYLE_UNKNOWN
} UNITY_DISPLAY_STYLE_T;

struct Unity
{
    const char* TestFile;
    const char* CurrentTestName;
    UNITY_LINE_TYPE CurrentTestLineNumber;
    UNITY_COUNTER_TYPE NumberOfTests;
    UNITY_COUNTER_TYPE TestFailures;
    UNITY_COUNTER_TYPE TestIgnores;
    UNITY_COUNTER_TYPE CurrentTestFailed;
    UNITY_COUNTER_TYPE CurrentTestIgnored;
    jmp_buf AbortFrame;
};

extern struct Unity Unity;

//-------------------------------------------------------
// Test Suite Management
//-------------------------------------------------------

void UnityBegin(void);
int  UnityEnd(void);
void UnityConcludeTest(void);
void UnityDefaultTestRun(UnityTestFunction Func, const char* FuncName, const int FuncLineNum);

//-------------------------------------------------------
// Test Output
//-------------------------------------------------------

void UnityPrint(const char* string);
void UnityPrintMask(const U_UINT mask, const U_UINT number);
void UnityPrintNumberByStyle(const U_SINT number, const UNITY_DISPLAY_STYLE_T style);
void UnityPrintNumber(const U_SINT number);
void UnityPrintNumberUnsigned(const U_UINT number);
void UnityPrintNumberHex(const U_UINT number, const char nibbles);

#ifdef UNITY_FLOAT_VERBOSE
void UnityPrintFloat(const UF number);
#endif

//-------------------------------------------------------
// Test Assertion Fuctions
//-------------------------------------------------------
//  Use the macros below this section instead of calling
//  these directly. The macros have a consistent naming
//  convention and will pull in file and line information
//  for you.

void UnityAssertEqualNumber(const U_SINT expected,
                            const U_SINT actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber,
                            const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualIntArray(const U_SINT* expected,
                              const U_SINT* actual,
                              const UU32 num_elements,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber,
                              const UNITY_DISPLAY_STYLE_T style);

void UnityAssertBits(const U_SINT mask,
                     const U_SINT expected,
                     const U_SINT actual,
                     const char* msg,
                     const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualStringArray( const char** expected,
                                  const char** actual,
                                  const UU32 num_elements,
                                  const char* msg,
                                  const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualMemory( const void* expected,
                             const void* actual,
                             const UU32 length,
                             const UU32 num_elements,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber);

void UnityAssertNumbersWithin(const U_SINT delta,
                              const U_SINT expected,
                              const U_SINT actual,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber,
                              const UNITY_DISPLAY_STYLE_T style);

void UnityFail(const char* message, const UNITY_LINE_TYPE line);

void UnityIgnore(const char* message, const UNITY_LINE_TYPE line);

#ifndef UNITY_EXCLUDE_FLOAT
void UnityAssertFloatsWithin(const UF delta,
                             const UF expected,
                             const UF actual,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualFloatArray(const UF* expected,
                                const UF* actual,
                                const UU32 num_elements,
                                const char* msg,
                                const UNITY_LINE_TYPE lineNumber);

void UnityAssertFloatIsInf(const UF actual,
                           const char* msg,
                           const UNITY_LINE_TYPE lineNumber);

void UnityAssertFloatIsNegInf(const UF actual,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber);

void UnityAssertFloatIsNaN(const UF actual,
                           const char* msg,
                           const UNITY_LINE_TYPE lineNumber);
#endif

#ifndef UNITY_EXCLUDE_DOUBLE
void UnityAssertDoublesWithin(const UD delta,
                              const UD expected,
                              const UD actual,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualDoubleArray(const UD* expected,
                                 const UD* actual,
                                 const UU32 num_elements,
                                 const char* msg,
                                 const UNITY_LINE_TYPE lineNumber);

void UnityAssertDoubleIsInf(const UD actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber);

void UnityAssertDoubleIsNegInf(const UD actual,
                               const char* msg,
                               const UNITY_LINE_TYPE lineNumber);

void UnityAssertDoubleIsNaN(const UD actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber);
#endif

//-------------------------------------------------------
// Basic Fail and Ignore
//-------------------------------------------------------

#define UNITY_TEST_FAIL(line, message)   UnityFail(   (message), (UNITY_LINE_TYPE)line);
#define UNITY_TEST_IGNORE(line, message) UnityIgnore( (message), (UNITY_LINE_TYPE)line);

//-------------------------------------------------------
// Test Asserts
//-------------------------------------------------------

#define UNITY_TEST_ASSERT(condition, line, message)                                              if (condition) {} else {UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, message);}
#define UNITY_TEST_ASSERT_NULL(pointer, line, message)                                           UNITY_TEST_ASSERT(((pointer) == NULL),  (UNITY_LINE_TYPE)line, message)
#define UNITY_TEST_ASSERT_NOT_NULL(pointer, line, message)                                       UNITY_TEST_ASSERT(((pointer) != NULL),  (UNITY_LINE_TYPE)line, message)

#define UNITY_TEST_ASSERT_EQUAL_INT(expected, actual, line, message)                             UnityAssertEqualNumber((U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_EQUAL_INT8(expected, actual, line, message)                            UnityAssertEqualNumber((U_SINT)(US8 )(expected), (U_SINT)(US8 )(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_EQUAL_INT16(expected, actual, line, message)                           UnityAssertEqualNumber((U_SINT)(US16)(expected), (U_SINT)(US16)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_EQUAL_INT32(expected, actual, line, message)                           UnityAssertEqualNumber((U_SINT)(US32)(expected), (U_SINT)(US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_EQUAL_UINT(expected, actual, line, message)                            UnityAssertEqualNumber((U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_EQUAL_UINT8(expected, actual, line, message)                           UnityAssertEqualNumber((U_SINT)(US8 )(expected), (U_SINT)(US8 )(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_EQUAL_UINT16(expected, actual, line, message)                          UnityAssertEqualNumber((U_SINT)(US16)(expected), (U_SINT)(US16)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_EQUAL_UINT32(expected, actual, line, message)                          UnityAssertEqualNumber((U_SINT)(US32)(expected), (U_SINT)(US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_EQUAL_HEX8(expected, actual, line, message)                            UnityAssertEqualNumber((U_SINT)(US8 )(expected), (U_SINT)(US8 )(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX8)
#define UNITY_TEST_ASSERT_EQUAL_HEX16(expected, actual, line, message)                           UnityAssertEqualNumber((U_SINT)(US16)(expected), (U_SINT)(US16)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX16)
#define UNITY_TEST_ASSERT_EQUAL_HEX32(expected, actual, line, message)                           UnityAssertEqualNumber((U_SINT)(US32)(expected), (U_SINT)(US32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX32)
#define UNITY_TEST_ASSERT_BITS(mask, expected, actual, line, message)                            UnityAssertBits((U_SINT)(mask), (U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line)

#define UNITY_TEST_ASSERT_INT_WITHIN(delta, expected, actual, line, message)                     UnityAssertNumbersWithin((U_SINT)(delta), (U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_UINT_WITHIN(delta, expected, actual, line, message)                    UnityAssertNumbersWithin((U_SINT)(delta), (U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_HEX8_WITHIN(delta, expected, actual, line, message)                    UnityAssertNumbersWithin((U_SINT)(U_UINT)(UU8 )(delta), (U_SINT)(U_UINT)(UU8 )(expected), (U_SINT)(U_UINT)(UU8 )(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX8)
#define UNITY_TEST_ASSERT_HEX16_WITHIN(delta, expected, actual, line, message)                   UnityAssertNumbersWithin((U_SINT)(U_UINT)(UU16)(delta), (U_SINT)(U_UINT)(UU16)(expected), (U_SINT)(U_UINT)(UU16)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX16)
#define UNITY_TEST_ASSERT_HEX32_WITHIN(delta, expected, actual, line, message)                   UnityAssertNumbersWithin((U_SINT)(U_UINT)(UU32)(delta), (U_SINT)(U_UINT)(UU32)(expected), (U_SINT)(U_UINT)(UU32)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX32)

#define UNITY_TEST_ASSERT_EQUAL_PTR(expected, actual, line, message)                             UnityAssertEqualNumber((U_SINT)(UP)(expected), (U_SINT)(UP)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_POINTER)
#define UNITY_TEST_ASSERT_EQUAL_STRING(expected, actual, line, message)                          UnityAssertEqualString((const char*)(expected), (const char*)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_EQUAL_MEMORY(expected, actual, len, line, message)                     UnityAssertEqualMemory((void*)(expected), (void*)(actual), (UU32)(len), 1, (message), (UNITY_LINE_TYPE)line)

#define UNITY_TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, num_elements, line, message)         UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT)
#define UNITY_TEST_ASSERT_EQUAL_INT8_ARRAY(expected, actual, num_elements, line, message)        UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT8)
#define UNITY_TEST_ASSERT_EQUAL_INT16_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT16)
#define UNITY_TEST_ASSERT_EQUAL_INT32_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT32)
#define UNITY_TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, num_elements, line, message)        UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT)
#define UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT8)
#define UNITY_TEST_ASSERT_EQUAL_UINT16_ARRAY(expected, actual, num_elements, line, message)      UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT16)
#define UNITY_TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, num_elements, line, message)      UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT32)
#define UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, num_elements, line, message)        UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX8)
#define UNITY_TEST_ASSERT_EQUAL_HEX16_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX16)
#define UNITY_TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX32)
#define UNITY_TEST_ASSERT_EQUAL_PTR_ARRAY(expected, actual, num_elements, line, message)         UnityAssertEqualIntArray((const U_SINT*)(UP*)(expected), (const U_SINT*)(UP*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_POINTER)
#define UNITY_TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, num_elements, line, message)      UnityAssertEqualStringArray((const char**)(expected), (const char**)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, actual, len, num_elements, line, message) UnityAssertEqualMemory((void*)(expected), (void*)(actual), (UU32)(len), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line)

#ifdef UNITY_SUPPORT_64
#define UNITY_TEST_ASSERT_EQUAL_INT64(expected, actual, line, message)                           UnityAssertEqualNumber((U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT64)
#define UNITY_TEST_ASSERT_EQUAL_UINT64(expected, actual, line, message)                          UnityAssertEqualNumber((U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT64)
#define UNITY_TEST_ASSERT_EQUAL_HEX64(expected, actual, line, message)                           UnityAssertEqualNumber((U_SINT)(expected), (U_SINT)(actual), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX64)
#define UNITY_TEST_ASSERT_EQUAL_INT64_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_INT64)
#define UNITY_TEST_ASSERT_EQUAL_UINT64_ARRAY(expected, actual, num_elements, line, message)      UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_UINT64)
#define UNITY_TEST_ASSERT_EQUAL_HEX64_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualIntArray((const U_SINT*)(expected), (const U_SINT*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX64)
#define UNITY_TEST_ASSERT_HEX64_WITHIN(delta, expected, actual, line, message)                   UnityAssertNumbersWithin((U_SINT)(delta), (U_SINT)(expected), (U_SINT)(actual), NULL, (UNITY_LINE_TYPE)line, UNITY_DISPLAY_STYLE_HEX64)
#endif

#ifdef UNITY_EXCLUDE_FLOAT
#define UNITY_TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual, line, message)                   UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#define UNITY_TEST_ASSERT_EQUAL_FLOAT(expected, actual, line, message)                           UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#define UNITY_TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements, line, message)       UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#define UNITY_TEST_ASSERT_FLOAT_IS_INF(actual, line, message)                                    UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#define UNITY_TEST_ASSERT_FLOAT_IS_NEG_INF(actual, line, message)                                UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#define UNITY_TEST_ASSERT_FLOAT_IS_NAN(actual, line, message)                                    UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Floating Point Disabled")
#else
#define UNITY_TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual, line, message)                   UnityAssertFloatsWithin((UF)(delta), (UF)(expected), (UF)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_EQUAL_FLOAT(expected, actual, line, message)                           UNITY_TEST_ASSERT_FLOAT_WITHIN((UF)(expected) * (UF)UNITY_FLOAT_PRECISION, (UF)expected, (UF)actual, (UNITY_LINE_TYPE)line, message)
#define UNITY_TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements, line, message)       UnityAssertEqualFloatArray((UF*)(expected), (UF*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_FLOAT_IS_INF(actual, line, message)                                    UnityAssertFloatIsInf((UF)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_FLOAT_IS_NEG_INF(actual, line, message)                                UnityAssertFloatIsNegInf((UF)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_FLOAT_IS_NAN(actual, line, message)                                    UnityAssertFloatIsNaN((UF)(actual), (message), (UNITY_LINE_TYPE)line)
#endif

#ifdef UNITY_EXCLUDE_DOUBLE
#define UNITY_TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual, line, message)                  UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Double Precision Disabled")
#define UNITY_TEST_ASSERT_EQUAL_DOUBLE(expected, actual, line, message)                          UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Double Precision Disabled")
#define UNITY_TEST_ASSERT_EQUAL_DOUBLE_ARRAY(expected, actual, num_elements, line, message)      UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Double Precision Disabled")
#define UNITY_TEST_ASSERT_DOUBLE_IS_INF(actual, line, message)                                   UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Double Precision Disabled")
#define UNITY_TEST_ASSERT_DOUBLE_IS_NEG_INF(actual, line, message)                               UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Double Precision Disabled")
#define UNITY_TEST_ASSERT_DOUBLE_IS_NAN(actual, line, message)                                   UNITY_TEST_FAIL((UNITY_LINE_TYPE)line, "Unity Double Precision Disabled")
#else
#define UNITY_TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual, line, message)                  UnityAssertDoublesWithin((UD)(delta), (UD)(expected), (UD)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_EQUAL_DOUBLE(expected, actual, line, message)                          UNITY_TEST_ASSERT_DOUBLE_WITHIN((UF)(expected) * (UD)UNITY_DOUBLE_PRECISION, (UD)expected, (UD)actual, (UNITY_LINE_TYPE)line, message)
#define UNITY_TEST_ASSERT_EQUAL_DOUBLE_ARRAY(expected, actual, num_elements, line, message)      UnityAssertEqualDoubleArray((UD*)(expected), (UD*)(actual), (UU32)(num_elements), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_DOUBLE_IS_INF(actual, line, message)                                   UnityAssertFloatIsInf((UF)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_DOUBLE_IS_NEG_INF(actual, line, message)                               UnityAssertFloatIsNegInf((UF)(actual), (message), (UNITY_LINE_TYPE)line)
#define UNITY_TEST_ASSERT_DOUBLE_IS_NAN(actual, line, message)                                   UnityAssertFloatIsNaN((UF)(actual), (message), (UNITY_LINE_TYPE)line)
#endif

#endif
