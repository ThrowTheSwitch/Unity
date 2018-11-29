/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007-14 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_RUNNER_COMMON_H
#define UNITY_RUNNER_COMMON_H

/* This file defines the non-generated portion of each test runner. */

#include "unity.h"

#ifdef __WIN32__
#include "unity_setup_stubs.h"
#endif

#ifdef UNITY_RUNNER_USE_CEXCEPTION
#include "CException.h"
#endif

/*-----------------------------------------------*/
void UnityRunner_RunTest(UnityTestFunction func, const char* name, int line_num)
{
    Unity.CurrentTestName = name;
    Unity.CurrentTestLineNumber = line_num;

#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches())
        return;
#endif

    Unity.NumberOfTests++;
    UNITY_CLR_DETAILS();
    UNITY_EXEC_TIME_START();
    UnityRunner_Init();

    if (TEST_PROTECT())
    {
#ifdef UNITY_RUNNER_USE_CEXCEPTION
        CEXCEPTION_T e;
        Try {
#endif

        UnityRunner_SetUp();
        func();

#ifdef UNITY_RUNNER_USE_CEXCEPTION
        } Catch(e) {
            TEST_ASSERT_EQUAL_HEX32_MESSAGE(CEXCEPTION_NONE, e, "Unhandled Exception!");
        }
#endif
    }

    if (TEST_PROTECT())
    {
        UnityRunner_TearDown();
        UnityRunner_Verify();
    }

    UnityRunner_Destroy();
    UNITY_EXEC_TIME_STOP();
    UnityConcludeTest();
}

#endif
