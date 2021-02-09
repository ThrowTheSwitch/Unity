#include "unity.h"

TEST_SUITE("Simple no-register test suite") {
  TEST("Two zeros should equal") {
    int a, b;
    a = 0;
    b = 0;
    TEST_ASSERT_EQUAL(a, b);
  }

  TEST("A zero and a one should not equal") {
    int a, b;
    a = 0;
    b = 1;
    TEST_ASSERT_NOT_EQUAL(a, b);
  }

  TEST("A null pointer should be null") {
    void *p;
    p = NULL;
    TEST_ASSERT_NULL(p);
  }

  TEST("A blank test should do no harm") {}

  TEST("An ignored test should do no harm") {
    TEST_IGNORE_MESSAGE("This test is definitely being ignored");
  }

  TEST("An aborted test should do no harm") {
    int a, b;
    TEST_PASS_MESSAGE("Future failures should not be documented in this test");
    a = 1;
    b = 2;
    TEST_ASSERT_EQUAL(a, b);
  }

  TEST("Comparison of strings should not break anything thus far") {
    char *str_a, *str_b;
    str_a = "Hello, World.";
    str_b = "Hello, World.";
    TEST_ASSERT_EQUAL_STRING(str_a, str_b);
  }
}
