/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#ifndef MOCK_MOCK_H
#define MOCK_MOCK_H

extern int mockMock_Init_Counter;
extern int mockMock_Verify_Counter;
extern int mockMock_Destroy_Counter;
extern int CMockMemFreeFinalCounter;

void mockMock_Init(void);
void mockMock_Verify(void);
void mockMock_Destroy(void);

#endif
