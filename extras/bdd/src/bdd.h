/* Copyright (c) 2023 Michael Gene Brockus (Dreamer) and Contributed to Unity Project
 * ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

#ifndef UNITY_BDD_TEST_H_
#define UNITY_BDD_TEST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

/**
 * @brief Macros for defining a Behavior-Driven Development (BDD) structure with descriptions.
 *
 * These macros provide a way to structure and describe different phases (Given, When, Then) of a
 * test scenario in a BDD-style format. However, they don't have functional behavior by themselves
 * and are used for descriptive purposes.
 */
#define GIVEN(description) \
    if (0) { \
        printf("Given %s\n", description); \
    } else

#define WHEN(description) \
    if (0) { \
        printf("When %s\n", description); \
    } else

#define THEN(description) \
    if (0) { \
        printf("Then %s\n", description); \
    } else

#ifdef __cplusplus
}
#endif

#endif 
