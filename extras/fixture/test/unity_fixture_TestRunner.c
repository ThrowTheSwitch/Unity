/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity_fixture.h"

TEST_GROUP_RUNNER(UnityFixture)
{
    RUN_TEST_CASE(UnityFixture, PointerSetting);
    RUN_TEST_CASE(UnityFixture, PointerSet);
    RUN_TEST_CASE(UnityFixture, FreeNULLSafety);
    RUN_TEST_CASE(UnityFixture, ConcludeTestIncrementsFailCount);
}

TEST_GROUP_RUNNER(UnityCommandOptions)
{
    RUN_TEST_CASE(UnityCommandOptions, DefaultOptions);
    RUN_TEST_CASE(UnityCommandOptions, OptionVerbose);
    RUN_TEST_CASE(UnityCommandOptions, OptionDryRun);
    RUN_TEST_CASE(UnityCommandOptions, OptionSelectTestByGroup);
    RUN_TEST_CASE(UnityCommandOptions, OptionSelectTestByName);
    RUN_TEST_CASE(UnityCommandOptions, OptionSelectTestByGroupExact);
    RUN_TEST_CASE(UnityCommandOptions, OptionSelectTestByNameExact);
    RUN_TEST_CASE(UnityCommandOptions, OptionSelectRepeatTestsDefaultCount);
    RUN_TEST_CASE(UnityCommandOptions, OptionSelectRepeatTestsSpecificCount);
    RUN_TEST_CASE(UnityCommandOptions, MultipleOptions);
    RUN_TEST_CASE(UnityCommandOptions, MultipleOptionsDashRNotLastAndNoValueSpecified);
    RUN_TEST_CASE(UnityCommandOptions, UnknownCommandIsIgnored);
    RUN_TEST_CASE(UnityCommandOptions, GroupOrNameFilterWithoutStringFails);
    RUN_TEST_CASE(UnityCommandOptions, GroupFilterReallyFilters);
    RUN_TEST_CASE(UnityCommandOptions, TestShouldBeIgnored);
}
