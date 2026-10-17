/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#ifndef SPI_MOCK_CONFIG_H
#define SPI_MOCK_CONFIG_H

extern int spi_MockConfig_Init_Counter;
extern int spi_MockConfig_Verify_Counter;
extern int spi_MockConfig_Destroy_Counter;

void spi_MockConfig_Init(void);
void spi_MockConfig_Verify(void);
void spi_MockConfig_Destroy(void);

#endif
