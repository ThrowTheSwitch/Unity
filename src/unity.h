/* ==========================================
		Unity Project - A Test Framework for C
		Copyright (c) 2007-21 Mike Karlesky, Mark VanderVoord, Greg Williams
		[Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_FRAMEWORK_H
#define UNITY_FRAMEWORK_H
#define UNITY

#define UNITY_VERSION_MAJOR	2
#define UNITY_VERSION_MINOR	5
#define UNITY_VERSION_BUILD	4
#define UNITY_VERSION		((UNITY_VERSION_MAJOR << 16) | (UNITY_VERSION_MINOR << 8) | UNITY_VERSION_BUILD)

#ifdef __cplusplus
extern "C"
{
#endif

#include "unity_internals.h"

/*-------------------------------------------------------
 * Test Setup / Teardown
 *-------------------------------------------------------*/

/* These functions are intended to be called before and after each test.
 * If using unity directly, these will need to be provided for each test
 * executable built. If you are using the test runner generator and/or
 * Ceedling, these are optional. */
void setUp(void);
void tearDown(void);

/* These functions are intended to be called at the beginning and end of an
 * entire test suite.  suiteTearDown() is passed the number of tests that
 * failed, and its return value becomes the exit code of main(). If using
 * Unity directly, you're in charge of calling these if they are desired.
 * If using Ceedling or the test runner generator, these will be called
 * automatically if they exist. */
void suiteSetUp(void);
int suiteTearDown(int num_failures);

/*-------------------------------------------------------
 * Test Reset and Verify
 *-------------------------------------------------------*/

/* These functions are intended to be called before during tests in order
 * to support complex test loops, etc. Both are NOT built into Unity. Instead
 * the test runner generator will create them. resetTest will run teardown and
 * setup again, verifying any end-of-test needs between. verifyTest will only
 * run the verification. */
void resetTest(void);
void verifyTest(void);

/*-------------------------------------------------------
 * Configuration Options
 *-------------------------------------------------------
 * All options described below should be passed as a compiler flag to all files using Unity. If you must add #defines, place them BEFORE the #include above.

 * Integers/longs/pointers
 *     - Unity attempts to automatically discover your integer sizes
 *       - define UNITY_EXCLUDE_STDINT_H to stop attempting to look in <stdint.h>
 *       - define UNITY_EXCLUDE_LIMITS_H to stop attempting to look in <limits.h>
 *     - If you cannot use the automatic methods above, you can force Unity by using these options:
 *       - define UNITY_SUPPORT_64
 *       - set UNITY_INT_WIDTH
 *       - set UNITY_LONG_WIDTH
 *       - set UNITY_POINTER_WIDTH

 * Floats
 *     - define UNITY_EXCLUDE_FLOAT to disallow floating point comparisons
 *     - define UNITY_FLOAT_PRECISION to specify the precision to use when doing TEST_ASSERT__EQUAL__FLOAT
 *     - define UNITY_FLOAT_TYPE to specify doubles instead of single precision floats
 *     - define UNITY_INCLUDE_DOUBLE to allow double floating point comparisons
 *     - define UNITY_EXCLUDE_DOUBLE to disallow double floating point comparisons (default)
 *     - define UNITY_DOUBLE_PRECISION to specify the precision to use when doing TEST_ASSERT__EQUAL__DOUBLE
 *     - define UNITY_DOUBLE_TYPE to specify something other than double
 *     - define UNITY_EXCLUDE_FLOAT_PRINT to trim binary size, won't print floating point values in errors

 * Output
 *     - by default, Unity prints to standard out with putchar.  define UNITY_OUTPUT_CHAR(a) with a different function if desired
 *     - define UNITY_DIFFERENTIATE_FINAL_FAIL to print FAILED (vs. FAIL) at test end summary - for automated search for failure

 * Optimization
 *     - by default, line numbers are stored in unsigned shorts.  Define UNITY_LINE_TYPE with a different type if your files are huge
 *     - by default, test and failure counters are unsigned shorts.  Define UNITY_COUNTER_TYPE with a different type if you want to save space or have more than 65535 Tests.

 * Test Cases
 *     - define UNITY_SUPPORT_TEST_CASES to include the TEST_CASE macro, though really it's mostly about the runner generator script

 * Parameterized Tests
 *     - you'll want to create a define of TEST_CASE(...) which basically evaluates to nothing

 * Tests with Arguments
 *     - you'll want to define UNITY_USE_COMMAND_LINE_ARGS if you have the test runner passing arguments to Unity

 *-------------------------------------------------------
 * Basic Fail and Ignore
 *-------------------------------------------------------*/

#define TEST_FAIL_MESSAGE(message)																	UNITY_TEST_FAIL(																									__LINE__,	(message))
#define TEST_FAIL()																					UNITY_TEST_FAIL(																									__LINE__,	NULL)
#define TEST_IGNORE_MESSAGE(message)																UNITY_TEST_IGNORE(																									__LINE__,	(message))
#define TEST_IGNORE()																				UNITY_TEST_IGNORE(																									__LINE__,	NULL)
#define TEST_MESSAGE(message)																		UnityMessage(																							(message),	__LINE__)
#define TEST_ONLY()
#ifdef UNITY_INCLUDE_PRINT_FORMATTED
#define TEST_PRINTF(message, ...)																	UnityPrintF(																										__LINE__, (message), __VA_ARGS__)
#endif

/* It is not necessary for you to call PASS. A PASS condition is assumed if nothing fails.
 * This method allows you to abort a test immediately with a PASS state, ignoring the remainder of the test. */
#define TEST_PASS()																					TEST_ABORT()
#define TEST_PASS_MESSAGE(message)																	do { UnityMessage(																						(message),	__LINE__); TEST_ABORT(); } while(0)

/* This macro does nothing, but it is useful for build tools (like Ceedling) to make use of this to figure out
 * which files should be linked to in order to perform a test. Use it like TEST_FILE("sandwiches.c") */
#define TEST_FILE(a)

/*-------------------------------------------------------
 * Test Asserts (simple)
 *-------------------------------------------------------*/

/* Boolean */
#define TEST_ASSERT(condition)																		UNITY_TEST_ASSERT(																(condition),						__LINE__,	" Expression Evaluated To FALSE")
#define TEST_ASSERT__TRUE(condition)																UNITY_TEST_ASSERT(																(condition),						__LINE__,	" Expected TRUE Was FALSE")
#define TEST_ASSERT__UNLESS(condition)																UNITY_TEST_ASSERT(																!(condition),						__LINE__,	" Expression Evaluated To TRUE")
#define TEST_ASSERT__FALSE(condition)																UNITY_TEST_ASSERT(																!(condition),						__LINE__,	" Expected FALSE Was TRUE")
#define TEST_ASSERT__NULL(pointer)																	UNITY_TEST_ASSERT__NULL(														(pointer),							__LINE__,	" Expected NULL")
#define TEST_ASSERT__NOT_NULL(pointer)																UNITY_TEST_ASSERT__NOT_NULL(													(pointer),							__LINE__,	" Expected Non-NULL")
#define TEST_ASSERT__EMPTY(pointer)																	UNITY_TEST_ASSERT__EMPTY(														(pointer),							__LINE__,	" Expected Empty")
#define TEST_ASSERT__NOT_EMPTY(pointer)																UNITY_TEST_ASSERT__NOT_EMPTY(													(pointer),							__LINE__,	" Expected Non-Empty")

/* Integers (of all sizes) */
#define TEST_ASSERT__EQUAL__INT(expected, actual)													UNITY_TEST_ASSERT__EQUAL__INT(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__INT8(expected, actual)													UNITY_TEST_ASSERT__EQUAL__INT8(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__INT16(expected, actual)													UNITY_TEST_ASSERT__EQUAL__INT16(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__INT32(expected, actual)													UNITY_TEST_ASSERT__EQUAL__INT32(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__INT64(expected, actual)													UNITY_TEST_ASSERT__EQUAL__INT64(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__UINT(expected, actual)													UNITY_TEST_ASSERT__EQUAL__UINT(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__UINT8(expected, actual)													UNITY_TEST_ASSERT__EQUAL__UINT8(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__UINT16(expected, actual)												UNITY_TEST_ASSERT__EQUAL__UINT16(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__UINT32(expected, actual)												UNITY_TEST_ASSERT__EQUAL__UINT32(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__UINT64(expected, actual)												UNITY_TEST_ASSERT__EQUAL__UINT64(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__size_t(expected, actual)												UNITY_TEST_ASSERT__EQUAL__UINT(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__HEX(expected, actual)													UNITY_TEST_ASSERT__EQUAL__HEX32(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__HEX8(expected, actual)													UNITY_TEST_ASSERT__EQUAL__HEX8(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__HEX16(expected, actual)													UNITY_TEST_ASSERT__EQUAL__HEX16(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__HEX32(expected, actual)													UNITY_TEST_ASSERT__EQUAL__HEX32(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__HEX64(expected, actual)													UNITY_TEST_ASSERT__EQUAL__HEX64(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__CHAR(expected, actual)													UNITY_TEST_ASSERT__EQUAL__CHAR(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__BITS(mask, expected, actual)											UNITY_TEST_ASSERT__EQUAL__BITS(	(mask),						(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__BITS_HIGH(mask, actual)													UNITY_TEST_ASSERT__EQUAL__BITS(	(mask),						(UNITY_UINT)(-1),	(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__BITS_LOW(mask, actual)													UNITY_TEST_ASSERT__EQUAL__BITS(	(mask),						(UNITY_UINT)(0),	(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__BIT_HIGH(bit, actual)													UNITY_TEST_ASSERT__EQUAL__BITS(	((UNITY_UINT)1 << (bit)),	(UNITY_UINT)(-1),	(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__BIT_LOW(bit, actual)													UNITY_TEST_ASSERT__EQUAL__BITS(	((UNITY_UINT)1 << (bit)),	(UNITY_UINT)(0),	(actual),							__LINE__,	NULL)

/* Integer Not Equal To (of all sizes) */
#define TEST_ASSERT__NOT_EQUAL__INT(threshold, actual)												UNITY_TEST_ASSERT__NOT_EQUAL__INT(							(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__INT8(threshold, actual)												UNITY_TEST_ASSERT__NOT_EQUAL__INT8(							(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__INT16(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__INT16(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__INT32(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__INT32(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__INT64(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__INT64(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__UINT(threshold, actual)												UNITY_TEST_ASSERT__NOT_EQUAL__UINT(							(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__UINT8(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__UINT8(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__UINT16(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__UINT16(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__UINT32(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__UINT32(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__UINT64(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__UINT64(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__size_t(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__UINT(							(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__HEX8(threshold, actual)												UNITY_TEST_ASSERT__NOT_EQUAL__HEX8(							(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__HEX16(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__HEX16(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__HEX32(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__HEX32(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__HEX64(threshold, actual)											UNITY_TEST_ASSERT__NOT_EQUAL__HEX64(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL__CHAR(threshold, actual)												UNITY_TEST_ASSERT__NOT_EQUAL__CHAR(							(threshold),		(actual),							__LINE__,	NULL)

/* Integer Greater Than/ Less Than (of all sizes) */
#define TEST_ASSERT__GREATER_THAN(threshold, actual)												UNITY_TEST_ASSERT__GREATER_THAN__INT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__INT(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__INT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__INT8(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__INT8(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__INT16(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__INT16(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__INT32(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__INT32(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__INT64(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__INT64(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__UINT(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__UINT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__UINT8(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__UINT8(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__UINT16(threshold, actual)										UNITY_TEST_ASSERT__GREATER_THAN__UINT16(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__UINT32(threshold, actual)										UNITY_TEST_ASSERT__GREATER_THAN__UINT32(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__UINT64(threshold, actual)										UNITY_TEST_ASSERT__GREATER_THAN__UINT64(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__size_t(threshold, actual)										UNITY_TEST_ASSERT__GREATER_THAN__UINT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__HEX8(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__HEX8(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__HEX16(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__HEX16(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__HEX32(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__HEX32(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__HEX64(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__HEX64(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_THAN__CHAR(threshold, actual)											UNITY_TEST_ASSERT__GREATER_THAN__CHAR(						(threshold),		(actual),							__LINE__,	NULL)

#define TEST_ASSERT__LESS_THAN(threshold, actual)													UNITY_TEST_ASSERT__SMALLER_THAN__INT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__INT(threshold, actual)												UNITY_TEST_ASSERT__SMALLER_THAN__INT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__INT8(threshold, actual)												UNITY_TEST_ASSERT__SMALLER_THAN__INT8(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__INT16(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__INT16(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__INT32(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__INT32(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__INT64(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__INT64(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__UINT(threshold, actual)												UNITY_TEST_ASSERT__SMALLER_THAN__UINT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__UINT8(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__UINT8(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__UINT16(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__UINT16(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__UINT32(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__UINT32(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__UINT64(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__UINT64(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__size_t(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__UINT(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__HEX8(threshold, actual)												UNITY_TEST_ASSERT__SMALLER_THAN__HEX8(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__HEX16(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__HEX16(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__HEX32(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__HEX32(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__HEX64(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_THAN__HEX64(						(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_THAN__CHAR(threshold, actual)												UNITY_TEST_ASSERT__SMALLER_THAN__CHAR(						(threshold),		(actual),							__LINE__,	NULL)

#define TEST_ASSERT__GREATER_OR_EQUAL(threshold, actual)											UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__INT(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__INT8(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT8(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__INT16(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT16(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__INT32(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT32(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__INT64(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT64(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT8(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT8(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT16(threshold, actual)									UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT16(				(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT32(threshold, actual)									UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT32(				(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT64(threshold, actual)									UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT64(				(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__size_t(threshold, actual)									UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX8(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX8(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX16(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX16(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX32(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX32(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX64(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX64(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__GREATER_OR_EQUAL__CHAR(threshold, actual)										UNITY_TEST_ASSERT__GREATER_OR_EQUAL__CHAR(					(threshold),		(actual),							__LINE__,	NULL)

#define TEST_ASSERT__LESS_OR_EQUAL(threshold, actual)												UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__INT(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__INT8(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT8(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__INT16(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT16(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__INT32(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT32(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__INT64(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT64(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__UINT(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__UINT8(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT8(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__UINT16(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT16(				(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__UINT32(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT32(				(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__UINT64(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT64(				(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__size_t(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__HEX8(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX8(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__HEX16(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX16(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__HEX32(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX32(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__HEX64(threshold, actual)										UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX64(					(threshold),		(actual),							__LINE__,	NULL)
#define TEST_ASSERT__LESS_OR_EQUAL__CHAR(threshold, actual)											UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__CHAR(					(threshold),		(actual),							__LINE__,	NULL)

/* Integer Ranges (of all sizes) */
#define TEST_ASSERT__WITHIN__INT(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__INT(				(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__INT8(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__INT8(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__INT16(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__INT16(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__INT32(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__INT32(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__INT64(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__INT64(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__UINT(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__UINT(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__UINT8(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__UINT8(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__UINT16(delta, expected, actual)										UNITY_TEST_ASSERT__WITHIN__UINT16(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__UINT32(delta, expected, actual)										UNITY_TEST_ASSERT__WITHIN__UINT32(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__UINT64(delta, expected, actual)										UNITY_TEST_ASSERT__WITHIN__UINT64(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__size_t(delta, expected, actual)										UNITY_TEST_ASSERT__WITHIN__UINT(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__HEX(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__HEX32(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__HEX8(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__HEX8(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__HEX16(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__HEX16(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__HEX32(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__HEX32(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__HEX64(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__HEX64(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__WITHIN__CHAR(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__CHAR(			(delta),		(expected),			(actual),							__LINE__,	NULL)

/* Integer Array Ranges (of all sizes) */
#define TEST_ASSERT__WITHIN_ARRAY__INT(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__INT(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__INT8(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__INT8(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__INT16(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__INT16(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__INT32(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__INT32(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__INT64(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__INT64(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__UINT(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__UINT8(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT8(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__UINT16(delta, expected, actual, num_elements)					UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT16(	(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__UINT32(delta, expected, actual, num_elements)					UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT32(	(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__UINT64(delta, expected, actual, num_elements)					UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT64(	(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__size_t(delta, expected, actual, num_elements)					UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__HEX(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX32(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__HEX8(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX8(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__HEX16(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX16(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__HEX32(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX32(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__HEX64(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX64(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__WITHIN_ARRAY__CHAR(delta, expected, actual, num_elements)						UNITY_TEST_ASSERT__WITHIN_ARRAY__CHAR(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	NULL)


/* Structs and Strings */
#define TEST_ASSERT__EQUAL__PTR(expected, actual)													UNITY_TEST_ASSERT__EQUAL__PTR(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__STRING(expected, actual)												UNITY_TEST_ASSERT__EQUAL__STRING(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__STRING_LEN(expected, actual, len)										UNITY_TEST_ASSERT__EQUAL__STRING_LEN(						(expected),			(actual),	(len),					__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__MEMORY(expected, actual, len)											UNITY_TEST_ASSERT__EQUAL__MEMORY(							(expected),			(actual),	(len),					__LINE__,	NULL)

/* Arrays */
#define TEST_ASSERT__EQUAL_ARRAY__INT(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__INT(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__INT8(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__INT8(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__INT16(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__INT16(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__INT32(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__INT32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__INT64(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__INT64(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__UINT(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__UINT8(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT8(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__UINT16(expected, actual, num_elements)							UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT16(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__UINT32(expected, actual, num_elements)							UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__UINT64(expected, actual, num_elements)							UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT64(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__size_t(expected, actual, num_elements)							UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__HEX(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__HEX8(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX8(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__HEX16(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX16(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__HEX32(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__HEX64(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX64(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__PTR(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__PTR(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__STRING(expected, actual, num_elements)							UNITY_TEST_ASSERT__EQUAL_ARRAY__STRING(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__MEMORY(expected, actual, len, num_elements)						UNITY_TEST_ASSERT__EQUAL_ARRAY__MEMORY(						(expected),			(actual),	(len),	(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__CHAR(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__CHAR(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)

/* Arrays Compared To Single Value */
#define TEST_ASSERT__EQUAL_EACH__INT(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__INT(							(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__INT8(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__INT8(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__INT16(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__INT16(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__INT32(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__INT32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__INT64(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__INT64(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__UINT(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__UINT8(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__UINT8(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__UINT16(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__UINT16(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__UINT32(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__UINT32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__UINT64(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__UINT64(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__size_t(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__HEX(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__HEX8(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__HEX8(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__HEX16(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__HEX16(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__HEX32(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__HEX64(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__HEX64(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__PTR(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__PTR(							(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__STRING(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__STRING(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__MEMORY(expected, actual, len, num_elements)						UNITY_TEST_ASSERT__EQUAL_EACH__MEMORY(						(expected),			(actual),	(len),	(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__CHAR(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__CHAR(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)

/* Floating Point (If Enabled) */
#define TEST_ASSERT__WITHIN__FLOAT(delta, expected, actual)											UNITY_TEST_ASSERT__WITHIN__FLOAT(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__FLOAT(expected, actual)													UNITY_TEST_ASSERT__EQUAL__FLOAT(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__FLOAT(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_ARRAY__FLOAT(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__FLOAT(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__FLOAT(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_INF(actual)															UNITY_TEST_ASSERT__FLOAT__IS_INF(												(actual),							__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_NEG_INF(actual)														UNITY_TEST_ASSERT__FLOAT__IS_NEG_INF(											(actual),							__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_NAN(actual)															UNITY_TEST_ASSERT__FLOAT__IS_NAN(												(actual),							__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_DETERMINATE(actual)													UNITY_TEST_ASSERT__FLOAT__IS_DETERMINATE(										(actual),							__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_NOT_INF(actual)														UNITY_TEST_ASSERT__FLOAT__IS_NOT_INF(											(actual),							__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_NOT_NEG_INF(actual)													UNITY_TEST_ASSERT__FLOAT__IS_NOT_NEG_INF(										(actual),							__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_NOT_NAN(actual)														UNITY_TEST_ASSERT__FLOAT__IS_NOT_NAN(											(actual),							__LINE__,	NULL)
#define TEST_ASSERT__FLOAT__IS_NOT_DETERMINATE(actual)												UNITY_TEST_ASSERT__FLOAT__IS_NOT_DETERMINATE(									(actual),							__LINE__,	NULL)

/* Double (If Enabled) */
#define TEST_ASSERT__WITHIN__DOUBLE(delta, expected, actual)										UNITY_TEST_ASSERT__WITHIN__DOUBLE(			(delta),		(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL__DOUBLE(expected, actual)												UNITY_TEST_ASSERT__EQUAL__DOUBLE(							(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_ARRAY__DOUBLE(expected, actual, num_elements)							UNITY_TEST_ASSERT__EQUAL_ARRAY__DOUBLE(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__EQUAL_EACH__DOUBLE(expected, actual, num_elements)								UNITY_TEST_ASSERT__EQUAL_EACH__DOUBLE(						(expected),			(actual),			(num_elements),	__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_INF(actual)															UNITY_TEST_ASSERT__DOUBLE__IS_INF(												(actual),							__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_NEG_INF(actual)														UNITY_TEST_ASSERT__DOUBLE__IS_NEG_INF(											(actual),							__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_NAN(actual)															UNITY_TEST_ASSERT__DOUBLE__IS_NAN(												(actual),							__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_DETERMINATE(actual)													UNITY_TEST_ASSERT__DOUBLE__IS_DETERMINATE(										(actual),							__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_NOT_INF(actual)														UNITY_TEST_ASSERT__DOUBLE__IS_NOT_INF(											(actual),							__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_NOT_NEG_INF(actual)													UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NEG_INF(										(actual),							__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_NOT_NAN(actual)														UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NAN(											(actual),							__LINE__,	NULL)
#define TEST_ASSERT__DOUBLE__IS_NOT_DETERMINATE(actual)												UNITY_TEST_ASSERT__DOUBLE__IS_NOT_DETERMINATE(									(actual),							__LINE__,	NULL)

/* Shorthand */
#ifdef UNITY_SHORTHAND_AS_OLD
#define TEST_ASSERT__EQUAL(expected, actual)														UNITY_TEST_ASSERT__EQUAL__INT(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL(expected, actual)													UNITY_TEST_ASSERT(											((expected) !=		(actual)),							__LINE__,	" Expected Not-Equal")
#endif
#ifdef UNITY_SHORTHAND_AS_INT
#define TEST_ASSERT__EQUAL(expected, actual)														UNITY_TEST_ASSERT__EQUAL__INT(								(expected),			(actual),							__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL(expected, actual)													UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#endif
#ifdef UNITY_SHORTHAND_AS_MEM
#define TEST_ASSERT__EQUAL(expected, actual)														UNITY_TEST_ASSERT__EQUAL__MEMORY(							(&expected),		(&actual),	sizeof(expected),		__LINE__,	NULL)
#define TEST_ASSERT__NOT_EQUAL(expected, actual)													UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#endif
#ifdef UNITY_SHORTHAND_AS_RAW
#define TEST_ASSERT__EQUAL(expected, actual)														UNITY_TEST_ASSERT(											((expected) ==		(actual)),							__LINE__,	" Expected Equal")
#define TEST_ASSERT__NOT_EQUAL(expected, actual)													UNITY_TEST_ASSERT(											((expected) !=		(actual)),							__LINE__,	" Expected Not-Equal")
#endif
#ifdef UNITY_SHORTHAND_AS_NONE
#define TEST_ASSERT__EQUAL(expected, actual)														UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#define TEST_ASSERT__NOT_EQUAL(expected, actual)													UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#endif

/*-------------------------------------------------------
 * Test Asserts (with additional messages)
 *-------------------------------------------------------*/

/* Boolean */
#define TEST_ASSERT__MESSAGE(condition, message)													UNITY_TEST_ASSERT(																(condition),						__LINE__,	(message))
#define TEST_ASSERT__TRUE_MESSAGE(condition, message)												UNITY_TEST_ASSERT(																(condition),						__LINE__,	(message))
#define TEST_ASSERT__UNLESS_MESSAGE(condition, message)												UNITY_TEST_ASSERT(																!(condition),						__LINE__,	(message))
#define TEST_ASSERT__FALSE_MESSAGE(condition, message)												UNITY_TEST_ASSERT(																!(condition),						__LINE__,	(message))
#define TEST_ASSERT__NULL_MESSAGE(pointer, message)													UNITY_TEST_ASSERT__NULL(														(pointer),							__LINE__,	(message))
#define TEST_ASSERT__NOT_NULL_MESSAGE(pointer, message)												UNITY_TEST_ASSERT__NOT_NULL(													(pointer),							__LINE__,	(message))
#define TEST_ASSERT__EMPTY_MESSAGE(pointer, message)												UNITY_TEST_ASSERT__EMPTY(														(pointer),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EMPTY_MESSAGE(pointer, message)											UNITY_TEST_ASSERT__NOT_EMPTY(													(pointer),							__LINE__,	(message))

/* Integers (of all sizes) */
#define TEST_ASSERT__EQUAL__INT_MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT__EQUAL__INT(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__INT8_MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT__EQUAL__INT8(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__INT16_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__INT16(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__INT32_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__INT32(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__INT64_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__INT64(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__UINT_MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT__EQUAL__UINT(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__UINT8_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__UINT8(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__UINT16_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__UINT16(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__UINT32_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__UINT32(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__UINT64_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__UINT64(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__size_t_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__UINT(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__HEX_MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT__EQUAL__HEX32(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__HEX8_MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT__EQUAL__HEX8(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__HEX16_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__HEX16(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__HEX32_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__HEX32(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__HEX64_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__HEX64(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__BITS_MESSAGE(mask, expected, actual, message)							UNITY_TEST_ASSERT__EQUAL__BITS(	(mask),						(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__BITS_HIGH_MESSAGE(mask, actual, message)								UNITY_TEST_ASSERT__EQUAL__BITS(	(mask),						(UNITY_UINT32)(-1),	(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__BITS_LOW_MESSAGE(mask, actual, message)									UNITY_TEST_ASSERT__EQUAL__BITS(	(mask),						(UNITY_UINT32)(0),	(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__BIT_HIGH_MESSAGE(bit, actual, message)									UNITY_TEST_ASSERT__EQUAL__BITS(	((UNITY_UINT32)1 << (bit)),	(UNITY_UINT32)(-1),	(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__BIT_LOW_MESSAGE(bit, actual, message)									UNITY_TEST_ASSERT__EQUAL__BITS(	((UNITY_UINT32)1 << (bit)),	(UNITY_UINT32)(0),	(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__CHAR_MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT__EQUAL__CHAR(								(expected),			(actual),							__LINE__,	(message))

/* Integer Not Equal To (of all sizes) */
#define TEST_ASSERT__NOT_EQUAL__INT_MESSAGE(threshold, actual, message)								UNITY_TEST_ASSERT__NOT_EQUAL__INT(							(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__INT8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__INT8(							(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__INT16_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__INT16(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__INT32_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__INT32(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__INT64_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__INT64(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__UINT_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__UINT(							(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__UINT8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__UINT8(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__UINT16_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__UINT16(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__UINT32_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__UINT32(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__UINT64_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__UINT64(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__size_t_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__UINT(							(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__HEX8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__HEX8(							(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__HEX16_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__HEX16(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__HEX32_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__HEX32(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__HEX64_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__HEX64(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__CHAR_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__NOT_EQUAL__CHAR(							(threshold),		(actual),							__LINE__,	(message))


/* Integer Greater Than/ Less Than (of all sizes) */
#define TEST_ASSERT__GREATER_THAN__MESSAGE(threshold, actual, message)								UNITY_TEST_ASSERT__GREATER_THAN__INT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__INT_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__GREATER_THAN__INT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__INT8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__GREATER_THAN__INT8(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__INT16_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__INT16(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__INT32_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__INT32(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__INT64_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__INT64(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__UINT_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__GREATER_THAN__UINT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__UINT8_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__UINT8(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__UINT16_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__UINT16(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__UINT32_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__UINT32(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__UINT64_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__UINT64(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__size_t_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__UINT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__HEX8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__GREATER_THAN__HEX8(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__HEX16_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__HEX16(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__HEX32_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__HEX32(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__HEX64_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_THAN__HEX64(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_THAN__CHAR_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__GREATER_THAN__CHAR(						(threshold),		(actual),							__LINE__,	(message))

#define TEST_ASSERT__LESS_THAN__MESSAGE(threshold, actual, message)									UNITY_TEST_ASSERT__SMALLER_THAN__INT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__INT_MESSAGE(threshold, actual, message)								UNITY_TEST_ASSERT__SMALLER_THAN__INT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__INT8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__INT8(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__INT16_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__INT16(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__INT32_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__INT32(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__INT64_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__INT64(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__UINT_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__UINT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__UINT8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__UINT8(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__UINT16_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__UINT16(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__UINT32_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__UINT32(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__UINT64_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__UINT64(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__size_t_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__UINT(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__HEX8_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__HEX8(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__HEX16_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__HEX16(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__HEX32_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__HEX32(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__HEX64_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__HEX64(						(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_THAN__CHAR_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_THAN__CHAR(						(threshold),		(actual),							__LINE__,	(message))

#define TEST_ASSERT__GREATER_OR_EQUAL__MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__INT_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__INT8_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT8(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__INT16_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT16(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__INT32_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT32(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__INT64_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__INT64(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT8_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT8(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT16_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT16(				(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT32_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT32(				(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__UINT64_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT64(				(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__size_t_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX8_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX8(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX16_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX16(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX32_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX32(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__HEX64_MESSAGE(threshold, actual, message)					UNITY_TEST_ASSERT__GREATER_OR_EQUAL__HEX64(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__GREATER_OR_EQUAL__CHAR_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__GREATER_OR_EQUAL__CHAR(					(threshold),		(actual),							__LINE__,	(message))

#define TEST_ASSERT__LESS_OR_EQUAL__MESSAGE(threshold, actual, message)								UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__INT_MESSAGE(threshold, actual, message)							UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__INT8_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT8(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__INT16_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT16(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__INT32_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT32(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__INT64_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__INT64(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__UINT_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__UINT8_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT8(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__UINT16_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT16(				(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__UINT32_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT32(				(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__UINT64_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT64(				(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__size_t_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__UINT(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__HEX8_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX8(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__HEX16_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX16(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__HEX32_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX32(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__HEX64_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__HEX64(					(threshold),		(actual),							__LINE__,	(message))
#define TEST_ASSERT__LESS_OR_EQUAL__CHAR_MESSAGE(threshold, actual, message)						UNITY_TEST_ASSERT__SMALLER_OR_EQUAL__CHAR(					(threshold),		(actual),							__LINE__,	(message))

/* Integer Ranges (of all sizes) */
#define TEST_ASSERT__WITHIN__INT_MESSAGE(delta, expected, actual, message)							UNITY_TEST_ASSERT__WITHIN__INT(				(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__INT8_MESSAGE(delta, expected, actual, message)							UNITY_TEST_ASSERT__WITHIN__INT8(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__INT16_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__INT16(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__INT32_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__INT32(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__INT64_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__INT64(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__UINT_MESSAGE(delta, expected, actual, message)							UNITY_TEST_ASSERT__WITHIN__UINT(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__UINT8_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__UINT8(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__UINT16_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__UINT16(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__UINT32_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__UINT32(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__UINT64_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__UINT64(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__size_t_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__UINT(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__HEX_MESSAGE(delta, expected, actual, message)							UNITY_TEST_ASSERT__WITHIN__HEX32(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__HEX8_MESSAGE(delta, expected, actual, message)							UNITY_TEST_ASSERT__WITHIN__HEX8(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__HEX16_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__HEX16(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__HEX32_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__HEX32(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__HEX64_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__HEX64(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__WITHIN__CHAR_MESSAGE(delta, expected, actual, message)							UNITY_TEST_ASSERT__WITHIN__CHAR(			(delta),		(expected),			(actual),							__LINE__,	(message))

/* Integer Array Ranges (of all sizes) */
#define TEST_ASSERT__WITHIN_ARRAY__INT_MESSAGE(delta, expected, actual, num_elements, message)		UNITY_TEST_ASSERT__WITHIN_ARRAY__INT(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__INT8_MESSAGE(delta, expected, actual, num_elements, message)		UNITY_TEST_ASSERT__WITHIN_ARRAY__INT8(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__INT16_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__INT16(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__INT32_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__INT32(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__INT64_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__INT64(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__UINT_MESSAGE(delta, expected, actual, num_elements, message)		UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__UINT8_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT8(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__UINT16_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT16(	(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__UINT32_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT32(	(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__UINT64_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT64(	(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__size_t_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__UINT(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__HEX_MESSAGE(delta, expected, actual, num_elements, message)		UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX32(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__HEX8_MESSAGE(delta, expected, actual, num_elements, message)		UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX8(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__HEX16_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX16(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__HEX32_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX32(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__HEX64_MESSAGE(delta, expected, actual, num_elements, message)	UNITY_TEST_ASSERT__WITHIN_ARRAY__HEX64(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__WITHIN_ARRAY__CHAR_MESSAGE(delta, expected, actual, num_elements, message)		UNITY_TEST_ASSERT__WITHIN_ARRAY__CHAR(		(delta),		(expected),			(actual),			(num_elements),	__LINE__,	(message))


/* Structs and Strings */
#define TEST_ASSERT__EQUAL__PTR_MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT__EQUAL__PTR(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__STRING_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__STRING(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__STRING_LEN_MESSAGE(expected, actual, len, message)						UNITY_TEST_ASSERT__EQUAL__STRING_LEN(						(expected),			(actual),	(len),					__LINE__,	(message))
#define TEST_ASSERT__EQUAL__MEMORY_MESSAGE(expected, actual, len, message)							UNITY_TEST_ASSERT__EQUAL__MEMORY(							(expected),			(actual),	(len),					__LINE__,	(message))

/* Arrays */
#define TEST_ASSERT__EQUAL_ARRAY__INT_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_ARRAY__INT(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__INT8_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_ARRAY__INT8(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__INT16_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__INT16(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__INT32_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__INT32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__INT64_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__INT64(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__UINT_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__UINT8_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT8(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__UINT16_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT16(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__UINT32_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__UINT64_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT64(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__size_t_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__HEX_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__HEX8_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX8(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__HEX16_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX16(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__HEX32_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__HEX64_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__HEX64(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__PTR_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_ARRAY__PTR(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__STRING_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__STRING(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__MEMORY_MESSAGE(expected, actual, len, num_elements, message)		UNITY_TEST_ASSERT__EQUAL_ARRAY__MEMORY(						(expected),			(actual),	(len),	(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__CHAR_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_ARRAY__CHAR(						(expected),			(actual),			(num_elements),	__LINE__,	(message))

/* Arrays Compared To Single Value*/
#define TEST_ASSERT__EQUAL_EACH__INT_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__INT(							(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__INT8_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__INT8(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__INT16_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__INT16(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__INT32_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__INT32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__INT64_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__INT64(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__UINT_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__UINT8_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__UINT8(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__UINT16_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_EACH__UINT16(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__UINT32_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_EACH__UINT32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__UINT64_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_EACH__UINT64(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__size_t_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_EACH__UINT(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__HEX_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__HEX8_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__HEX8(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__HEX16_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__HEX16(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__HEX32_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__HEX32(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__HEX64_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__HEX64(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__PTR_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__PTR(							(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__STRING_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_EACH__STRING(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__MEMORY_MESSAGE(expected, actual, len, num_elements, message)		UNITY_TEST_ASSERT__EQUAL_EACH__MEMORY(						(expected),			(actual),	(len),	(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__CHAR_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__CHAR(						(expected),			(actual),			(num_elements),	__LINE__,	(message))

/* Floating Point (If Enabled) */
#define TEST_ASSERT__WITHIN__FLOAT_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__FLOAT(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__FLOAT_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__FLOAT(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__FLOAT_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__FLOAT(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__FLOAT_MESSAGE(expected, actual, num_elements, message)				UNITY_TEST_ASSERT__EQUAL_EACH__FLOAT(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_INF_MESSAGE(actual, message)											UNITY_TEST_ASSERT__FLOAT__IS_INF(												(actual),							__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_NEG_INF_MESSAGE(actual, message)										UNITY_TEST_ASSERT__FLOAT__IS_NEG_INF(											(actual),							__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_NAN_MESSAGE(actual, message)											UNITY_TEST_ASSERT__FLOAT__IS_NAN(												(actual),							__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_DETERMINATE_MESSAGE(actual, message)									UNITY_TEST_ASSERT__FLOAT__IS_DETERMINATE(										(actual),							__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_NOT_INF_MESSAGE(actual, message)										UNITY_TEST_ASSERT__FLOAT__IS_NOT_INF(											(actual),							__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_NOT_NEG_INF_MESSAGE(actual, message)									UNITY_TEST_ASSERT__FLOAT__IS_NOT_NEG_INF(										(actual),							__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_NOT_NAN_MESSAGE(actual, message)										UNITY_TEST_ASSERT__FLOAT__IS_NOT_NAN(											(actual),							__LINE__,	(message))
#define TEST_ASSERT__FLOAT__IS_NOT_DETERMINATE_MESSAGE(actual, message)								UNITY_TEST_ASSERT__FLOAT__IS_NOT_DETERMINATE(									(actual),							__LINE__,	(message))

/* Double (If Enabled) */
#define TEST_ASSERT__WITHIN__DOUBLE_MESSAGE(delta, expected, actual, message)						UNITY_TEST_ASSERT__WITHIN__DOUBLE(			(delta),		(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL__DOUBLE_MESSAGE(expected, actual, message)								UNITY_TEST_ASSERT__EQUAL__DOUBLE(							(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__EQUAL_ARRAY__DOUBLE_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_ARRAY__DOUBLE(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__EQUAL_EACH__DOUBLE_MESSAGE(expected, actual, num_elements, message)			UNITY_TEST_ASSERT__EQUAL_EACH__DOUBLE(						(expected),			(actual),			(num_elements),	__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_INF_MESSAGE(actual, message)										UNITY_TEST_ASSERT__DOUBLE__IS_INF(												(actual),							__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_NEG_INF_MESSAGE(actual, message)									UNITY_TEST_ASSERT__DOUBLE__IS_NEG_INF(											(actual),							__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_NAN_MESSAGE(actual, message)										UNITY_TEST_ASSERT__DOUBLE__IS_NAN(												(actual),							__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_DETERMINATE_MESSAGE(actual, message)								UNITY_TEST_ASSERT__DOUBLE__IS_DETERMINATE(										(actual),							__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_NOT_INF_MESSAGE(actual, message)									UNITY_TEST_ASSERT__DOUBLE__IS_NOT_INF(											(actual),							__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_NOT_NEG_INF_MESSAGE(actual, message)								UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NEG_INF(										(actual),							__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_NOT_NAN_MESSAGE(actual, message)									UNITY_TEST_ASSERT__DOUBLE__IS_NOT_NAN(											(actual),							__LINE__,	(message))
#define TEST_ASSERT__DOUBLE__IS_NOT_DETERMINATE_MESSAGE(actual, message)							UNITY_TEST_ASSERT__DOUBLE__IS_NOT_DETERMINATE(									(actual),							__LINE__,	(message))

/* Shorthand */
#ifdef UNITY_SHORTHAND_AS_OLD
#define TEST_ASSERT__EQUAL__MESSAGE(expected, actual, message)										UNITY_TEST_ASSERT__EQUAL__INT(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT(											((expected) !=		(actual)),							__LINE__,	(message))
#endif
#ifdef UNITY_SHORTHAND_AS_INT
#define TEST_ASSERT__EQUAL__MESSAGE(expected, actual, message)										UNITY_TEST_ASSERT__EQUAL__INT(								(expected),			(actual),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__MESSAGE(expected, actual, message)									UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#endif
#ifdef  UNITY_SHORTHAND_AS_MEM
#define TEST_ASSERT__EQUAL__MESSAGE(expected, actual, message)										UNITY_TEST_ASSERT__EQUAL__MEMORY(							(&expected),		(&actual),	sizeof(expected),		__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__MESSAGE(expected, actual, message)									UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#endif
#ifdef  UNITY_SHORTHAND_AS_RAW
#define TEST_ASSERT__EQUAL__MESSAGE(expected, actual, message)										UNITY_TEST_ASSERT(											((expected) ==		(actual)),							__LINE__,	(message))
#define TEST_ASSERT__NOT_EQUAL__MESSAGE(expected, actual, message)									UNITY_TEST_ASSERT(											((expected) !=		(actual)),							__LINE__,	(message))
#endif
#ifdef UNITY_SHORTHAND_AS_NONE
#define TEST_ASSERT__EQUAL__MESSAGE(expected, actual, message)										UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#define TEST_ASSERT__NOT_EQUAL__MESSAGE(expected, actual, message)									UNITY_TEST_FAIL(																									__LINE__,	UnityStrErrShorthand)
#endif

/* end of UNITY_FRAMEWORK_H */
#ifdef __cplusplus
}
#endif
#endif
