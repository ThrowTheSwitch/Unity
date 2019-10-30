/* ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

#ifndef UNITY_MEMORY_OVERRIDES_H_
#define UNITY_MEMORY_OVERRIDES_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

#ifdef UNITY_EXCLUDE_STDLIB_MALLOC
/* Define this macro to remove the use of stdlib.h, malloc, and free.
 * Many embedded systems do not have a heap or malloc/free by default.
 * This internal unity_malloc() provides allocated memory deterministically from
 * the end of an array only, unity_free() only releases from end-of-array,
 * blocks are not coalesced, and memory not freed in LIFO order is stranded. */
    #ifndef UNITY_INTERNAL_HEAP_SIZE_BYTES
    #define UNITY_INTERNAL_HEAP_SIZE_BYTES 256
    #endif
#endif

/* These functions are used by Unity to allocate and release memory
 * on the heap and can be overridden with platform-specific implementations.
 * For example, when using FreeRTOS UNITY_MALLOC becomes pvPortMalloc()
 * and UNITY_FREE becomes vPortFree(). */
#if !defined(UNITY_MALLOC) || !defined(UNITY_FREE)
    #include <stdlib.h>
    #define UNITY_MALLOC(size) malloc(size)
    #define UNITY_FREE(ptr)    free(ptr)
#else
    extern void* UNITY_MALLOC(size_t size);
    extern void UNITY_FREE(void* ptr);
#endif

#define malloc  unity_malloc
#define calloc  unity_calloc
#define realloc unity_realloc
#define free    unity_free

void* unity_malloc(size_t size);
void* unity_calloc(size_t num, size_t size);
void* unity_realloc(void * oldMem, size_t size);
void unity_free(void * mem);

/* You must compile with malloc replacement, as defined in unity_fixture_malloc_overrides.h */
void UnityMalloc_StartTest(void);
void UnityMalloc_EndTest(void);
void UnityMalloc_MakeMallocFailAfterCount(int countdown);

#ifdef __cplusplus
}
#endif

#endif 
