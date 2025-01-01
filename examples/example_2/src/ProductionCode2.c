/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ProductionCode2.h"

char* ThisFunctionHasNotBeenTested(int Poor, char* LittleFunction)
{
  (void)Poor;
  (void)LittleFunction;
  //Since There Are No Tests Yet, This Function Could Be Empty For All We Know.
  // Which isn't terribly useful... but at least we put in a TEST_IGNORE so we won't forget
  return (char*)0;
}
