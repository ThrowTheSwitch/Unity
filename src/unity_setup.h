/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007-14 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_SETUP_H
#define UNITY_SETUP_H

#include "unity_internals.h"

/* On some platforms (MinGW for example), weak function implementations
 * need to be in the same translation unit they are called from.  This
 * header can be included to provide implementations of setUp(), tearDown(),
 * suiteSetUp(), and suiteTearDown(). */

#if defined(UNITY_WEAK_ATTRIBUTE)
  UNITY_WEAK_ATTRIBUTE void setUp(void) { }
  UNITY_WEAK_ATTRIBUTE void tearDown(void) { }
  UNITY_WEAK_ATTRIBUTE void suiteSetUp(void) { }
  UNITY_WEAK_ATTRIBUTE int suiteTearDown(int num_failures) { return num_failures; }
#elif defined(UNITY_WEAK_PRAGMA)
  #pragma weak setUp
  void setUp(void) { }
  #pragma weak tearDown
  void tearDown(void) { }
  #pragma weak suiteSetUp
  void suiteSetUp(void) { }
  #pragma weak suiteTearDown
  int suiteTearDown(int num_failures) { return num_failures; }
#endif

#endif
