/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ProductionCode.h"

#include <stdint.h>

#ifdef UNIT_TESTING
#include "unity.h"
#else
/* No-Op when not testing */
#define UNITY_DETAIL_PUSH
#define UNITY_DETAIL_POP
#endif

static void BitExtractor_up(uint8_t input, callback_t cb)
{
    int32_t pos;
    UNITY_DETAIL_PUSH(UNITY_DETAIL_CALL, __FUNCTION__);
    for(pos=0; pos<8; pos++) {
        UNITY_DETAIL_PUSH(UNITY_DETAIL_BIT_POS, pos);
        UNITY_DETAIL_PUSH(UNITY_DETAIL_BIT_MASK, 1<<pos);
        cb(pos, !!(input & (1<<pos)));
        UNITY_DETAIL_POP(UNITY_DETAIL_BIT_MASK, 1<<pos);
        UNITY_DETAIL_POP(UNITY_DETAIL_BIT_POS, pos);
    }
    UNITY_DETAIL_POP(UNITY_DETAIL_CALL, __FUNCTION__);
}

static void BitExtractor_down(uint8_t input, callback_t cb)
{
    int32_t pos;
    UNITY_DETAIL_PUSH(UNITY_DETAIL_CALL, __FUNCTION__);
    for(pos=0; pos<8; pos++) {
        UNITY_DETAIL_PUSH(UNITY_DETAIL_BIT_POS, pos);
        UNITY_DETAIL_PUSH(UNITY_DETAIL_BIT_MASK, 0x80>>pos);
        cb(pos, !!(input & (0x80>>pos)));
        UNITY_DETAIL_POP(UNITY_DETAIL_BIT_MASK, 0x80>>pos);
        UNITY_DETAIL_POP(UNITY_DETAIL_BIT_POS, pos);
    }
    UNITY_DETAIL_POP(UNITY_DETAIL_CALL, __FUNCTION__);
}
void BitExtractor(bit_direction_t dir, uint8_t input, callback_t cb)
{
    UNITY_DETAIL_PUSH(UNITY_DETAIL_CALL, __FUNCTION__);
    if(dir == BIT_DIRECTION_UP) {
        BitExtractor_up(input, cb);
    } else {
        BitExtractor_down(input, cb);
    }
    UNITY_DETAIL_POP(UNITY_DETAIL_CALL, __FUNCTION__);
}
