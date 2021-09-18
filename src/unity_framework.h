/* ==========================================
	Unity Project - A Test Framework for C
	Copyright (c) 2007-21 Mike Karlesky, Mark VanderVoord, Greg Williams, David Koch
	[Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_FRAMEWORK_H
#define UNITY_FRAMEWORK_H

/* ****************************** TEST FRAMEWORK *******************************
 * Testing is performed during campaigns that are basically organized to mirror
 * software specifications and requirements. In the V-model development cycle,
 * tests are written after specifications and software architecture, but before
 * actual coding. Hence this enables test-driven development which allows the
 * coder to ensure the conformance of the source code against the previously
 * written tests in a continuous fashion.
 *
 * The typical organization of such tests is mostly the following one:
 *  1. Test framework : test engine (Unity)
 *  2. Test suite     : specification to test
 *  3. Test plan      : functionality to test
 *  4. Test fixture   : hardware or class to test
 *  5. Test case      : pin or method to test
 *  6. Test step      : level or value to test
 *  7. Test assert    : test collection (Unity)
 *
 * Some levels are `virtual` in the sense that those are tests organized into
 * different folders to separate functionalities, but some are actual written
 * tests called from a specific `main()` to perform the software verification.
 *
 * Unity is a collection of asserts to test the software, but also a collection
 * of macros to organize the tests into plans, fixtures, cases and steps.
 *
 * Cases, fixtures and plans have to be declared into the test header file, in
 * that very specific order to create linked tables able to sequence the tests
 * execution. Typically that looks like this (no needs to add semicolons):
 *
 *  TEST_CASE__DECL(ble_profile)
 *  TEST_CASE__DECL(connect)
 *
 *  TEST_FIXT__DECL(ble)
 *  TEST_FIXT__DECL(wifi)
 *
 *  TEST_PLAN__DECL(com)
 *
 * Then, in the test source file, the cases are added to fixtures, which are
 * added to plans, which are executed. During the execution there is an output
 * of the test sequencing interlaced with test asserts. At the end of a plan,
 * a light report is issued counting how many tests succeeded and/or failed,
 * and the status of the plan itself, either success or failure. Typically that
 * looks like this (no needs to add semicolons):
 *
 * #define UNITY_OUTPUT_CHAR(a)	sendchar(a)
 * #define UNITY_INCLUDE_CONFIG_H
 * #include "unity.h"
 *
 * static UNITY_UINT32 counter;
 *
 * // Cases
 *
 * TEST_CASE__START(ble_profile)
 * 		TEST_ASSERT__EQUAL__UINT32(0x34, counter)
 * TEST_CASE__END(ble_profile)
 *
 * TEST_CASE__START(connect)
 * 		TEST_STEP__START()
 * 			counter = 3;
 * 		TEST_STEP__END__REBOOT_AFTER(100)
 *
 * 		TEST_STEP__START()
 * 			TEST_ASSERT__EQUAL__UINT32_MESSAGE(3, counter, "Problem")
 * 		TEST_STEP__END()
 * TEST_CASE__END(connect)
 *
 * TEST_CASE__START(is)
 * 		int check_char = isalpha('A');
 *
 * 		TEST_ASSERT__TRUE(check_char);
 * 		TEST_ASSERT__FALSE(check_char);
 * TEST_CASE__END(is)
 *
 * // Fixtures
 *
 * TEST_FIXT__SETUP_START(ble)
 * 		counter = 2;
 * TEST_FIXT__SETUP_END(ble)
 *
 * TEST_FIXT__TEARDOWN_START(ble)
 * 		counter = 0;
 * TEST_FIXT__TEARDOWN_END(ble)
 *
 * TEST_FIXT__START(ble)
 * 		TEST_FIXT__CASE_ADD(ble_profile)
 * TEST_FIXT__END(ble)
 *
 * TEST_FIXT__START(wifi)
 * 		TEST_FIXT__CASE_ADD(connect)
 * TEST_FIXT__END(wifi)
 *
 * TEST_FIXT__START(chr)
 * 		TEST_FIXT__CASE_ADD(is)
 * TEST_FIXT__END(chr)
 *
 * // Plans
 *
 * TEST_PLAN__START(com)
 * 		TEST_PLAN__FIXT_ADD(ble)
 * 		TEST_PLAN__FIXT_ADD(wifi)
 * TEST_PLAN__END(com)
 *
 * TEST_PLAN__START(utl)
 * 		TEST_PLAN__FIXT_ADD(chr)
 * TEST_PLAN__END(utl)
 *
 * // main
 *
 * void main(void)
 * {
 * 		TEST_PLAN__RUN(com)
 * 		TEST_PLAN__RUN(utl)
 * }
 **************************************************************************** */

#define TEST_CONTEXT__NONE
#define TEST_CONTEXT__RAM_NO_INIT	__attribute__((section("User_NoInit"))) __attribute__((zero_init))

#define TEST__FUNC_PTR(func)												\
			UnityTestFunction	( func )

#define TEST__SETUP()														\
			{	Unity.TestFile				= __FILE__;						\
				Unity.CurrentTestLineNumber	= __LINE__;						\
			}

#define TEST__START()														\
			UnityBegin(__FILE__);

#define TEST__PRINT_EOL()													\
			UNITY_OUTPUT_CHAR('\r');										\
			UNITY_OUTPUT_CHAR('\n');

#define TEST__PRINT(txt)													\
			UnityPrint(txt);												\
			TEST__PRINT_EOL();

#define TEST__RUN(func, name, line)											\
			UnityDefaultTestRun( func, name, line );

#define TEST__END()															\
			TEST__PRINT_EOL();												\
			UnityEnd();

#define TEST__GET_FAIL()													\
			Unity.TestFailures

/*-------------------------------------------------------
 * Test Step
 *-------------------------------------------------------*/

int UnityStepStart(void);
void UnityStepEnd(UNITY_UINT32 time_ms);

#define TEST_STEP__START()													\
			if (0 != UnityStepStart())										\
			{

#define TEST_STEP__END()													\
			TEST_STEP__END__REBOOT_AFTER(0)
#define TEST_STEP__END__REBOOT_NOW()										\
			TEST_STEP__END__REBOOT_AFTER(1)
#define TEST_STEP__END__REBOOT_AFTER(ms)									\
				UnityStepEnd(ms);											\
			}	UnityStepEnd(-1);

/*-------------------------------------------------------
 * Test Case
 *-------------------------------------------------------*/

typedef enum
{
	UNITY_CASE_STATE__READY	= 0,
	UNITY_CASE_STATE__WAITING,
	UNITY_CASE_STATE__BLOCKED,
	UNITY_CASE_STATE__FAILED,
	UNITY_CASE_STATE__PASSED,

	UNITY_CASE_STATE
} UNITY_CASE_STATE_T;

typedef struct
{
	UNITY_CASE_STATE_T	State;
	UNITY_COUNTER_TYPE	CurrentStep;
} UNITY_CASE_VARS_T;

typedef struct
{
	UnityTestFunction	funcCase;
	const char*			Name;
	UNITY_CASE_VARS_T*	Vars;
	UNITY_LINE_TYPE		LineNumber;
} UNITY_CASE_T;

int UnityCaseStart(void);
void UnityCaseCheck(int check);
void UnityCaseExitOnFail(int check);
int UnityCaseExitOn(int check);
int UnityCaseRebootCheck(void);
void UnityCaseEnd(void);

#define TEST_CASE__NAME(cause)	UnityCase__ ## cause
#define TEST_CASE__VARS(cause)	UnityCase__ ## cause ## __Vars
#define TEST_CASE__FUNC(cause)	void TEST_CASE__NAME(cause)	(void)
#define TEST_CASE__DECL(cause)	TEST_CASE__FUNC(cause);

#define TEST_CASE__START(cause)												\
			TEST_CASE__START_CNTX(cause, TEST_CONTEXT__NONE)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define TEST_CASE__START_INIT(init)											\
				{ .State			= init									\
				, .CurrentStep		= 0										\
				}
#else	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_CASE__START_INIT(init)											\
				{					  init									\
				,					  0										\
				}
#endif	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_CASE__START_CNTX(cause, cntx)									\
			UNITY_CASE_VARS_T TEST_CASE__VARS(cause) cntx					\
			= TEST_CASE__START_INIT(UNITY_CASE_STATE__READY);				\
			TEST_CASE__FUNC(cause)											\
			{	if (0 != UnityCaseStart())									\
				{	TEST__SETUP()

#define TEST_CASE__CHECK(check)												\
					UnityCaseCheck(check);

#define TEST_CASE__EXIT_ON_FAIL(check)										\
					UnityCaseExitOnFail(check);

#define TEST_CASE__EXIT_ON(check)											\
					if (0 != UnityCaseExitOn(check)) { return; }

#define TEST_CASE__ASSERT(check)											\
					{	int _result = (check);								\
						TEST_ASSERT(_result)								\
						TEST_CASE__CHECK(_result)							\
						TEST_CASE__EXIT_ON(_result)							\
					}

#define TEST_CASE__VAR(type, var)											\
					static type var;

#define TEST_CASE__REBOOT_CHECK()											\
					UnityCaseRebootCheck()

#define TEST_CASE__END(cause)												\
				}															\
				else														\
				{															\
					TEST_ASSERT__TRUE_MESSAGE(0, "No fixture set !");		\
				}															\
				UnityCaseEnd();												\
			}

/*-------------------------------------------------------
 * Test Fixture
 *-------------------------------------------------------*/

typedef struct
{
	UnityTestFunction	funcCall;
	int					ExitOnFail;
	UNITY_COUNTER_TYPE	Fails;
	UNITY_COUNTER_TYPE	CurrentCase;
	UNITY_COUNTER_TYPE	CurrentStep;
} UNITY_FIXT_VARS_T;

typedef struct
{
	UnityTestFunction	setUp;
	UnityTestFunction	tearDown;
	const char*			Name;
	UNITY_FIXT_VARS_T*	Vars;
	const UNITY_CASE_T*	Cases;
	UNITY_COUNTER_TYPE	CaseMax;
	UNITY_LINE_TYPE		LineNumber;
} UNITY_FIXT_T;

void funcCall(void);					/* global call */

void UnityFixtExe(const UNITY_FIXT_T* fixt);

#define TEST_FIXT__NAME(fixt)	UnityFixture__ ## fixt
#define TEST_FIXT__VARS(fixt)	UnityFixture__ ## fixt ## __Vars
#define TEST_FIXT__CASE(fixt)	UnityFixture__ ## fixt ## __Cases
#define TEST_FIXT__DECL(fixt)	extern const UNITY_FIXT_T TEST_FIXT__NAME(fixt);

/* Setup */

#define TEST_FIXT__SETUP_NAME(fixt)											\
			UnityFixture__ ## fixt ## __Setup

#define TEST_FIXT__SETUP_FUNC(fixt)											\
			void TEST_FIXT__SETUP_NAME(fixt)	(void)

#define TEST_FIXT__SETUP_DECL(fixt)											\
			TEST_FIXT__SETUP_FUNC(fixt);

#define TEST_FIXT__SETUP_START(fixt)										\
			TEST_FIXT__SETUP_FUNC(fixt)										\
			{

#define TEST_FIXT__SETUP_END(fixt)											\
			}

/* Teardown */

#define TEST_FIXT__TEARDOWN_NAME(fixt)										\
			UnityFixture__ ## fixt ## __Teardown

#define TEST_FIXT__TEARDOWN_FUNC(fixt)										\
			void TEST_FIXT__TEARDOWN_NAME(fixt)	(void)

#define TEST_FIXT__TEARDOWN_DECL(fixt)										\
			TEST_FIXT__TEARDOWN_FUNC(fixt);

#define TEST_FIXT__TEARDOWN_START(fixt)										\
			TEST_FIXT__TEARDOWN_FUNC(fixt)									\
			{

#define TEST_FIXT__TEARDOWN_END(fixt)										\
			}

#define TEST_FIXT__START(fixt)												\
			TEST_FIXT__START_CNTX(fixt, TEST_CONTEXT__NONE)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define TEST_FIXT__START_INIT(init)											\
				{ .funcCall			= init									\
				, .Fails			= 0										\
				, .ExitOnFail		= 0										\
				, .CurrentCase		= (UNITY_COUNTER_TYPE) (-1)				\
				, .CurrentStep		= 0										\
				}
#else	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_FIXT__START_INIT(init)											\
				{					  init									\
				,					  0										\
				,					  0										\
				,					  (UNITY_COUNTER_TYPE) (-1)				\
				,					  0										\
				}
#endif	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_FIXT__START_CNTX(fixt, cntx)									\
			UNITY_FIXT_VARS_T TEST_FIXT__VARS(fixt) cntx					\
			= TEST_FIXT__START_INIT(NULL);									\
			const UNITY_CASE_T TEST_FIXT__CASE(fixt)[]						\
			=	{

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define TEST_FIXT__CASE_ADD(cause)											\
					{ .funcCase		= TEST_CASE__NAME(cause)				\
					, .Name			= #cause								\
					, .Vars			= &TEST_CASE__VARS(cause)				\
					, .LineNumber	= __LINE__								\
					},
#else	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_FIXT__CASE_ADD(cause)											\
					{				  TEST_CASE__NAME(cause)				\
					,				  #cause								\
					,				  &TEST_CASE__VARS(cause)				\
					,				  __LINE__								\
					},
#endif	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define TEST_FIXT__END_INIT(fixt)											\
				{ .setUp			= TEST_FIXT__SETUP_NAME(fixt)			\
				, .tearDown			= TEST_FIXT__TEARDOWN_NAME(fixt)		\
				, .Name				= #fixt									\
				, .Vars				= &TEST_FIXT__VARS(fixt)				\
				, .Cases			= TEST_FIXT__CASE(fixt)					\
				, .CaseMax			= (sizeof(TEST_FIXT__CASE(fixt)) /		\
										sizeof(TEST_FIXT__CASE(fixt)[0]))	\
				, .LineNumber		= __LINE__								\
				}
#else	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_FIXT__END_INIT(fixt)											\
				{					  TEST_FIXT__SETUP_NAME(fixt)			\
				,					  TEST_FIXT__TEARDOWN_NAME(fixt)		\
				,					  #fixt									\
				,					  &TEST_FIXT__VARS(fixt)				\
				,					  TEST_FIXT__CASE(fixt)					\
				,					  (sizeof(TEST_FIXT__CASE(fixt)) /		\
										sizeof(TEST_FIXT__CASE(fixt)[0]))	\
				,					  __LINE__								\
				}
#endif	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_FIXT__END(fixt)												\
				};															\
			UNITY_WEAK_ATTRIBUTE	TEST_FIXT__SETUP_FUNC(fixt)		{ ; }	\
			UNITY_WEAK_ATTRIBUTE	TEST_FIXT__TEARDOWN_FUNC(fixt)	{ ; }	\
			const UNITY_FIXT_T TEST_FIXT__NAME(fixt)						\
			= TEST_FIXT__END_INIT(fixt);

#define	TEST_FIXT__EXE(fixt)	UnityFixtExe( &TEST_FIXT__NAME(fixt) );
#define TEST_FIXT__RUN(fixt)	{ TEST_FIXT__EXE(fixt); }

/*-------------------------------------------------------
 * Test Plan
 *-------------------------------------------------------*/

typedef struct
{
	UNITY_COUNTER_TYPE		CurrentFixture;
} UNITY_PLAN_VARS_T;

typedef struct
{
	const char*				Name;
	UNITY_PLAN_VARS_T*		Vars;
	const UNITY_FIXT_T**	Fixtures;
	UNITY_COUNTER_TYPE		FixtureMax;
	UNITY_LINE_TYPE			LineNumber;
} UNITY_PLAN_T;

void UnityPlanExe(const UNITY_PLAN_T* plan);

#define TEST_PLAN__NAME(plan)	UnityPlan__ ## plan
#define TEST_PLAN__VARS(plan)	UnityPlan__ ## plan ## __Vars
#define TEST_PLAN__FIXT(plan)	UnityPlan__ ## plan ## __Fixtures
#define TEST_PLAN__DECL(plan)	extern const UNITY_PLAN_T TEST_PLAN__NAME(plan);

#define TEST_PLAN__START(plan)												\
			TEST_PLAN__START_CNTX(plan, TEST_CONTEXT__NONE)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define TEST_PLAN__START_INIT(init)											\
				{ .CurrentFixture	= (UNITY_COUNTER_TYPE) init				\
				}
#else	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_PLAN__START_INIT(init)											\
				{					  (UNITY_COUNTER_TYPE) init				\
				}
#endif	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_PLAN__START_CNTX(plan, cntx)									\
			UNITY_PLAN_VARS_T TEST_PLAN__VARS(plan)	cntx					\
			= TEST_PLAN__START_INIT(-1);									\
			const UNITY_FIXT_T* TEST_PLAN__FIXT(plan)[]						\
			=	{

#define TEST_PLAN__FIXT_ADD(fixt)											\
					&TEST_FIXT__NAME(fixt),

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define TEST_PLAN__END_INIT(plan)											\
				{ .Name				= #plan									\
				, .Vars				= &TEST_PLAN__VARS(plan)				\
				, .Fixtures			= &TEST_PLAN__FIXT(plan)[0]				\
				, .FixtureMax		= (sizeof(TEST_PLAN__FIXT(plan)) /		\
										sizeof(TEST_PLAN__FIXT(plan)[0]))	\
				, .LineNumber		= __LINE__								\
				}
#else	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_PLAN__END_INIT(plan)											\
				{					  #plan									\
				,					  &TEST_PLAN__VARS(plan)				\
				,					  &TEST_PLAN__FIXT(plan)[0]				\
				,					  (sizeof(TEST_PLAN__FIXT(plan)) /		\
										sizeof(TEST_PLAN__FIXT(plan)[0]))	\
				,					  __LINE__								\
				}
#endif	/* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */
#define TEST_PLAN__END(plan)												\
				}; 															\
			const UNITY_PLAN_T TEST_PLAN__NAME(plan)						\
			= TEST_PLAN__END_INIT(plan);

#define	TEST_PLAN__EXE(plan)	UnityPlanExe( &TEST_PLAN__NAME(plan) );
#define TEST_PLAN__RUN(plan)	{ TEST_PLAN__EXE(plan); }

#endif	/* UNITY_FRAMEWORK_H */
