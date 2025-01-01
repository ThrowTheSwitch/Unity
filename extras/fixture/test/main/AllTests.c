/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity_fixture.h"

static void runAllTests(void)
{
    RUN_TEST_GROUP(UnityFixture);
    RUN_TEST_GROUP(UnityCommandOptions);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, runAllTests);
}

