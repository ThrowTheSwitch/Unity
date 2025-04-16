/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include <stdint.h>

typedef void callback_t(int position, int bit_value);
typedef enum {
    BIT_DIRECTION_UP,
    BIT_DIRECTION_DOWN,
} bit_direction_t;

void BitExtractor(bit_direction_t dir, uint8_t input, callback_t cb);
