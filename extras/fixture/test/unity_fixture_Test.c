/* Copyright (c) 2010 James Grenning and Contributed to Unity Project
 * ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

#include "unity_fixture.h"
#include <stdlib.h>
#include <string.h>

TEST_GROUP(UnityFixture);

TEST_SETUP(UnityFixture)
{
}

TEST_TEAR_DOWN(UnityFixture)
{
}

static int* pointer1 = 0;
static int* pointer2 = (int*)2;
static int* pointer3 = (int*)3;
static int int1;
static int int2;
static int int3;
static int int4;

TEST(UnityFixture, PointerSetting)
{
    TEST_ASSERT_POINTERS_EQUAL(pointer1, 0);
    UT_PTR_SET(pointer1, &int1);
    UT_PTR_SET(pointer2, &int2);
    UT_PTR_SET(pointer3, &int3);
    TEST_ASSERT_POINTERS_EQUAL(pointer1, &int1);
    TEST_ASSERT_POINTERS_EQUAL(pointer2, &int2);
    TEST_ASSERT_POINTERS_EQUAL(pointer3, &int3);
    UT_PTR_SET(pointer1, &int4);
    UnityPointer_UndoAllSets();
    TEST_ASSERT_POINTERS_EQUAL(pointer1, 0);
    TEST_ASSERT_POINTERS_EQUAL(pointer2, (int*)2);
    TEST_ASSERT_POINTERS_EQUAL(pointer3, (int*)3);
}

static char *p1;
static char *p2;

TEST(UnityFixture, PointerSet)
{
    char c1;
    char c2;
    char newC1;
    char newC2;
    p1 = &c1;
    p2 = &c2;

    UnityPointer_Init();
    UT_PTR_SET(p1, &newC1);
    UT_PTR_SET(p2, &newC2);
    TEST_ASSERT_POINTERS_EQUAL(&newC1, p1);
    TEST_ASSERT_POINTERS_EQUAL(&newC2, p2);
    UnityPointer_UndoAllSets();
    TEST_ASSERT_POINTERS_EQUAL(&c1, p1);
    TEST_ASSERT_POINTERS_EQUAL(&c2, p2);
}

TEST(UnityFixture, FreeNULLSafety)
{
    free(NULL);
}

TEST(UnityFixture, ConcludeTestIncrementsFailCount)
{
    UNITY_UINT savedFails = Unity.TestFailures;
    UNITY_UINT savedIgnores = Unity.TestIgnores;
    Unity.CurrentTestFailed = 1;
    UnityConcludeFixtureTest(); /* Resets TestFailed for this test to pass */
    Unity.CurrentTestIgnored = 1;
    UnityConcludeFixtureTest(); /* Resets TestIgnored */
    TEST_ASSERT_EQUAL(savedFails + 1, Unity.TestFailures);
    TEST_ASSERT_EQUAL(savedIgnores + 1, Unity.TestIgnores);
    Unity.TestFailures = savedFails;
    Unity.TestIgnores = savedIgnores;
}

/*------------------------------------------------------------ */

TEST_GROUP(UnityCommandOptions);

static int savedVerbose;
static unsigned int savedRepeat;
static const char* savedName;
static const char* savedGroup;

TEST_SETUP(UnityCommandOptions)
{
    savedVerbose = UnityFixture.Verbose;
    savedRepeat = UnityFixture.RepeatCount;
    savedName = UnityFixture.NameFilter;
    savedGroup = UnityFixture.GroupFilter;
}

TEST_TEAR_DOWN(UnityCommandOptions)
{
    UnityFixture.Verbose = savedVerbose;
    UnityFixture.RepeatCount= savedRepeat;
    UnityFixture.NameFilter = savedName;
    UnityFixture.GroupFilter = savedGroup;
}


static const char* noOptions[] = {
        "testrunner.exe"
};

TEST(UnityCommandOptions, DefaultOptions)
{
    UnityGetCommandLineOptions(1, noOptions);
    TEST_ASSERT_EQUAL(0, UnityFixture.Verbose);
    TEST_ASSERT_POINTERS_EQUAL(0, UnityFixture.GroupFilter);
    TEST_ASSERT_POINTERS_EQUAL(0, UnityFixture.NameFilter);
    TEST_ASSERT_EQUAL(1, UnityFixture.RepeatCount);
}

static const char* verbose[] = {
        "testrunner.exe",
        "-v"
};

TEST(UnityCommandOptions, OptionVerbose)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(2, verbose));
    TEST_ASSERT_EQUAL(1, UnityFixture.Verbose);
}

static const char* group[] = {
        "testrunner.exe",
        "-g", "groupname"
};

TEST(UnityCommandOptions, OptionSelectTestByGroup)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(3, group));
    STRCMP_EQUAL("groupname", UnityFixture.GroupFilter);
}

static const char* name[] = {
        "testrunner.exe",
        "-n", "testname"
};

TEST(UnityCommandOptions, OptionSelectTestByName)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(3, name));
    STRCMP_EQUAL("testname", UnityFixture.NameFilter);
}

static const char* repeat[] = {
        "testrunner.exe",
        "-r", "99"
};

TEST(UnityCommandOptions, OptionSelectRepeatTestsDefaultCount)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(2, repeat));
    TEST_ASSERT_EQUAL(2, UnityFixture.RepeatCount);
}

TEST(UnityCommandOptions, OptionSelectRepeatTestsSpecificCount)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(3, repeat));
    TEST_ASSERT_EQUAL(99, UnityFixture.RepeatCount);
}

static const char* multiple[] = {
        "testrunner.exe",
        "-v",
        "-g", "groupname",
        "-n", "testname",
        "-r", "98"
};

TEST(UnityCommandOptions, MultipleOptions)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(8, multiple));
    TEST_ASSERT_EQUAL(1, UnityFixture.Verbose);
    STRCMP_EQUAL("groupname", UnityFixture.GroupFilter);
    STRCMP_EQUAL("testname", UnityFixture.NameFilter);
    TEST_ASSERT_EQUAL(98, UnityFixture.RepeatCount);
}

static const char* dashRNotLast[] = {
        "testrunner.exe",
        "-v",
        "-g", "gggg",
        "-r",
        "-n", "tttt",
};

TEST(UnityCommandOptions, MultipleOptionsDashRNotLastAndNoValueSpecified)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(7, dashRNotLast));
    TEST_ASSERT_EQUAL(1, UnityFixture.Verbose);
    STRCMP_EQUAL("gggg", UnityFixture.GroupFilter);
    STRCMP_EQUAL("tttt", UnityFixture.NameFilter);
    TEST_ASSERT_EQUAL(2, UnityFixture.RepeatCount);
}

static const char* unknownCommand[] = {
        "testrunner.exe",
        "-v",
        "-g", "groupname",
        "-n", "testname",
        "-r", "98",
        "-z"
};
TEST(UnityCommandOptions, UnknownCommandIsIgnored)
{
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(9, unknownCommand));
    TEST_ASSERT_EQUAL(1, UnityFixture.Verbose);
    STRCMP_EQUAL("groupname", UnityFixture.GroupFilter);
    STRCMP_EQUAL("testname", UnityFixture.NameFilter);
    TEST_ASSERT_EQUAL(98, UnityFixture.RepeatCount);
}

TEST(UnityCommandOptions, GroupOrNameFilterWithoutStringFails)
{
    TEST_ASSERT_EQUAL(1, UnityGetCommandLineOptions(3, unknownCommand));
    TEST_ASSERT_EQUAL(1, UnityGetCommandLineOptions(5, unknownCommand));
    TEST_ASSERT_EQUAL(1, UnityMain(3, unknownCommand, NULL));
}

TEST(UnityCommandOptions, GroupFilterReallyFilters)
{
    UNITY_UINT saved = Unity.NumberOfTests;
    TEST_ASSERT_EQUAL(0, UnityGetCommandLineOptions(4, unknownCommand));
    UnityIgnoreTest(NULL, "non-matching", NULL);
    TEST_ASSERT_EQUAL(saved, Unity.NumberOfTests);
}

IGNORE_TEST(UnityCommandOptions, TestShouldBeIgnored)
{
    TEST_FAIL_MESSAGE("This test should not run!");
}
