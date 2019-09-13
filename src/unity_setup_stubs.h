/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007-19 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_SETUP_STUBS_H
#define UNITY_SETUP_STUBS_H

#include "unity.h"

/* If the compiler supports it, this header provides stub implementations
 * of the following functions as weak symbols:
 *
 *   - setUp()
 *   - tearDown()
 *   - suiteSetUp()
 *   - suiteTearDown()
 *
 * This header is always included by unity.c, which is sufficient when
 * targeting GNU/Linux and other ELF targets.  On some other platforms
 * (MinGW for example), weak function implementations need to be in the
 * same translation unit they are called from, so this header is also
 * included by each generated test runner.
 *
 * If neither UNITY_WEAK_ATTRIBUTE nor UNITY_WEAK_PRAGMA is defined, the
 * compiler is assumed to have no support for weak symbols, and this
 * header has no effect. */

#ifdef UNITY_WEAK_ATTRIBUTE
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
