/* Copyright (c) 2010 James Grenning and Contributed to Unity Project
 * ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

#include "unity_fixture.h"
#include "unity_internals.h"
#include <string.h>

struct UNITY_FIXTURE_T UnityFixture;

/* If you decide to use the function pointer approach.
 * Build with -D UNITY_OUTPUT_CHAR=outputChar and include <stdio.h>
 * int (*outputChar)(int) = putchar; */

void setUp(void)    { /*does nothing*/ }
void tearDown(void) { /*does nothing*/ }

static void announceTestRun(unsigned int runNumber)
{
    UnityPrint("Unity test run ");
    UnityPrintNumberUnsigned(runNumber+1);
    UnityPrint(" of ");
    UnityPrintNumberUnsigned(UnityFixture.RepeatCount);
    UNITY_PRINT_EOL();
}

int UnityMain(int argc, const char* argv[], void (*runAllTests)(void))
{
    int result = UnityGetCommandLineOptions(argc, argv);
    unsigned int r;
    if (result != 0)
        return result;

    for (r = 0; r < UnityFixture.RepeatCount; r++)
    {
        UnityBegin(argv[0]);
        announceTestRun(r);
        runAllTests();
        if (!UnityFixture.Verbose) UNITY_PRINT_EOL();
        UnityEnd();
    }

    return (int)Unity.TestFailures;
}

static int selected(const char* filter, const char* name)
{
    if (filter == 0)
        return 1;
    return strstr(name, filter) ? 1 : 0;
}

static int testSelected(const char* test)
{
    return selected(UnityFixture.NameFilter, test);
}

static int groupSelected(const char* group)
{
    return selected(UnityFixture.GroupFilter, group);
}

void UnityTestRunner(unityfunction* setup,
                     unityfunction* testBody,
                     unityfunction* teardown,
                     const char* printableName,
                     const char* group,
                     const char* name,
                     const char* file,
                     unsigned int line)
{
    if (testSelected(name) && groupSelected(group))
    {
        Unity.TestFile = file;
        Unity.CurrentTestName = printableName;
        Unity.CurrentTestLineNumber = line;
        if (UnityFixture.Verbose)
        {
            UnityPrint(printableName);
        #ifndef UNITY_REPEAT_TEST_NAME
            Unity.CurrentTestName = NULL;
        #endif
        }
        else if (UnityFixture.Silent)
        {
            /* Do Nothing */
        }
        else
        {
            UNITY_OUTPUT_CHAR('.');
        }

        Unity.NumberOfTests++;
        UnityPointer_Init();

        UNITY_EXEC_TIME_START();

        if (TEST_PROTECT())
        {
            setup();
            testBody();
        }
        if (TEST_PROTECT())
        {
            teardown();
        }
        if (TEST_PROTECT())
        {
            UnityPointer_UndoAllSets();
        }
        UnityConcludeFixtureTest();
    }
}

void UnityIgnoreTest(const char* printableName, const char* group, const char* name)
{
    if (testSelected(name) && groupSelected(group))
    {
        Unity.NumberOfTests++;
        Unity.TestIgnores++;
        if (UnityFixture.Verbose)
        {
            UnityPrint(printableName);
            UNITY_PRINT_EOL();
        }
        else if (UnityFixture.Silent)
        {
            /* Do Nothing */
        }
        else
        {
            UNITY_OUTPUT_CHAR('!');
        }
    }
}

/*-------------------------------------------------------- */
/*Automatic pointer restoration functions */
struct PointerPair
{
    void** pointer;
    void* old_value;
};

static struct PointerPair pointer_store[UNITY_MAX_POINTERS];
static int pointer_index = 0;

void UnityPointer_Init(void)
{
    pointer_index = 0;
}

void UnityPointer_Set(void** pointer, void* newValue, UNITY_LINE_TYPE line)
{
    if (pointer_index >= UNITY_MAX_POINTERS)
    {
        UNITY_TEST_FAIL(line, "Too many pointers set");
    }
    else
    {
        pointer_store[pointer_index].pointer = pointer;
        pointer_store[pointer_index].old_value = *pointer;
        *pointer = newValue;
        pointer_index++;
    }
}

void UnityPointer_UndoAllSets(void)
{
    while (pointer_index > 0)
    {
        pointer_index--;
        *(pointer_store[pointer_index].pointer) =
            pointer_store[pointer_index].old_value;
    }
}

int UnityGetCommandLineOptions(int argc, const char* argv[])
{
    int i;
    UnityFixture.Verbose = 0;
    UnityFixture.Silent = 0;
    UnityFixture.GroupFilter = 0;
    UnityFixture.NameFilter = 0;
    UnityFixture.RepeatCount = 1;

    if (argc == 1)
        return 0;

    for (i = 1; i < argc; )
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            /* Usage */
            UnityPrint("Runs a series of unit tests.");
            UNITY_PRINT_EOL();
            UNITY_PRINT_EOL();
            UnityPrint("When no flag is specified, all tests are run.");
            UNITY_PRINT_EOL();
            UNITY_PRINT_EOL();
            UnityPrint("Optional flags:");
            UNITY_PRINT_EOL();
            UnityPrint("  -v          Verbose output: show all tests executed even if they pass");
            UNITY_PRINT_EOL();
            UnityPrint("  -s          Silent mode: minimal output showing only test failures");
            UNITY_PRINT_EOL();
            UnityPrint("  -g NAME     Only run tests in groups that contain the string NAME");
            UNITY_PRINT_EOL();
            UnityPrint("  -n NAME     Only run tests whose name contains the string NAME");
            UNITY_PRINT_EOL();
            UnityPrint("  -r NUMBER   Repeatedly run all tests NUMBER times");
            UNITY_PRINT_EOL();
            UnityPrint("  -h, --help  Display this help message");
            UNITY_PRINT_EOL();
            UNITY_PRINT_EOL();
#ifdef UNITY_CUSTOM_HELP_MSG
            /* User-defined help message, e.g. to point to project-specific documentation */
            UnityPrint(UNITY_CUSTOM_HELP_MSG);
            UNITY_PRINT_EOL();
#else
            /* Default help suffix if a custom one is not defined */
            UnityPrint("More information about Unity: https://www.throwtheswitch.org/unity");
            UNITY_PRINT_EOL();
#endif
            return 1;  /* Exit without running the tests */
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            UnityFixture.Verbose = 1;
            i++;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            UnityFixture.Silent = 1;
            i++;
        }
        else if (strcmp(argv[i], "-g") == 0)
        {
            i++;
            if (i >= argc)
                return 1;
            UnityFixture.GroupFilter = argv[i];
            i++;
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            i++;
            if (i >= argc)
                return 1;
            UnityFixture.NameFilter = argv[i];
            i++;
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            UnityFixture.RepeatCount = 2;
            i++;
            if (i < argc)
            {
                if (*(argv[i]) >= '0' && *(argv[i]) <= '9')
                {
                    unsigned int digit = 0;
                    UnityFixture.RepeatCount = 0;
                    while (argv[i][digit] >= '0' && argv[i][digit] <= '9')
                    {
                        UnityFixture.RepeatCount *= 10;
                        UnityFixture.RepeatCount += (unsigned int)argv[i][digit++] - '0';
                    }
                    i++;
                }
            }
        }
        else
        {
            /* ignore unknown parameter */
            i++;
        }
    }
    return 0;
}

void UnityConcludeFixtureTest(void)
{
    if (Unity.CurrentTestIgnored)
    {
        Unity.TestIgnores++;
        UNITY_PRINT_EOL();
    }
    else if (!Unity.CurrentTestFailed)
    {
        if (UnityFixture.Verbose)
        {
            UnityPrint(" ");
            UnityPrint(UnityStrPass);
            UNITY_EXEC_TIME_STOP();
            UNITY_PRINT_EXEC_TIME();
            UNITY_PRINT_EOL();
        }
    }
    else /* Unity.CurrentTestFailed */
    {
        Unity.TestFailures++;
        UNITY_PRINT_EOL();
    }

    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
}
