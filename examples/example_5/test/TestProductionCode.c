/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ProductionCode.h"
#include "unity.h"

const int* current_expected_bits = NULL;
UNITY_LINE_TYPE current_vector_line = 0;
typedef struct {
  UNITY_LINE_TYPE line;
  uint8_t value;
  bit_direction_t dir;
  int expected_bits[8];
} test_vector_t;

void setUp(void)
{
}

void tearDown(void)
{
}

static void be_bit_tester(int position, int value) {
  UNITY_TEST_ASSERT_EQUAL_INT(current_expected_bits[position], value, current_vector_line, "Unexpected bit value");
}

void test_BitExtractor(void)
{
  const test_vector_t test_vectors[] = {
    {__LINE__, 7, BIT_DIRECTION_UP, {1,1,1,0,0,0,0,0}},
    {__LINE__, 7, BIT_DIRECTION_DOWN, {0,0,0,0,0,1,0,1}},  /* intentionally wrong to demonstrate detail output */
    {0}
  };
  const test_vector_t* tv;
  for (tv = test_vectors; tv->line; tv++) {
    current_vector_line = tv->line;
    current_expected_bits = tv->expected_bits;
    BitExtractor(tv->dir, tv->value, be_bit_tester);
  }
}
