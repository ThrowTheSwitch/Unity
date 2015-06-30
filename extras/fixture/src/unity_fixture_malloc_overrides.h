//- Copyright (c) 2010 James Grenning and Contributed to Unity Project
/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_FIXTURE_MALLOC_OVERRIDES_H_
#define UNITY_FIXTURE_MALLOC_OVERRIDES_H_

#include <stddef.h>

// This function is used by the Unity Fixture to allocate memory on
// the heap and can be overridden with platform-specific heap
// implementations. For example, when using FreeRTOS
// UNITY_FIXTURE_MALLOC becomes pvPortMalloc().

#ifndef UNITY_FIXTURE_MALLOC
   #define UNITY_FIXTURE_MALLOC( SIZE ) malloc( ( SIZE ) )
#else
   extern void * UNITY_FIXTURE_MALLOC(size_t size);
#endif

// This function is used by the Unity Fixture to release memory in the
// heap and can be overridden with platform-specific heap
// implementations. For example, when using FreeRTOS
// UNITY_FIXTURE_FREE becomes vPortFree().

#ifndef UNITY_FIXTURE_FREE
   #define UNITY_FIXTURE_FREE( PTR ) free( ( PTR ) )
#else
   extern void UNITY_FIXTURE_FREE(void *ptr);
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
