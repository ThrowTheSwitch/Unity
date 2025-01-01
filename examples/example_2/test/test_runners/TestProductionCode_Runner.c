/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(ProductionCode)
{
  RUN_TEST_CASE(ProductionCode, FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode);
  RUN_TEST_CASE(ProductionCode, FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken);
  RUN_TEST_CASE(ProductionCode, FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue);
  RUN_TEST_CASE(ProductionCode, FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain);
  RUN_TEST_CASE(ProductionCode, FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed);
}