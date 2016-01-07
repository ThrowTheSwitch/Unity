// This is just a sample test file to be used to test the generator script
#ifndef TEST_SAMPLE_H
#define TEST_SAMPLE_H

#include <setjmp.h>
#include "unity.h"
#include "funky.h"
#include "stanky.h"

void setUp(void)
{
  CustomSetupStuff();
}

void tearDown(void)
{
  CustomTeardownStuff
}

//Yup, nice comment
void test_TheFirstThingToTest(void)
{
    TEST_ASSERT(1);

    TEST_ASSERT_TRUE(1);
}

/*
void test_ShouldBeIgnored(void)
{
    DoesStuff();
}
*/

//void test_ShouldAlsoNotBeTested(void)
//{
//    Call_An_Expect();
//    
//    CallAFunction();
//    test_CallAFunctionThatLooksLikeATest();
//}

void test_TheSecondThingToTest(void)
{
    Call_An_Expect();
    
    CallAFunction();
    test_CallAFunctionThatLooksLikeATest();
}

#endif //TEST_SAMPLE_H
