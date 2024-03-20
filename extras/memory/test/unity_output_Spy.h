/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#ifndef UNITY_OUTPUT_SPY_H
#define UNITY_OUTPUT_SPY_H

void UnityOutputCharSpy_Create(int s);
void UnityOutputCharSpy_Destroy(void);
void UnityOutputCharSpy_OutputChar(int c);
const char * UnityOutputCharSpy_Get(void);
void UnityOutputCharSpy_Enable(int enable);

#endif
