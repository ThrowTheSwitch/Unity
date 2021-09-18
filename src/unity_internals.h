/* ==========================================
	Unity Project - A Test Framework for C
	Copyright (c) 2007-21 Mike Karlesky, Mark VanderVoord, Greg Williams
	[Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

#ifdef UNITY_INCLUDE_CONFIG_H
	#include "unity_config.h"
#endif	/* UNITY_INCLUDE_CONFIG_H */

#ifndef UNITY_EXCLUDE_SETJMP_H
	#include <setjmp.h>
#endif	/* UNITY_EXCLUDE_SETJMP_H */

#ifndef UNITY_EXCLUDE_MATH_H
	#include <math.h>
#endif	/* UNITY_EXCLUDE_MATH_H */

#ifndef UNITY_EXCLUDE_STDDEF_H
	#include <stddef.h>
#endif	/* UNITY_EXCLUDE_STDDEF_H */

#ifdef UNITY_INCLUDE_PRINT_FORMATTED
	#include <stdarg.h>
#endif	/* UNITY_INCLUDE_PRINT_FORMATTED */

/* Unity Attempts to Auto-Detect Integer Types
 * Attempt 1: UINT_MAX, ULONG_MAX in <limits.h>, or default to 32 bits
 * Attempt 2: UINTPTR_MAX in <stdint.h>, or default to same size as long
 * The user may override any of these derived constants:
 * UNITY_INT_WIDTH, UNITY_LONG_WIDTH, UNITY_POINTER_WIDTH */
#ifndef UNITY_EXCLUDE_STDINT_H
	#include <stdint.h>
#endif	/* UNITY_EXCLUDE_STDINT_H */

#ifndef UNITY_EXCLUDE_LIMITS_H
	#include <limits.h>
#endif	/* UNITY_EXCLUDE_LIMITS_H */

#if defined(__GNUC__) || defined(__clang__)
	#define UNITY_FUNCTION_ATTR(a)	__attribute__((a))
#else	/* defined(__GNUC__) || defined(__clang__) */
	#define UNITY_FUNCTION_ATTR(a)	/* ignore */
#endif	/* defined(__GNUC__) || defined(__clang__) */

#ifndef UNITY_NORETURN
	#if defined(__cplusplus)
		#if __cplusplus >= 201103L
			#define UNITY_NORETURN [[ noreturn ]]
		#endif	/* __cplusplus >= 201103L */
	#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
		#include <stdnoreturn.h>
		#define UNITY_NORETURN noreturn
	#endif	/* defined(__cplusplus) */
#endif	/* UNITY_NORETURN */
#ifndef UNITY_NORETURN
	#define UNITY_NORETURN UNITY_FUNCTION_ATTR(noreturn)
#endif	/* UNITY_NORETURN */

/*-------------------------------------------------------
 * Guess Widths If Not Specified
 *-------------------------------------------------------*/

/* Determine the size of an int, if not already specified.
 * We cannot use sizeof(int), because it is not yet defined
 * at this stage in the translation of the C program.
 * Also sizeof(int) does return the size in addressable units on all platforms,
 * which may not necessarily be the size in bytes.
 * Therefore, infer it from UINT_MAX if possible. */
#ifndef UNITY_INT_WIDTH
	#ifdef UINT_MAX
		#if (UINT_MAX == 0xFFFF)
			#define UNITY_INT_WIDTH	(16)
		#elif (UINT_MAX == 0xFFFFFFFF)
			#define UNITY_INT_WIDTH	(32)
		#elif (UINT_MAX == 0xFFFFFFFFFFFFFFFF)
			#define UNITY_INT_WIDTH	(64)
		#endif	/* (UINT_MAX == 0xFFFF) */
	#else /* Set to default */
		#define UNITY_INT_WIDTH		(32)
	#endif	/* UINT_MAX */
#endif	/* UNITY_INT_WIDTH */

/* Determine the size of a long, if not already specified. */
#ifndef UNITY_LONG_WIDTH
	#ifdef ULONG_MAX
		#if (ULONG_MAX == 0xFFFF)
			#define UNITY_LONG_WIDTH	(16)
		#elif (ULONG_MAX == 0xFFFFFFFF)
			#define UNITY_LONG_WIDTH	(32)
		#elif (ULONG_MAX == 0xFFFFFFFFFFFFFFFF)
			#define UNITY_LONG_WIDTH	(64)
		#endif	/* (ULONG_MAX == 0xFFFF) */
	#else /* Set to default */
		#define UNITY_LONG_WIDTH		(32)
	#endif	/* ULONG_MAX */
#endif	/* UNITY_LONG_WIDTH */

/* Determine the size of a pointer, if not already specified. */
#ifndef UNITY_POINTER_WIDTH
	#ifdef UINTPTR_MAX
		#if (UINTPTR_MAX <= 0xFFFF)
			#define UNITY_POINTER_WIDTH	(16)
		#elif (UINTPTR_MAX <= 0xFFFFFFFF)
			#define UNITY_POINTER_WIDTH	(32)
		#elif (UINTPTR_MAX <= 0xFFFFFFFFFFFFFFFF)
			#define UNITY_POINTER_WIDTH	(64)
		#endif	/* (UINTPTR_MAX <= 0xFFFF) */
	#else /* Set to default */
		#define UNITY_POINTER_WIDTH		UNITY_LONG_WIDTH
	#endif	/* UINTPTR_MAX */
#endif	/* UNITY_POINTER_WIDTH */

/*-------------------------------------------------------
 * Int Support (Define types based on detected sizes)
 *-------------------------------------------------------*/

#if (UNITY_INT_WIDTH == 32)
	typedef unsigned char	UNITY_UINT8;
	typedef unsigned short	UNITY_UINT16;
	typedef unsigned int	UNITY_UINT32;
	typedef signed char		UNITY_INT8;
	typedef signed short	UNITY_INT16;
	typedef signed int		UNITY_INT32;
#elif (UNITY_INT_WIDTH == 16)
	typedef unsigned char	UNITY_UINT8;
	typedef unsigned int	UNITY_UINT16;
	typedef unsigned long	UNITY_UINT32;
	typedef signed char		UNITY_INT8;
	typedef signed int		UNITY_INT16;
	typedef signed long		UNITY_INT32;
#else	/* (UNITY_INT_WIDTH == 32) */
	#error Invalid UNITY_INT_WIDTH specified! (16 or 32 are supported)
#endif	/* (UNITY_INT_WIDTH == 32) */

/*-------------------------------------------------------
 * 64-bit Support
 *-------------------------------------------------------*/

/* Auto-detect 64 Bit Support */
#ifndef UNITY_SUPPORT_64
	#if UNITY_LONG_WIDTH == 64 || UNITY_POINTER_WIDTH == 64
		#define UNITY_SUPPORT_64
	#endif	/* UNITY_LONG_WIDTH == 64 || UNITY_POINTER_WIDTH == 64 */
#endif	/* UNITY_SUPPORT_64 */

/* 64-Bit Support Dependent Configuration */
#ifndef UNITY_SUPPORT_64
		/* No 64-bit Support */
		typedef	UNITY_UINT32		UNITY_UINT;
		typedef	UNITY_INT32			UNITY_INT;
		/* Maximum number of nibbles in a UNITY_(U)INT */
		#define UNITY_MAX_NIBBLES	(8)
#else	/* UNITY_SUPPORT_64 */
	/* 64-bit Support */
	#if (UNITY_LONG_WIDTH == 32)
		typedef unsigned long long	UNITY_UINT64;
		typedef signed long long	UNITY_INT64;
	#elif (UNITY_LONG_WIDTH == 64)
		typedef unsigned long		UNITY_UINT64;
		typedef signed long			UNITY_INT64;
	#else	/* (UNITY_LONG_WIDTH == 32) */
		#error Invalid UNITY_LONG_WIDTH specified! (32 or 64 are supported)
	#endif	/* (UNITY_LONG_WIDTH == 32) */
		typedef	UNITY_UINT64		UNITY_UINT;
		typedef	UNITY_INT64			UNITY_INT;
		/* Maximum number of nibbles in a UNITY_(U)INT */
		#define UNITY_MAX_NIBBLES	(16)
#endif	/* UNITY_SUPPORT_64 */

/*-------------------------------------------------------
 * Pointer Support
 *-------------------------------------------------------*/

#if (UNITY_POINTER_WIDTH == 32)
	#define UNITY_PTR_TO_INT				UNITY_INT32
	#define UNITY_DISPLAY_STYLE__POINTER	UNITY_DISPLAY_STYLE__HEX32
#elif (UNITY_POINTER_WIDTH == 64)
	#define UNITY_PTR_TO_INT				UNITY_INT64
	#define UNITY_DISPLAY_STYLE__POINTER	UNITY_DISPLAY_STYLE__HEX64
#elif (UNITY_POINTER_WIDTH == 16)
	#define UNITY_PTR_TO_INT				UNITY_INT16
	#define UNITY_DISPLAY_STYLE__POINTER	UNITY_DISPLAY_STYLE__HEX16
#else	/* (UNITY_POINTER_WIDTH == 32) */
	#error Invalid UNITY_POINTER_WIDTH specified! (16, 32 or 64 are supported)
#endif	/* (UNITY_POINTER_WIDTH == 32) */

#ifndef UNITY_PTR_ATTRIBUTE
	#define UNITY_PTR_ATTRIBUTE
#endif	/* UNITY_PTR_ATTRIBUTE */

#ifndef UNITY_INTERNAL_PTR
	#define UNITY_INTERNAL_PTR				UNITY_PTR_ATTRIBUTE const void*
#endif	/* UNITY_INTERNAL_PTR */

/*-------------------------------------------------------
 * Float Support
 *-------------------------------------------------------*/

#ifdef UNITY_EXCLUDE_FLOAT

/* No Floating Point Support */
#ifndef UNITY_EXCLUDE_DOUBLE
	/* Remove double when excluding float support */
	#define UNITY_EXCLUDE_DOUBLE
#endif	/* UNITY_EXCLUDE_DOUBLE */
#ifndef UNITY_EXCLUDE_FLOAT_PRINT
	#define UNITY_EXCLUDE_FLOAT_PRINT
#endif	/* UNITY_EXCLUDE_FLOAT_PRINT */

#else	/* UNITY_EXCLUDE_FLOAT */

/* Floating Point Support */
#ifndef UNITY_FLOAT_PRECISION
	#define UNITY_FLOAT_PRECISION	(0.00001f)
#endif	/* UNITY_FLOAT_PRECISION */
#ifndef UNITY_FLOAT_TYPE
	#define UNITY_FLOAT_TYPE		float
#endif	/* UNITY_FLOAT_TYPE */
typedef UNITY_FLOAT_TYPE			UNITY_FLOAT;

/* isinf & isnan macros should be provided by math.h */
#ifndef isinf
	/* The value of Inf - Inf is NaN */
	#define isinf(n)	(isnan((n) - (n)) && !isnan(n))
#endif	/* isinf */

#ifndef isnan
	/* NaN is the only floating point value that does NOT equal itself.
	 * Therefore if n != n, then it is NaN. */
	#define isnan(n)	((n != n) ? 1 : 0)
#endif	/* isnan */

#endif	/* UNITY_EXCLUDE_FLOAT */

/*-------------------------------------------------------
 * Double Float Support
 *-------------------------------------------------------*/

/* unlike float, we DON'T include by default */
#if defined(UNITY_EXCLUDE_DOUBLE) || !defined(UNITY_INCLUDE_DOUBLE)

	/* No Floating Point Support */
	#ifndef UNITY_EXCLUDE_DOUBLE
		#define UNITY_EXCLUDE_DOUBLE
	#else	/* UNITY_EXCLUDE_DOUBLE */
		#undef UNITY_INCLUDE_DOUBLE
	#endif	/* UNITY_EXCLUDE_DOUBLE */

	#ifndef UNITY_EXCLUDE_FLOAT
		#ifndef UNITY_DOUBLE_TYPE
			#define UNITY_DOUBLE_TYPE	double
		#endif	/* UNITY_DOUBLE_TYPE */
		typedef UNITY_FLOAT				UNITY_DOUBLE;
		/* For parameter in UnityPrintFloat(UNITY_DOUBLE), which aliases to double or float */
	#endif	/* UNITY_EXCLUDE_FLOAT */

#else	/* defined(UNITY_EXCLUDE_DOUBLE) || !defined(UNITY_INCLUDE_DOUBLE) */

	/* Double Floating Point Support */
	#ifndef UNITY_DOUBLE_PRECISION
		#define UNITY_DOUBLE_PRECISION	(1e-12)
	#endif	/* UNITY_DOUBLE_PRECISION */

	#ifndef UNITY_DOUBLE_TYPE
		#define UNITY_DOUBLE_TYPE		double
	#endif	/* UNITY_DOUBLE_TYPE */
	typedef UNITY_DOUBLE_TYPE			UNITY_DOUBLE;

#endif	/* defined(UNITY_EXCLUDE_DOUBLE) || !defined(UNITY_INCLUDE_DOUBLE) */

/*-------------------------------------------------------
 * Output Method: stdout (DEFAULT)
 *-------------------------------------------------------*/
#ifndef UNITY_OUTPUT_CHAR
	/* Default to using putchar, which is defined in stdio.h */
	#include <stdio.h>
	#define UNITY_OUTPUT_CHAR(a)	(void)putchar(a)
#else	/* UNITY_OUTPUT_CHAR */
	/* If defined as something else, make sure we declare it here so it's ready for use */
	#ifdef UNITY_OUTPUT_CHAR_HEADER_DECLARATION
		extern void UNITY_OUTPUT_CHAR_HEADER_DECLARATION;
	#endif	/* UNITY_OUTPUT_CHAR_HEADER_DECLARATION */
#endif	/* UNITY_OUTPUT_CHAR */

#ifndef UNITY_OUTPUT_FLUSH
	#ifdef UNITY_USE_FLUSH_STDOUT
		/* We want to use the stdout flush utility */
		#include <stdio.h>
		#define UNITY_OUTPUT_FLUSH()	(void)fflush(stdout)
	#else	/* UNITY_USE_FLUSH_STDOUT */
		/* We've specified nothing, therefore flush should just be ignored */
		#define UNITY_OUTPUT_FLUSH()
	#endif	/* UNITY_USE_FLUSH_STDOUT */
#else	/* UNITY_OUTPUT_FLUSH */
	/* If defined as something else, make sure we declare it here so it's ready for use */
	#ifdef UNITY_OUTPUT_FLUSH_HEADER_DECLARATION
		extern void UNITY_OUTPUT_FLUSH_HEADER_DECLARATION;
	#endif	/* UNITY_OUTPUT_FLUSH_HEADER_DECLARATION */
#endif	/* UNITY_OUTPUT_FLUSH */

#ifndef UNITY_OUTPUT_FLUSH
	#define UNITY_FLUSH_CALL()
#else	/* UNITY_OUTPUT_FLUSH */
	#define UNITY_FLUSH_CALL()	UNITY_OUTPUT_FLUSH()
#endif	/* UNITY_OUTPUT_FLUSH */

#ifndef UNITY_PRINT_EOL
	#define UNITY_PRINT_EOL()	UNITY_OUTPUT_CHAR('\n')
#endif	/* UNITY_PRINT_EOL */

#ifndef UNITY_OUTPUT_START
	#define UNITY_OUTPUT_START()
#endif	/* UNITY_OUTPUT_START */

#ifndef UNITY_OUTPUT_COMPLETE
	#define UNITY_OUTPUT_COMPLETE()
#endif	/* UNITY_OUTPUT_COMPLETE */

#ifdef UNITY_INCLUDE_EXEC_TIME
	#if !defined(UNITY_EXEC_TIME_START) &&	\
		!defined(UNITY_EXEC_TIME_STOP) &&	\
		!defined(UNITY_PRINT_EXEC_TIME) &&	\
		!defined(UNITY_TIME_TYPE)
		/* If none any of these macros are defined then try to provide a default implementation */

		#if defined(UNITY_CLOCK_MS)
			/* This is a simple way to get a default implementation on platforms that support getting a millisecond counter */
			#define UNITY_TIME_TYPE			UNITY_UINT
			#define UNITY_EXEC_TIME_START()	Unity.CurrentTestStartTime	= UNITY_CLOCK_MS()
			#define UNITY_EXEC_TIME_STOP()	Unity.CurrentTestStopTime	= UNITY_CLOCK_MS()
			#define UNITY_PRINT_EXEC_TIME()	{																\
						UNITY_UINT execTimeMs = (Unity.CurrentTestStopTime - Unity.CurrentTestStartTime);	\
						UnityPrint(" (");																	\
						UnityPrintNumberUnsigned(execTimeMs);												\
						UnityPrint(" ms)");	}
		#elif defined(_WIN32)
			#include <time.h>
			#define UNITY_TIME_TYPE			clock_t
			#define UNITY_GET_TIME(t)		t = (clock_t)((clock() * 1000) / CLOCKS_PER_SEC)
			#define UNITY_EXEC_TIME_START()	UNITY_GET_TIME(Unity.CurrentTestStartTime)
			#define UNITY_EXEC_TIME_STOP()	UNITY_GET_TIME(Unity.CurrentTestStopTime)
			#define UNITY_PRINT_EXEC_TIME() {																\
						UNITY_UINT execTimeMs = (Unity.CurrentTestStopTime - Unity.CurrentTestStartTime);	\
						UnityPrint(" (");																	\
						UnityPrintNumberUnsigned(execTimeMs);												\
						UnityPrint(" ms)"); }
		#elif defined(__unix__) || defined(__APPLE__)
			#include <time.h>
			#define UNITY_TIME_TYPE			struct timespec
			#define UNITY_GET_TIME(t)		clock_gettime(CLOCK_MONOTONIC, &t)
			#define UNITY_EXEC_TIME_START()	UNITY_GET_TIME(Unity.CurrentTestStartTime)
			#define UNITY_EXEC_TIME_STOP()	UNITY_GET_TIME(Unity.CurrentTestStopTime)
			#define UNITY_PRINT_EXEC_TIME() {																						\
						UNITY_UINT execTimeMs = ((Unity.CurrentTestStopTime.tv_sec - Unity.CurrentTestStartTime.tv_sec) * 1000L);	\
						execTimeMs += ((Unity.CurrentTestStopTime.tv_nsec - Unity.CurrentTestStartTime.tv_nsec) / 1000000L);		\
						UnityPrint(" (");																							\
						UnityPrintNumberUnsigned(execTimeMs);																		\
						UnityPrint(" ms)"); }
		#endif	/* defined(UNITY_CLOCK_MS) */
	#endif	/* (...) */
#endif	/* UNITY_INCLUDE_EXEC_TIME */

#ifndef UNITY_EXEC_TIME_START
	#define UNITY_EXEC_TIME_START()	do{}while(0)
#endif	/* UNITY_EXEC_TIME_START */

#ifndef UNITY_EXEC_TIME_STOP
	#define UNITY_EXEC_TIME_STOP()	do{}while(0)
#endif	/* UNITY_EXEC_TIME_STOP */

#ifndef UNITY_TIME_TYPE
	#define UNITY_TIME_TYPE			UNITY_UINT
#endif	/* UNITY_TIME_TYPE */

#ifndef UNITY_PRINT_EXEC_TIME
	#define UNITY_PRINT_EXEC_TIME()	do{}while(0)
#endif	/* UNITY_PRINT_EXEC_TIME */

/*-------------------------------------------------------
 * Footprint
 *-------------------------------------------------------*/

#ifndef UNITY_LINE_TYPE
	#define UNITY_LINE_TYPE			UNITY_UINT
#endif	/* UNITY_LINE_TYPE */

#ifndef UNITY_COUNTER_TYPE
	#define UNITY_COUNTER_TYPE		UNITY_UINT
#endif	/* UNITY_COUNTER_TYPE */

/*-------------------------------------------------------
 * Logging Features
 *-------------------------------------------------------*/

#if !defined(UNITY_LOG_ID)
	#define UNITY_LOG_ID
#endif

#if !defined(UNITY_LOG_CONTEXT)
	#define UNITY_LOG_CONTEXT
#endif

#if !defined(UNITY_LOG_BUFFER_SIZE)
	#define UNITY_LOG_BUFFER_SIZE	240
#endif

#if !defined(UNITY_LOG_DECLARE)
	#define UNITY_LOG_DECLARE()
#endif

#if !defined(UNITY_LOG_REGISTER)
	#define UNITY_LOG_REGISTER()
#endif

#if !defined(UNITY_LOG_PRINT)
	#define UNITY_LOG_PRINT()
#endif

/*-------------------------------------------------------
 * Internal Structs Needed
 *-------------------------------------------------------*/

typedef void (*UnityTestFunction)(void);

#define UNITY_DISPLAY_RANGE__INT	(0x10)
#define UNITY_DISPLAY_RANGE__UINT	(0x20)
#define UNITY_DISPLAY_RANGE__HEX	(0x40)
#define UNITY_DISPLAY_RANGE__CHAR	(0x80)

typedef enum
{
	UNITY_DISPLAY_STYLE__INT	= (UNITY_INT_WIDTH / 8) + UNITY_DISPLAY_RANGE__INT,
	UNITY_DISPLAY_STYLE__INT8	= 1 + UNITY_DISPLAY_RANGE__INT,
	UNITY_DISPLAY_STYLE__INT16	= 2 + UNITY_DISPLAY_RANGE__INT,
	UNITY_DISPLAY_STYLE__INT32	= 4 + UNITY_DISPLAY_RANGE__INT,
#ifdef UNITY_SUPPORT_64
	UNITY_DISPLAY_STYLE__INT64	= 8 + UNITY_DISPLAY_RANGE__INT,
#endif	/* UNITY_SUPPORT_64 */

	UNITY_DISPLAY_STYLE__UINT	= (UNITY_INT_WIDTH / 8) + UNITY_DISPLAY_RANGE__UINT,
	UNITY_DISPLAY_STYLE__UINT8	= 1 + UNITY_DISPLAY_RANGE__UINT,
	UNITY_DISPLAY_STYLE__UINT16	= 2 + UNITY_DISPLAY_RANGE__UINT,
	UNITY_DISPLAY_STYLE__UINT32	= 4 + UNITY_DISPLAY_RANGE__UINT,
#ifdef UNITY_SUPPORT_64
	UNITY_DISPLAY_STYLE__UINT64	= 8 + UNITY_DISPLAY_RANGE__UINT,
#endif	/* UNITY_SUPPORT_64 */

	UNITY_DISPLAY_STYLE__HEX8	= 1 + UNITY_DISPLAY_RANGE__HEX,
	UNITY_DISPLAY_STYLE__HEX16	= 2 + UNITY_DISPLAY_RANGE__HEX,
	UNITY_DISPLAY_STYLE__HEX32	= 4 + UNITY_DISPLAY_RANGE__HEX,
#ifdef UNITY_SUPPORT_64
	UNITY_DISPLAY_STYLE__HEX64	= 8 + UNITY_DISPLAY_RANGE__HEX,
#endif	/* UNITY_SUPPORT_64 */

	UNITY_DISPLAY_STYLE__CHAR	= 1 + UNITY_DISPLAY_RANGE__CHAR + UNITY_DISPLAY_RANGE__INT,

	UNITY_DISPLAY_STYLE__UNKNOWN,

	UNITY_DISPLAY_STYLE
} UNITY_DISPLAY_STYLE__T;

typedef enum
{
	UNITY_COMPARISON__WITHIN			= 0x0,
	UNITY_COMPARISON__EQUAL_TO			= 0x1,
	UNITY_COMPARISON__GREATER_THAN		= 0x2,
	UNITY_COMPARISON__GREATER_OR_EQUAL	= 0x2 + UNITY_COMPARISON__EQUAL_TO,
	UNITY_COMPARISON__SMALLER_THAN		= 0x4,
	UNITY_COMPARISON__SMALLER_OR_EQUAL	= 0x4 + UNITY_COMPARISON__EQUAL_TO,
	UNITY_COMPARISON__NOT_EQUAL			= 0x0,
	UNITY_COMPARISON__UNKNOWN,

	UNITY_COMPARISON
} UNITY_COMPARISON_T;

#ifndef UNITY_EXCLUDE_FLOAT
typedef enum
{
	UNITY_FLOAT_TRAIT__IS_NOT_INF	= 0,
	UNITY_FLOAT_TRAIT__IS_INF,
	UNITY_FLOAT_TRAIT__IS_NOT_NEG_INF,
	UNITY_FLOAT_TRAIT__IS_NEG_INF,
	UNITY_FLOAT_TRAIT__IS_NOT_NAN,
	UNITY_FLOAT_TRAIT__IS_NAN,
	UNITY_FLOAT_TRAIT__IS_NOT_DET,
	UNITY_FLOAT_TRAIT__IS_DET,
	UNITY_FLOAT_TRAIT__UNKNOWN,

	UNITY_FLOAT_TRAIT
} UNITY_FLOAT_TRAIT_T;
#endif	/* UNITY_EXCLUDE_FLOAT */

typedef enum
{
	UNITY_FLAGS__ARRAY_TO_VAL	= 0,
	UNITY_FLAGS__ARRAY_TO_ARRAY,
	UNITY_FLAGS__UNKNOWN,

	UNITY_FLAGS
} UNITY_FLAGS_T;

typedef struct
{
	const char*			TestFile;
	const char*			CurrentTestName;
#ifndef UNITY_EXCLUDE_DETAILS
	const char*			CurrentDetail1;
	const char*			CurrentDetail2;
#endif	/* UNITY_EXCLUDE_DETAILS */
	UNITY_LINE_TYPE		CurrentTestLineNumber;
	UNITY_COUNTER_TYPE	NumberOfTests;
	UNITY_COUNTER_TYPE	TestFailures;
	UNITY_COUNTER_TYPE	TestIgnores;
	UNITY_COUNTER_TYPE	CurrentTestFailed;
	UNITY_COUNTER_TYPE	CurrentTestIgnored;
#ifdef UNITY_INCLUDE_EXEC_TIME
	UNITY_TIME_TYPE		CurrentTestStartTime;
	UNITY_TIME_TYPE		CurrentTestStopTime;
#endif	/* UNITY_INCLUDE_EXEC_TIME */
#ifdef UNITY_INCLUDE_LOG
	char				LogBuffer[UNITY_LOG_BUFFER_SIZE];
	UNITY_COUNTER_TYPE	LogIndex;
#endif
#ifndef UNITY_EXCLUDE_SETJMP_H
	jmp_buf				AbortFrame;
#endif	/* UNITY_EXCLUDE_SETJMP_H */
} UNITY_STORAGE_T;

extern UNITY_STORAGE_T	Unity;

/*-------------------------------------------------------
 * Test Suite Management
 *-------------------------------------------------------*/

void UnityBegin(const char* filename);
int  UnityEnd(void);
void UnitySetTestFile(const char* filename);
void UnityConcludeTest(void);

#ifndef RUN_TEST
	void UnityDefaultTestRun(UnityTestFunction Func, const char* FuncName, const int FuncLineNum);
#else	/* RUN_TEST */
	#define UNITY_SKIP_DEFAULT_RUNNER
#endif	/* RUN_TEST */

/*-------------------------------------------------------
 * Details Support
 *-------------------------------------------------------*/

#ifdef UNITY_EXCLUDE_DETAILS
	#define UNITY_CLR_DETAILS()
	#define UNITY_SET_DETAIL(d1)
	#define UNITY_SET_DETAILS(d1,d2)
#else	/* UNITY_EXCLUDE_DETAILS */
	#define UNITY_CLR_DETAILS()			{ Unity.CurrentDetail1 = 0;		Unity.CurrentDetail2 = 0;		}
	#define UNITY_SET_DETAIL(d1)		{ Unity.CurrentDetail1 = (d1);	Unity.CurrentDetail2 = 0;		}
	#define UNITY_SET_DETAILS(d1,d2)	{ Unity.CurrentDetail1 = (d1);	Unity.CurrentDetail2 = (d2);	}

	#ifndef UNITY_DETAIL1_NAME
		#define UNITY_DETAIL1_NAME	"Function"
	#endif	/* UNITY_DETAIL1_NAME */

	#ifndef UNITY_DETAIL2_NAME
		#define UNITY_DETAIL2_NAME	"Argument"
	#endif	/* UNITY_DETAIL2_NAME */
#endif	/* UNITY_EXCLUDE_DETAILS */

#ifdef UNITY_PRINT_TEST_CONTEXT
	void UNITY_PRINT_TEST_CONTEXT(void);
#endif	/* UNITY_PRINT_TEST_CONTEXT */

/*-------------------------------------------------------
 * Test Output
 *-------------------------------------------------------*/

void UnityPrint(const char* string);

#ifdef UNITY_INCLUDE_PRINT_FORMATTED
	void UnityPrintF(const UNITY_LINE_TYPE line, const char* format, ...);
#endif	/* UNITY_INCLUDE_PRINT_FORMATTED */

void UnityPrintLen(const char* string, const UNITY_UINT32 length);
void UnityPrintMask(const UNITY_UINT mask, const UNITY_UINT number);
void UnityPrintNumberByStyle(const UNITY_INT number, const UNITY_DISPLAY_STYLE__T style);
void UnityPrintNumber(const UNITY_INT number_to_print);
void UnityPrintNumberUnsigned(const UNITY_UINT number);
void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print);

#ifndef UNITY_EXCLUDE_FLOAT_PRINT
	void UnityPrintFloat(const UNITY_DOUBLE input_number);
#endif	/* UNITY_EXCLUDE_FLOAT_PRINT */

#ifdef UNITY_INCLUDE_LOG
	void UnityPrintLog(void);
#endif

/*-------------------------------------------------------
 * Test Assertion Functions
 *-------------------------------------------------------
 *  Use the macros below this section instead of calling
 *  these directly. The macros have a consistent naming
 *  convention and will pull in file and line information
 *  for you. */

void UnityAssertEqualNumber(const UNITY_INT expected,
							const UNITY_INT actual,
							const char* msg,
							const UNITY_LINE_TYPE lineNumber,
							const UNITY_DISPLAY_STYLE__T style);

void UnityAssertGreaterOrLessOrEqualNumber(	const UNITY_INT threshold,
											const UNITY_INT actual,
											const UNITY_COMPARISON_T compare,
											const char *msg,
											const UNITY_LINE_TYPE lineNumber,
											const UNITY_DISPLAY_STYLE__T style);

void UnityAssertEqualIntArray(	UNITY_INTERNAL_PTR expected,
								UNITY_INTERNAL_PTR actual,
								const UNITY_UINT32 num_elements,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber,
								const UNITY_DISPLAY_STYLE__T style,
								const UNITY_FLAGS_T flags);

void UnityAssertBits(	const UNITY_INT mask,
						const UNITY_INT expected,
						const UNITY_INT actual,
						const char* msg,
						const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualString(const char* expected,
							const char* actual,
							const char* msg,
							const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualStringLen(	const char* expected,
								const char* actual,
								const UNITY_UINT32 length,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualStringArray(	UNITY_INTERNAL_PTR expected,
									const char** actual,
									const UNITY_UINT32 num_elements,
									const char* msg,
									const UNITY_LINE_TYPE lineNumber,
									const UNITY_FLAGS_T flags);

void UnityAssertEqualMemory(UNITY_INTERNAL_PTR expected,
							UNITY_INTERNAL_PTR actual,
							const UNITY_UINT32 length,
							const UNITY_UINT32 num_elements,
							const char* msg,
							const UNITY_LINE_TYPE lineNumber,
							const UNITY_FLAGS_T flags);

void UnityAssertNumbersWithin(	const UNITY_UINT delta,
								const UNITY_INT expected,
								const UNITY_INT actual,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber,
								const UNITY_DISPLAY_STYLE__T style);

void UnityAssertNumbersArrayWithin(	const UNITY_UINT delta,
									UNITY_INTERNAL_PTR expected,
									UNITY_INTERNAL_PTR actual,
									const UNITY_UINT32 num_elements,
									const char* msg,
									const UNITY_LINE_TYPE lineNumber,
									const UNITY_DISPLAY_STYLE__T style,
									const UNITY_FLAGS_T flags);

#ifndef UNITY_EXCLUDE_SETJMP_H
	UNITY_NORETURN void UnityFail(const char* message, const UNITY_LINE_TYPE line);
	UNITY_NORETURN void UnityIgnore(const char* message, const UNITY_LINE_TYPE line);
#else	/* UNITY_EXCLUDE_SETJMP_H */
	void UnityFail(const char* message, const UNITY_LINE_TYPE line);
	void UnityIgnore(const char* message, const UNITY_LINE_TYPE line);
#endif	/* UNITY_EXCLUDE_SETJMP_H */

void UnityMessage(const char* message, const UNITY_LINE_TYPE line);

#ifndef UNITY_EXCLUDE_FLOAT
void UnityAssertFloatsWithin(	const UNITY_FLOAT delta,
								const UNITY_FLOAT expected,
								const UNITY_FLOAT actual,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualFloatArray(UNITY_PTR_ATTRIBUTE const UNITY_FLOAT* expected,
								UNITY_PTR_ATTRIBUTE const UNITY_FLOAT* actual,
								const UNITY_UINT32 num_elements,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber,
								const UNITY_FLAGS_T flags);

void UnityAssertFloatSpecial(	const UNITY_FLOAT actual,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber,
								const UNITY_FLOAT_TRAIT_T style);
#endif	/* UNITY_EXCLUDE_FLOAT */

#ifndef UNITY_EXCLUDE_DOUBLE
void UnityAssertDoublesWithin(	const UNITY_DOUBLE delta,
								const UNITY_DOUBLE expected,
								const UNITY_DOUBLE actual,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualDoubleArray(	UNITY_PTR_ATTRIBUTE const UNITY_DOUBLE* expected,
									UNITY_PTR_ATTRIBUTE const UNITY_DOUBLE* actual,
									const UNITY_UINT32 num_elements,
									const char* msg,
									const UNITY_LINE_TYPE lineNumber,
									const UNITY_FLAGS_T flags);

void UnityAssertDoubleSpecial(	const UNITY_DOUBLE actual,
								const char* msg,
								const UNITY_LINE_TYPE lineNumber,
								const UNITY_FLOAT_TRAIT_T style);
#endif	/* UNITY_EXCLUDE_DOUBLE */

/*-------------------------------------------------------
 * Helpers
 *-------------------------------------------------------*/

UNITY_INTERNAL_PTR UnityNumToPtr(const UNITY_INT num, const UNITY_UINT8 size);
#ifndef UNITY_EXCLUDE_FLOAT
	UNITY_INTERNAL_PTR UnityFloatToPtr(const float num);
#endif	/* UNITY_EXCLUDE_FLOAT */
#ifndef UNITY_EXCLUDE_DOUBLE
	UNITY_INTERNAL_PTR UnityDoubleToPtr(const double num);
#endif	/* UNITY_EXCLUDE_DOUBLE */

/*-------------------------------------------------------
 * Error Strings We Might Need
 *-------------------------------------------------------*/

extern const char UnityStrOk[];
extern const char UnityStrPass[];
extern const char UnityStrFail[];
extern const char UnityStrIgnore[];

extern const char UnityStrErrFloat[];
extern const char UnityStrErrDouble[];
extern const char UnityStrErr64[];
extern const char UnityStrErrShorthand[];

/*-------------------------------------------------------
 * Test Running Macros
 *-------------------------------------------------------*/

#ifndef UNITY_EXCLUDE_SETJMP_H
	#define TEST_PROTECT()	(setjmp(Unity.AbortFrame) == 0)
	#define TEST_ABORT()	longjmp(Unity.AbortFrame, 1)
#else	/* UNITY_EXCLUDE_SETJMP_H */
	#define TEST_PROTECT()	1
	#define TEST_ABORT()	return
#endif	/* UNITY_EXCLUDE_SETJMP_H */

/* This tricky series of macros gives us an optional line argument to treat it as RUN_TEST(func, num=__LINE__) */
#ifndef RUN_TEST
	#ifdef __STDC_VERSION__
		#if __STDC_VERSION__ >= 199901L
			#define UNITY_SUPPORT_VARIADIC_MACROS
		#endif	/* UNITY_SUPPORT_VARIADIC_MACROS */
	#endif	/* __STDC_VERSION__ */
		#ifdef UNITY_SUPPORT_VARIADIC_MACROS
			#define RUN_TEST(...)						RUN_TEST_AT_LINE(__VA_ARGS__, __LINE__, throwaway)
			#define RUN_TEST_AT_LINE(func, line, ...)	UnityDefaultTestRun(func, #func, line)
		#endif	/* UNITY_SUPPORT_VARIADIC_MACROS */
	#endif	/* __STDC_VERSION__ */
#endif	/* RUN_TEST */

/* If we can't do the tricky version, we'll just have to require them to always include the line number */
#ifndef RUN_TEST
	#ifdef CMOCK
		#define RUN_TEST(func, num)	UnityDefaultTestRun(func, #func, num)
	#else	/* CMOCK */
		#define RUN_TEST(func)		UnityDefaultTestRun(func, #func, __LINE__)
	#endif	/* CMOCK */
#endif	/* RUN_TEST */

#define TEST_LINE_NUM		(Unity.CurrentTestLineNumber)
#define TEST_IS_IGNORED		(Unity.CurrentTestIgnored)
#define UNITY_NEW_TEST(a)												\
			Unity.CurrentTestName		= (a);							\
			Unity.CurrentTestLineNumber	= (UNITY_LINE_TYPE)(__LINE__);	\
			Unity.NumberOfTests			+= 1;

#ifndef UNITY_BEGIN
	#define UNITY_BEGIN()	UnityBegin(__FILE__)
#endif	/* UNITY_BEGIN */

#ifndef UNITY_END
	#define UNITY_END()		UnityEnd()
#endif	/* UNITY_END */

#ifndef UNITY_SHORTHAND_AS_INT
	#ifndef UNITY_SHORTHAND_AS_MEM
		#ifndef UNITY_SHORTHAND_AS_NONE
			#ifndef UNITY_SHORTHAND_AS_RAW
				#define UNITY_SHORTHAND_AS_OLD
			#endif	/* UNITY_SHORTHAND_AS_RAW */
		#endif	/* UNITY_SHORTHAND_AS_NONE */
	#endif	/* UNITY_SHORTHAND_AS_MEM */
#endif	/* UNITY_SHORTHAND_AS_INT */

/*-----------------------------------------------
 * Command Line Argument Support
 *-----------------------------------------------*/

#ifdef UNITY_USE_COMMAND_LINE_ARGS
	int UnityParseOptions(int argc, char** argv);
	int UnityTestMatches(void);
#endif	/* UNITY_USE_COMMAND_LINE_ARGS */

/*-------------------------------------------------------
 * Basic Fail and Ignore
 *-------------------------------------------------------*/

#define UNITY_TEST_FAIL(line, message)																	UnityFail(																																																													(message), (UNITY_LINE_TYPE)	(line))
#define UNITY_TEST_IGNORE(line, message)																UnityIgnore(																																																												(message), (UNITY_LINE_TYPE)	(line))

/*-------------------------------------------------------
 * Test Asserts
 *-------------------------------------------------------*/

#define UNITY_TEST_ASSERT(condition, line, message)														do {if (condition) {} else {UNITY_TEST_FAIL(																																																				(UNITY_LINE_TYPE)	(line),	(message));}} while(0)
#define UNITY_TEST_ASSERT__NULL(pointer, line, message)													UNITY_TEST_ASSERT(																																										((pointer) == NULL),												(UNITY_LINE_TYPE)	(line),	(message))
#define UNITY_TEST_ASSERT__EMPTY(pointer, line, message)												UNITY_TEST_ASSERT(																																										((pointer[0]) == 0),												(UNITY_LINE_TYPE)	(line),	(message))

#define UNITY_TEST_ASSERT__NOT_NULL(pointer, line, message)												UNITY_TEST_ASSERT(																																										((pointer) != NULL),												(UNITY_LINE_TYPE)	(line),	(message))
#define UNITY_TEST_ASSERT__NOT_EMPTY(pointer, line, message)											UNITY_TEST_ASSERT(																																										((pointer[0]) != 0),												(UNITY_LINE_TYPE)	(line),	(message))

#define UNITY_TEST_ASSERT__EQUAL__INT(expected, actual, line, message)									UnityAssertEqualNumber(										(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT)
#define UNITY_TEST_ASSERT__EQUAL__INT8(expected, actual, line, message)									UnityAssertEqualNumber(										(UNITY_INT)(UNITY_INT8)					(expected),									(UNITY_INT)(UNITY_INT8)					(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8)
#define UNITY_TEST_ASSERT__EQUAL__INT16(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)(UNITY_INT16)				(expected),									(UNITY_INT)(UNITY_INT16)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16)
#define UNITY_TEST_ASSERT__EQUAL__INT32(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)(UNITY_INT32)				(expected),									(UNITY_INT)(UNITY_INT32)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32)
#define UNITY_TEST_ASSERT__EQUAL__UINT(expected, actual, line, message)									UnityAssertEqualNumber(										(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT)
#define UNITY_TEST_ASSERT__EQUAL__UINT8(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)(UNITY_UINT8)				(expected),									(UNITY_INT)(UNITY_UINT8)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8)
#define UNITY_TEST_ASSERT__EQUAL__UINT16(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)(UNITY_UINT16)				(expected),									(UNITY_INT)(UNITY_UINT16)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16)
#define UNITY_TEST_ASSERT__EQUAL__UINT32(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)(UNITY_UINT32)				(expected),									(UNITY_INT)(UNITY_UINT32)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32)
#define UNITY_TEST_ASSERT__EQUAL__HEX8(expected, actual, line, message)									UnityAssertEqualNumber(										(UNITY_INT)(UNITY_INT8)					(expected),									(UNITY_INT)(UNITY_INT8)					(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8)
#define UNITY_TEST_ASSERT__EQUAL__HEX16(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)(UNITY_INT16)				(expected),									(UNITY_INT)(UNITY_INT16)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16)
#define UNITY_TEST_ASSERT__EQUAL__HEX32(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)(UNITY_INT32)				(expected),									(UNITY_INT)(UNITY_INT32)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32)
#define UNITY_TEST_ASSERT__EQUAL__CHAR(expected, actual, line, message)									UnityAssertEqualNumber(										(UNITY_INT)(UNITY_INT8)					(expected),									(UNITY_INT)(UNITY_INT8)					(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR)
#define UNITY_TEST_ASSERT__EQUAL__BITS(mask, expected, actual, line, message)							UnityAssertBits(					(UNITY_INT)(mask),		(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line))

#define UNITY_TEST_ASSERT__EQUAL__PTR(expected, actual, line, message)									UnityAssertEqualNumber(										(UNITY_PTR_TO_INT)						(expected),									(UNITY_PTR_TO_INT)						(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__POINTER)
#define UNITY_TEST_ASSERT__EQUAL__STRING(expected, actual, line, message)								UnityAssertEqualString(										(const char*)							(expected),									(const char*)							(actual),															(message),	(UNITY_LINE_TYPE)	(line))
#define UNITY_TEST_ASSERT__EQUAL__STRING_LEN(expected, actual, len, line, message)						UnityAssertEqualStringLen(									(const char*)							(expected),									(const char*)							(actual),	(UNITY_UINT32)(len),									(message),	(UNITY_LINE_TYPE)	(line))
#define UNITY_TEST_ASSERT__EQUAL__MEMORY(expected, actual, len, line, message)							UnityAssertEqualMemory(										(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),	(UNITY_UINT32)(len), 					1,				(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_ARRAY)

#define UNITY_TEST_ASSERT__EQUAL_ARRAY__INT(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__INT8(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__INT16(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__INT32(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT8(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT16(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16,	UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT32(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32,	UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX8(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX16(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX32(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__PTR(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__POINTER,	UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__STRING(expected, actual, num_elements, line, message)			UnityAssertEqualStringArray(								(UNITY_INTERNAL_PTR)					(expected),									(const char**)							(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__MEMORY(expected, actual, len, num_elements, line, message)		UnityAssertEqualMemory(										(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),	(UNITY_UINT32)(len),	(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__CHAR(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR,		UNITY_FLAGS__ARRAY_TO_ARRAY)

#define UNITY_TEST_ASSERT__EQUAL_EACH__INT(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)								(expected),	(UNITY_INT_WIDTH / 8)),			(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__INT8(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT8)					(expected),	1),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__INT16(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT16)				(expected),	2),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__INT32(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT32)				(expected),	4),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__UINT(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)								(expected),	(UNITY_INT_WIDTH / 8)),			(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__UINT8(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_UINT8)				(expected),	1),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__UINT16(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_UINT16)				(expected),	2),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16,	UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__UINT32(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_UINT32)				(expected),	4),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32,	UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__HEX8(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT8)					(expected),	1),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__HEX16(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT16)				(expected),	2),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__HEX32(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT32)				(expected),	4),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__PTR(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_PTR_TO_INT)						(expected),	(UNITY_POINTER_WIDTH / 8)),		(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__POINTER,	UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__STRING(expected, actual, num_elements, line, message)			UnityAssertEqualStringArray(								(UNITY_INTERNAL_PTR)					(expected),									(const char**)							(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__MEMORY(expected, actual, len, num_elements, line, message)		UnityAssertEqualMemory(										(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),	(UNITY_UINT32)(len),	(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__CHAR(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT8)					(expected),	1),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR,		UNITY_FLAGS__ARRAY_TO_VAL)

#define UNITY_TEST_ASSERT__NOT_EQUAL__INT(threshold, actual, line, message)								UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT)
#define UNITY_TEST_ASSERT__NOT_EQUAL__INT8(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8)
#define UNITY_TEST_ASSERT__NOT_EQUAL__INT16(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT16)				(threshold),								(UNITY_INT)(UNITY_INT16)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16)
#define UNITY_TEST_ASSERT__NOT_EQUAL__INT32(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT32)				(threshold),								(UNITY_INT)(UNITY_INT32)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32)
#define UNITY_TEST_ASSERT__NOT_EQUAL__UINT(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT)
#define UNITY_TEST_ASSERT__NOT_EQUAL__UINT8(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8)
#define UNITY_TEST_ASSERT__NOT_EQUAL__UINT16(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16)
#define UNITY_TEST_ASSERT__NOT_EQUAL__UINT32(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32)
#define UNITY_TEST_ASSERT__NOT_EQUAL__HEX8(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8)
#define UNITY_TEST_ASSERT__NOT_EQUAL__HEX16(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16)
#define UNITY_TEST_ASSERT__NOT_EQUAL__HEX32(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32)
#define UNITY_TEST_ASSERT__NOT_EQUAL__CHAR(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR)

#define UNITY_TEST_ASSERT__GREATER_THAN__INT(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT)
#define UNITY_TEST_ASSERT__GREATER_THAN__INT8(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8)
#define UNITY_TEST_ASSERT__GREATER_THAN__INT16(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT16)				(threshold),								(UNITY_INT)(UNITY_INT16)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16)
#define UNITY_TEST_ASSERT__GREATER_THAN__INT32(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT32)				(threshold),								(UNITY_INT)(UNITY_INT32)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32)
#define UNITY_TEST_ASSERT__GREATER_THAN__UINT(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT)
#define UNITY_TEST_ASSERT__GREATER_THAN__UINT8(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8)
#define UNITY_TEST_ASSERT__GREATER_THAN__UINT16(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16)
#define UNITY_TEST_ASSERT__GREATER_THAN__UINT32(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32)
#define UNITY_TEST_ASSERT__GREATER_THAN__HEX8(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8)
#define UNITY_TEST_ASSERT__GREATER_THAN__HEX16(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16)
#define UNITY_TEST_ASSERT__GREATER_THAN__HEX32(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32)
#define UNITY_TEST_ASSERT__GREATER_THAN__CHAR(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR)

#define UNITY_TEST_ASSERT__SMALLER_THAN__INT(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT)
#define UNITY_TEST_ASSERT__SMALLER_THAN__INT8(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8)
#define UNITY_TEST_ASSERT__SMALLER_THAN__INT16(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT16)				(threshold),								(UNITY_INT)(UNITY_INT16)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16)
#define UNITY_TEST_ASSERT__SMALLER_THAN__INT32(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT32)				(threshold),								(UNITY_INT)(UNITY_INT32)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32)
#define UNITY_TEST_ASSERT__SMALLER_THAN__UINT(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT)
#define UNITY_TEST_ASSERT__SMALLER_THAN__UINT8(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8)
#define UNITY_TEST_ASSERT__SMALLER_THAN__UINT16(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16)
#define UNITY_TEST_ASSERT__SMALLER_THAN__UINT32(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32)
#define UNITY_TEST_ASSERT__SMALLER_THAN__HEX8(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8)
#define UNITY_TEST_ASSERT__SMALLER_THAN__HEX16(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16)
#define UNITY_TEST_ASSERT__SMALLER_THAN__HEX32(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32)
#define UNITY_TEST_ASSERT__SMALLER_THAN__CHAR(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR)

#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT8(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT16(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT16)				(threshold),								(UNITY_INT)(UNITY_INT16)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT32(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT32)				(threshold),								(UNITY_INT)(UNITY_INT32)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT8(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT16(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT32(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX8(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX16(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX32(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__CHAR(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR)

#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT8(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT16(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT16)				(threshold),								(UNITY_INT)(UNITY_INT16)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT32(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT32)				(threshold),								(UNITY_INT)(UNITY_INT32)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT8(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT16(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT32(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX8(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT8)				(threshold),								(UNITY_INT)(UNITY_UINT8)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX16(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT16)				(threshold),								(UNITY_INT)(UNITY_UINT16)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX32(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_UINT32)				(threshold),								(UNITY_INT)(UNITY_UINT32)				(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__CHAR(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)(UNITY_INT8)					(threshold),								(UNITY_INT)(UNITY_INT8)					(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR)

#define UNITY_TEST_ASSERT__WITHIN__INT(delta, expected, actual, line, message)							UnityAssertNumbersWithin(						(delta),	(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT)
#define UNITY_TEST_ASSERT__WITHIN__INT8(delta, expected, actual, line, message)							UnityAssertNumbersWithin(		(UNITY_UINT8)	(delta),	(UNITY_INT)(UNITY_INT8)					(expected),									(UNITY_INT)(UNITY_INT8)					(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8)
#define UNITY_TEST_ASSERT__WITHIN__INT16(delta, expected, actual, line, message)						UnityAssertNumbersWithin(		(UNITY_UINT16)	(delta),	(UNITY_INT)(UNITY_INT16)				(expected),									(UNITY_INT)(UNITY_INT16)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16)
#define UNITY_TEST_ASSERT__WITHIN__INT32(delta, expected, actual, line, message)						UnityAssertNumbersWithin(		(UNITY_UINT32)	(delta),	(UNITY_INT)(UNITY_INT32)				(expected),									(UNITY_INT)(UNITY_INT32)				(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32)
#define UNITY_TEST_ASSERT__WITHIN__UINT(delta, expected, actual, line, message)							UnityAssertNumbersWithin(						(delta),	(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT)
#define UNITY_TEST_ASSERT__WITHIN__UINT8(delta, expected, actual, line, message)						UnityAssertNumbersWithin(		(UNITY_UINT8)	(delta),	(UNITY_INT)(UNITY_UINT)(UNITY_UINT8)	(expected),									(UNITY_INT)(UNITY_UINT)(UNITY_UINT8)	(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8)
#define UNITY_TEST_ASSERT__WITHIN__UINT16(delta, expected, actual, line, message)						UnityAssertNumbersWithin(		(UNITY_UINT16)	(delta),	(UNITY_INT)(UNITY_UINT)(UNITY_UINT16)	(expected),									(UNITY_INT)(UNITY_UINT)(UNITY_UINT16)	(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16)
#define UNITY_TEST_ASSERT__WITHIN__UINT32(delta, expected, actual, line, message)						UnityAssertNumbersWithin(		(UNITY_UINT32)	(delta),	(UNITY_INT)(UNITY_UINT)(UNITY_UINT32)	(expected),									(UNITY_INT)(UNITY_UINT)(UNITY_UINT32)	(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32)
#define UNITY_TEST_ASSERT__WITHIN__HEX8(delta, expected, actual, line, message)							UnityAssertNumbersWithin(		(UNITY_UINT8)	(delta),	(UNITY_INT)(UNITY_UINT)(UNITY_UINT8)	(expected),									(UNITY_INT)(UNITY_UINT)(UNITY_UINT8)	(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8)
#define UNITY_TEST_ASSERT__WITHIN__HEX16(delta, expected, actual, line, message)						UnityAssertNumbersWithin(		(UNITY_UINT16)	(delta),	(UNITY_INT)(UNITY_UINT)(UNITY_UINT16)	(expected),									(UNITY_INT)(UNITY_UINT)(UNITY_UINT16)	(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16)
#define UNITY_TEST_ASSERT__WITHIN__HEX32(delta, expected, actual, line, message)						UnityAssertNumbersWithin(		(UNITY_UINT32)	(delta),	(UNITY_INT)(UNITY_UINT)(UNITY_UINT32)	(expected),									(UNITY_INT)(UNITY_UINT)(UNITY_UINT32)	(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32)
#define UNITY_TEST_ASSERT__WITHIN__CHAR(delta, expected, actual, line, message)							UnityAssertNumbersWithin(		(UNITY_UINT8)	(delta),	(UNITY_INT)(UNITY_INT8)					(expected),									(UNITY_INT)(UNITY_INT8)					(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR)

#define UNITY_TEST_ASSERT__WITHIN_ARRAY__INT(delta, expected, actual, num_elements, line, message)		UnityAssertNumbersArrayWithin(					(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__INT8(delta, expected, actual, num_elements, line, message)		UnityAssertNumbersArrayWithin(	(UNITY_UINT8)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT8,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__INT16(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT16)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT16,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__INT32(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT32)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT32,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT(delta, expected, actual, num_elements, line, message)		UnityAssertNumbersArrayWithin(					(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT8(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT16)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT8,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT16(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT16)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT16,	UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT32(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT32)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT32,	UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX8(delta, expected, actual, num_elements, line, message)		UnityAssertNumbersArrayWithin(	(UNITY_UINT8)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX8,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX16(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT16)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX16,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX32(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT32)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX32,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__CHAR(delta, expected, actual, num_elements, line, message)		UnityAssertNumbersArrayWithin(	(UNITY_UINT8)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__CHAR,		UNITY_FLAGS__ARRAY_TO_ARRAY)

#ifdef UNITY_SUPPORT_64
#define UNITY_TEST_ASSERT__EQUAL__INT64(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64)
#define UNITY_TEST_ASSERT__EQUAL__UINT64(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64)
#define UNITY_TEST_ASSERT__EQUAL__HEX64(expected, actual, line, message)								UnityAssertEqualNumber(										(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__INT64(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT64(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64,	UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX64(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(									(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_EACH__INT64(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT64)				(expected),	8),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__UINT64(expected, actual, num_elements, line, message)			UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_UINT64)				(expected),	8),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64,	UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__EQUAL_EACH__HEX64(expected, actual, num_elements, line, message)				UnityAssertEqualIntArray(UnityNumToPtr(						(UNITY_INT)(UNITY_INT64)				(expected),	8),								(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64,		UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__NOT_EQUAL__INT64(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64)
#define UNITY_TEST_ASSERT__NOT_EQUAL__UINT64(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64)
#define UNITY_TEST_ASSERT__NOT_EQUAL__HEX64(threshold, actual, line, message)							UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__NOT_EQUAL,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64)
#define UNITY_TEST_ASSERT__GREATER_THAN__INT64(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64)
#define UNITY_TEST_ASSERT__GREATER_THAN__UINT64(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64)
#define UNITY_TEST_ASSERT__GREATER_THAN__HEX64(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT64(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT64(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX64(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__GREATER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64)
#define UNITY_TEST_ASSERT__SMALLER_THAN__INT64(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64)
#define UNITY_TEST_ASSERT__SMALLER_THAN__UINT64(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64)
#define UNITY_TEST_ASSERT__SMALLER_THAN__HEX64(threshold, actual, line, message)						UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_THAN,							(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT64(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT64(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX64(threshold, actual, line, message)					UnityAssertGreaterOrLessOrEqualNumber(						(UNITY_INT)								(threshold),								(UNITY_INT)								(actual),	UNITY_COMPARISON__SMALLER_OR_EQUAL,						(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64)
#define UNITY_TEST_ASSERT__WITHIN__INT64(delta, expected, actual, line, message)						UnityAssertNumbersWithin(						(delta),	(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64)
#define UNITY_TEST_ASSERT__WITHIN__UINT64(delta, expected, actual, line, message)						UnityAssertNumbersWithin(						(delta),	(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64)
#define UNITY_TEST_ASSERT__WITHIN__HEX64(delta, expected, actual, line, message)						UnityAssertNumbersWithin(						(delta),	(UNITY_INT)								(expected),									(UNITY_INT)								(actual),															(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__INT64(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT64)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__INT64,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT64(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT64)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__UINT64,	UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX64(delta, expected, actual, num_elements, line, message)	UnityAssertNumbersArrayWithin(	(UNITY_UINT64)	(delta),	(UNITY_INTERNAL_PTR)					(expected),									(UNITY_INTERNAL_PTR)					(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),	UNITY_DISPLAY_STYLE__HEX64,		UNITY_FLAGS__ARRAY_TO_ARRAY)
#else	/* UNITY_SUPPORT_64 */
#define UNITY_TEST_ASSERT__EQUAL__INT64(expected, actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL__UINT64(expected, actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL__HEX64(expected, actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__INT64(expected, actual, num_elements, line, message)			UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT64(expected, actual, num_elements, line, message)			UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX64(expected, actual, num_elements, line, message)			UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL_EACH__INT64(expected, actual, num_elements, line, message)				UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL_EACH__UINT64(expected, actual, num_elements, line, message)			UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__EQUAL_EACH__HEX64(expected, actual, num_elements, line, message)				UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__GREATER_THAN__INT64(threshold, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__GREATER_THAN__UINT64(threshold, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__GREATER_THAN__HEX64(threshold, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT64(threshold, actual, line, message)					UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT64(threshold, actual, line, message)					UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX64(threshold, actual, line, message)					UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__SMALLER_THAN__INT64(threshold, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__SMALLER_THAN__UINT64(threshold, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__SMALLER_THAN__HEX64(threshold, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT64(threshold, actual, line, message)					UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT64(threshold, actual, line, message)					UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX64(threshold, actual, line, message)					UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__WITHIN__INT64(delta, expected, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__WITHIN__UINT64(delta, expected, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__WITHIN__HEX64(delta, expected, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__INT64(delta, expected, actual, num_elements, line, message)	UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT64(delta, expected, actual, num_elements, line, message)	UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#define UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX64(delta, expected, actual, num_elements, line, message)	UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErr64)
#endif	/* UNITY_SUPPORT_64 */

#ifdef UNITY_EXCLUDE_FLOAT
#define UNITY_TEST_ASSERT__EQUAL__FLOAT(expected, actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__FLOAT(expected, actual, num_elements, line, message)			UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__EQUAL_EACH__FLOAT(expected, actual, num_elements, line, message)				UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__WITHIN__FLOAT(delta, expected, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_INF(actual, line, message)											UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_NEG_INF(actual, line, message)										UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_NAN(actual, line, message)											UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_DETERMINATE(actual, line, message)									UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_INF(actual, line, message)										UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_NEG_INF(actual, line, message)									UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_NAN(actual, line, message)										UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_DETERMINATE(actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrFloat)
#else	/* UNITY_EXCLUDE_FLOAT */
#define UNITY_TEST_ASSERT__EQUAL__FLOAT(expected, actual, line, message)								UNITY_TEST_ASSERT__WITHIN__FLOAT(	(UNITY_FLOAT)	(expected) * (UNITY_FLOAT)UNITY_FLOAT_PRECISION, (UNITY_FLOAT)	(expected),					(UNITY_FLOAT)							(actual),																		(UNITY_LINE_TYPE)	(line),	(message))
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__FLOAT(expected, actual, num_elements, line, message)			UnityAssertEqualFloatArray(									(UNITY_FLOAT*)							(expected),									(UNITY_FLOAT*)							(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_EACH__FLOAT(expected, actual, num_elements, line, message)				UnityAssertEqualFloatArray(UnityFloatToPtr															(expected),									(UNITY_FLOAT*)							(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__WITHIN__FLOAT(delta, expected, actual, line, message)						UnityAssertFloatsWithin(		(UNITY_FLOAT)	(delta),	(UNITY_FLOAT)							(expected),									(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line))
#define UNITY_TEST_ASSERT__FLOAT__IS_INF(actual, line, message)											UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_INF)
#define UNITY_TEST_ASSERT__FLOAT__IS_NEG_INF(actual, line, message)										UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NEG_INF)
#define UNITY_TEST_ASSERT__FLOAT__IS_NAN(actual, line, message)											UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NAN)
#define UNITY_TEST_ASSERT__FLOAT__IS_DETERMINATE(actual, line, message)									UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_DET)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_INF(actual, line, message)										UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_INF)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_NEG_INF(actual, line, message)									UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_NEG_INF)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_NAN(actual, line, message)										UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_NAN)
#define UNITY_TEST_ASSERT__FLOAT__IS_NOT_DETERMINATE(actual, line, message)								UnityAssertFloatSpecial(																														(UNITY_FLOAT)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_DET)
#endif	/* UNITY_EXCLUDE_FLOAT */

#ifdef UNITY_EXCLUDE_DOUBLE
#define UNITY_TEST_ASSERT__EQUAL__DOUBLE(expected, actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__DOUBLE(expected, actual, num_elements, line, message)			UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__EQUAL_EACH__DOUBLE(expected, actual, num_elements, line, message)			UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__WITHIN__DOUBLE(delta, expected, actual, line, message)						UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_INF(actual, line, message)										UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NEG_INF(actual, line, message)									UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NAN(actual, line, message)										UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_DETERMINATE(actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_INF(actual, line, message)									UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NEG_INF(actual, line, message)								UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NAN(actual, line, message)									UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_DETERMINATE(actual, line, message)							UNITY_TEST_FAIL(																																																														(UNITY_LINE_TYPE)	(line),	UnityStrErrDouble)
#else	/* UNITY_EXCLUDE_DOUBLE */
#define UNITY_TEST_ASSERT__EQUAL__DOUBLE(expected, actual, line, message)								UNITY_TEST_ASSERT__WITHIN__DOUBLE(	(UNITY_DOUBLE)	(expected) * (UNITY_DOUBLE)UNITY_DOUBLE_PRECISION, (UNITY_DOUBLE)	(expected),				(UNITY_DOUBLE)							(actual),																		(UNITY_LINE_TYPE)	(line),	(message))
#define UNITY_TEST_ASSERT__EQUAL_ARRAY__DOUBLE(expected, actual, num_elements, line, message)			UnityAssertEqualDoubleArray(								(UNITY_DOUBLE*)							(expected),									(UNITY_DOUBLE*)							(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_ARRAY)
#define UNITY_TEST_ASSERT__EQUAL_EACH__DOUBLE(expected, actual, num_elements, line, message)			UnityAssertEqualDoubleArray(UnityDoubleToPtr														(expected),									(UNITY_DOUBLE*)							(actual),							(UNITY_UINT32)	(num_elements),	(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLAGS__ARRAY_TO_VAL)
#define UNITY_TEST_ASSERT__WITHIN__DOUBLE(delta, expected, actual, line, message)						UnityAssertDoublesWithin(		(UNITY_DOUBLE)	(delta),	(UNITY_DOUBLE)							(expected),									(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line))
#define UNITY_TEST_ASSERT__DOUBLE__IS_INF(actual, line, message)										UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_INF)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NEG_INF(actual, line, message)									UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NEG_INF)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NAN(actual, line, message)										UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NAN)
#define UNITY_TEST_ASSERT__DOUBLE__IS_DETERMINATE(actual, line, message)								UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_DET)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_INF(actual, line, message)									UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_INF)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NEG_INF(actual, line, message)								UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_NEG_INF)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NAN(actual, line, message)									UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_NAN)
#define UNITY_TEST_ASSERT__DOUBLE__IS_NOT_DETERMINATE(actual, line, message)							UnityAssertDoubleSpecial(																														(UNITY_DOUBLE)							(actual),															(message),	(UNITY_LINE_TYPE)	(line),									UNITY_FLOAT_TRAIT__IS_NOT_DET)
#endif	/* UNITY_EXCLUDE_DOUBLE */

#endif	/* UNITY_INTERNALS_H */
