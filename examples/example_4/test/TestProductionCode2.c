
#include "ProductionCode2.h"
#include "unity.h"

/* These should be ignored because they are commented out in various ways:
#include "whatever.h" 
#include "somethingelse.h"
*/

void setUp(void)
{
}

void tearDown(void)
{
}

void test_IgnoredTest(void);
void test_AnotherIgnoredTest(void);
void test_ThisFunctionHasNotBeenTested_NeedsToBeImplemented(void);

void test_IgnoredTest(void)
{
    TEST_IGNORE_MESSAGE("This Test Was Ignored On Purpose");
}

void test_AnotherIgnoredTest(void)
{
    TEST_IGNORE_MESSAGE("These Can Be Useful For Leaving Yourself Notes On What You Need To Do Yet");
}

void test_ThisFunctionHasNotBeenTested_NeedsToBeImplemented(void)
{
    TEST_IGNORE(); /* Like This */
}
