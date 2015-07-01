//- Copyright (c) 2010 James Grenning and Contributed to Unity Project
/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */


#include "unity_output_Spy.h"
#include "unity_fixture.h"

#include <stdio.h>
#include <string.h>

static int size;
static int count;
static char* buffer;
static int spy_enable;

void UnityOutputCharSpy_Create(int s)
{
    size = s;
    count = 0;
    spy_enable = 0;
    buffer = UNITY_FIXTURE_MALLOC(size);
    memset(buffer, 0, size);
}

void UnityOutputCharSpy_Destroy(void)
{
    size = 0;
    UNITY_FIXTURE_FREE(buffer);
}

int UnityOutputCharSpy_OutputChar(int c)
{
    if (spy_enable)
    {
        if (count < (size-1))
            buffer[count++] = c;
    }
    else
    {
        putchar(c);
    }
    return c;
}

const char * UnityOutputCharSpy_Get(void)
{
    return buffer;
}

void UnityOutputCharSpy_Enable(int enable)
{
    spy_enable = enable;
}
