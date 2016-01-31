//- Copyright (c) 2010 James Grenning and Contributed to Unity Project
/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_FIXTURE_MALLOC_OVERRIDES_H_
#define UNITY_FIXTURE_MALLOC_OVERRIDES_H_

#include <stddef.h>

// These functions are used by the Unity Fixture to allocate and release memory
// on the heap and can be overridden with platform-specific implementations.
// For example, when using FreeRTOS UNITY_FIXTURE_MALLOC becomes pvPortMalloc()
// and UNITY_FIXTURE_FREE becomes vPortFree().

#if !defined(UNITY_FIXTURE_MALLOC) || !defined(UNITY_FIXTURE_FREE)
    #define UNITY_FIXTURE_MALLOC(size) malloc(size)
    #define UNITY_FIXTURE_FREE(ptr)    free(ptr)
#else
    extern void* UNITY_FIXTURE_MALLOC(size_t size);
    extern void UNITY_FIXTURE_FREE(void* ptr);
#endif

#define malloc  unity_malloc
#define calloc  unity_calloc
#define realloc unity_realloc
#define free    unity_free

void* unity_malloc(size_t size);
void* unity_calloc(size_t num, size_t size);
void* unity_realloc(void * oldMem, size_t size);
void unity_free(void * mem);

#endif /* UNITY_FIXTURE_MALLOC_OVERRIDES_H_ */
