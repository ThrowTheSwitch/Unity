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
    uint8_t* crazyString = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8081\r\nConnection: keep-alive\r\nCache-Control: no-cache\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36\r\nPostman-Token: 768c7149-c3fb-f704-71a2-63918d9195b2\r\nAccept: */*\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: en-GB,en-US;q=0.8,en;q=0.6\r\n\r\n";

    Call_An_Expect();

    CallAFunction();
    test_CallAFunctionThatLooksLikeATest();
}

void test_TheThirdThingToTest(void)
{
    CallAFunction();
}

void test_TheFourthThingToTest(void)
{
    uint8_t* anotherString = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8081\r\nConnection: keep-alive\r\nCache-Control: no-cache\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36\r\nPostman-Token: 768c7149-c3fb-f704-71a2-63918d9195b2\r\nAccept: */*\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: en-GB,en-US;q=0.8,en;q=0.6\r\n\r\n";

    Call_An_Expect();

    CallAFunction();
    test_CallAFunctionThatLooksLikeATest();
}

#endif //TEST_SAMPLE_H
