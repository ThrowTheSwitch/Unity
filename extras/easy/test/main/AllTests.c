/* Copyright (c) 2010 James Grenning and Contributed to Unity Project
 * ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */
// by using unity_easy.h , not need to write the run all test function,just need a name.
#include "unity_easy.h"

RUN_ALL_TEST(runAllTests);


int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, runAllTests);
}

