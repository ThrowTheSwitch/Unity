/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "uart/MockConfig.h"
#include "spi/MockConfig.h"

/* Stand-ins for two mocks sharing a filename, named by the folder each is included by */
int uart_MockConfig_Init_Counter = 0;
int uart_MockConfig_Verify_Counter = 0;
int uart_MockConfig_Destroy_Counter = 0;
int spi_MockConfig_Init_Counter = 0;
int spi_MockConfig_Verify_Counter = 0;
int spi_MockConfig_Destroy_Counter = 0;

void uart_MockConfig_Init(void)    { uart_MockConfig_Init_Counter++;    }
void uart_MockConfig_Verify(void)  { uart_MockConfig_Verify_Counter++;  }
void uart_MockConfig_Destroy(void) { uart_MockConfig_Destroy_Counter++; }
void spi_MockConfig_Init(void)     { spi_MockConfig_Init_Counter++;     }
void spi_MockConfig_Verify(void)   { spi_MockConfig_Verify_Counter++;   }
void spi_MockConfig_Destroy(void)  { spi_MockConfig_Destroy_Counter++;  }

void setUp(void) {}
void tearDown(void) {}

void test_ShouldInitEachSameNamedMock(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(1, uart_MockConfig_Init_Counter, "First Mock Init Should Be Called");
    TEST_ASSERT_EQUAL_MESSAGE(1, spi_MockConfig_Init_Counter,  "Second Mock Init Should Be Called");
}

void test_ShouldVerifyAndDestroyEachSameNamedMock(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(1, uart_MockConfig_Verify_Counter,  "First Mock Verify Should Be Called");
    TEST_ASSERT_EQUAL_MESSAGE(1, spi_MockConfig_Verify_Counter,   "Second Mock Verify Should Be Called");
    TEST_ASSERT_EQUAL_MESSAGE(1, uart_MockConfig_Destroy_Counter, "First Mock Destroy Should Be Called");
    TEST_ASSERT_EQUAL_MESSAGE(1, spi_MockConfig_Destroy_Counter,  "Second Mock Destroy Should Be Called");
}
