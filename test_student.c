// test_student.c

#include "unity.h"
#include "student.h"  // Your original code's header file

void setUp(void) {
    // Code to run before each test
}

void tearDown(void) {
    // Code to run after each test
}

void test_calculate_percentage_all_same_marks(void) {
    struct student s = {"John", {80, 80, 80, 80, 80}, 0.0};
    calculate_percentage(&s);
    TEST_ASSERT_EQUAL_FLOAT(80.0, s.percentage);
}

void test_calculate_percentage_different_marks(void) {
    struct student s = {"Alice", {90, 80, 85, 70, 75}, 0.0};
    calculate_percentage(&s);
    TEST_ASSERT_EQUAL_FLOAT(80.0, s.percentage);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_calculate_percentage_all_same_marks);
    RUN_TEST(test_calculate_percentage_different_marks);
    return UNITY_END();
}
