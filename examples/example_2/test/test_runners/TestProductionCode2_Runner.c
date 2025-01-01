/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(ProductionCode2)
{
  RUN_TEST_CASE(ProductionCode2, IgnoredTest);
  RUN_TEST_CASE(ProductionCode2, AnotherIgnoredTest);
  RUN_TEST_CASE(ProductionCode2, ThisFunctionHasNotBeenTested_NeedsToBeImplemented);
}