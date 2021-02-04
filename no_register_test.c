#include "unity.h"

TEST_SUITE("Simple no-register test suite.") {
    TEST("Should pass") {
        TEST_ASSERT_EQUAL(0, 0);
    }
}

