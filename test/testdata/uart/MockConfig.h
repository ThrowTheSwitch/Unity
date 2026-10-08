/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#ifndef UART_MOCK_CONFIG_H
#define UART_MOCK_CONFIG_H

extern int uart_MockConfig_Init_Counter;
extern int uart_MockConfig_Verify_Counter;
extern int uart_MockConfig_Destroy_Counter;

void uart_MockConfig_Init(void);
void uart_MockConfig_Verify(void);
void uart_MockConfig_Destroy(void);

#endif
