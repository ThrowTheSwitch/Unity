/* ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

#include "unity.h"
#include "unity_memory.h"
#include "unity_output_Spy.h"
#include <stdlib.h>
#include <string.h>

/* This test module includes the following tests: */

void test_ForceMallocFail(void);
void test_ReallocSmallerIsUnchanged(void);
void test_ReallocSameIsUnchanged(void);
void test_ReallocLargerNeeded(void);
void test_ReallocNullPointerIsLikeMalloc(void);
void test_ReallocSizeZeroFreesMemAndReturnsNullPointer(void);
void test_CallocFillsWithZero(void);
void test_FreeNULLSafety(void);
void test_DetectsLeak(void);
void test_BufferOverrunFoundDuringFree(void);
void test_BufferOverrunFoundDuringRealloc(void);
void test_BufferGuardWriteFoundDuringFree(void);
void test_BufferGuardWriteFoundDuringRealloc(void);
void test_MallocPastBufferFails(void);
void test_CallocPastBufferFails(void);
void test_MallocThenReallocGrowsMemoryInPlace(void);
void test_ReallocFailDoesNotFreeMem(void);

/* It makes use of the following features */
void setUp(void);
void tearDown(void);

/* Let's Go! */
void setUp(void) 
{ 
#ifdef UNITY_EXCLUDE_STDLIB_MALLOC
    UnityOutputCharSpy_Create(200);
#else
    UnityOutputCharSpy_Create(1000);
#endif
    UnityMalloc_StartTest();
}

void tearDown(void) 
{ 
    UnityMalloc_EndTest();
    UnityOutputCharSpy_Destroy();
}

void test_ForceMallocFail(void)
{
    void* m;
    void* mfails;
    UnityMalloc_MakeMallocFailAfterCount(1);
    m = malloc(10);
    TEST_ASSERT_NOT_NULL(m);
    mfails = malloc(10);
    TEST_ASSERT_EQUAL_PTR(0, mfails);
    free(m);
}

void test_ReallocSmallerIsUnchanged(void)
{
    void* m1 = malloc(10);
    void* m2 = realloc(m1, 5);
    TEST_ASSERT_NOT_NULL(m1);
    TEST_ASSERT_EQUAL_PTR(m1, m2);
    free(m2);
}

void test_ReallocSameIsUnchanged(void)
{
    void* m1 = malloc(10);
    void* m2 = realloc(m1, 10);
    TEST_ASSERT_NOT_NULL(m1);
    TEST_ASSERT_EQUAL_PTR(m1, m2);
    free(m2);
}

void test_ReallocLargerNeeded(void)
{
    void* m2;
    void* m1 = malloc(10);
    TEST_ASSERT_NOT_NULL(m1);
    strcpy((char*)m1, "123456789");
    m2 = realloc(m1, 15);
    TEST_ASSERT_EQUAL_STRING("123456789", m2);
    free(m2);
}

void test_ReallocNullPointerIsLikeMalloc(void)
{
    void* m = realloc(0, 15);
    TEST_ASSERT_NOT_NULL(m);
    free(m);
}

void test_ReallocSizeZeroFreesMemAndReturnsNullPointer(void)
{
    void* m1 = malloc(10);
    void* m2 = realloc(m1, 0);
    TEST_ASSERT_EQUAL_PTR(0, m2);
}

void test_CallocFillsWithZero(void)
{
    void* m = calloc(3, sizeof(char));
    char* s = (char*)m;
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_HEX8(0, s[0]);
    TEST_ASSERT_EQUAL_HEX8(0, s[1]);
    TEST_ASSERT_EQUAL_HEX8(0, s[2]);
    free(m);
}

void test_FreeNULLSafety(void)
{
    free(NULL);
}

/*------------------------------------------------------------ */

#define EXPECT_ABORT_BEGIN \
  { \
    jmp_buf TestAbortFrame;   \
    memcpy(TestAbortFrame, Unity.AbortFrame, sizeof(jmp_buf)); \
    if (TEST_PROTECT()) \
    {

#define EXPECT_ABORT_END \
    } \
    memcpy(Unity.AbortFrame, TestAbortFrame, sizeof(jmp_buf)); \
  }

/* This tricky set of defines lets us see if we are using the Spy, returns 1 if true */
#ifdef __STDC_VERSION__

#ifdef UNITY_SUPPORT_VARIADIC_MACROS
#define USING_SPY_AS(a)                    EXPAND_AND_USE_2ND(ASSIGN_VALUE(a), 0)
#define ASSIGN_VALUE(a)                    VAL_##a
#define VAL_UnityOutputCharSpy_OutputChar  0, 1
#define EXPAND_AND_USE_2ND(a, b)           SECOND_PARAM(a, b, throwaway)
#define SECOND_PARAM(a, b, ...)            b
#if USING_SPY_AS(UNITY_OUTPUT_CHAR)
  #define USING_OUTPUT_SPY /* UNITY_OUTPUT_CHAR = UnityOutputCharSpy_OutputChar */
#endif
#endif /* UNITY_SUPPORT_VARIADIC_MACROS */

#else  /* __STDC_VERSION__ else */

#define UnityOutputCharSpy_OutputChar 42
#if UNITY_OUTPUT_CHAR == UnityOutputCharSpy_OutputChar /* Works if no -Wundef -Werror */
  #define USING_OUTPUT_SPY
#endif
#undef UnityOutputCharSpy_OutputChar
  
#endif /* __STDC_VERSION__ */

void test_DetectsLeak(void)
{
#ifdef USING_OUTPUT_SPY
    void* m = malloc(10);
    TEST_ASSERT_NOT_NULL(m);
    UnityOutputCharSpy_Enable(1);
    EXPECT_ABORT_BEGIN
    UnityMalloc_EndTest();
    EXPECT_ABORT_END
    UnityOutputCharSpy_Enable(0);
    Unity.CurrentTestFailed = 0;
    TEST_ASSERT_NOT_NULL(strstr(UnityOutputCharSpy_Get(), "This test leaks!"));
    free(m);
#else
    TEST_IGNORE_MESSAGE("Enable USING_OUTPUT_SPY To Run This Test");
#endif
}

void test_BufferOverrunFoundDuringFree(void)
{
#ifdef USING_OUTPUT_SPY
    void* m = malloc(10);
    char* s = (char*)m;
    TEST_ASSERT_NOT_NULL(m);
    s[10] = (char)0xFF;
    UnityOutputCharSpy_Enable(1);
    EXPECT_ABORT_BEGIN
    free(m);
    EXPECT_ABORT_END
    UnityOutputCharSpy_Enable(0);
    Unity.CurrentTestFailed = 0;
    TEST_ASSERT_NOT_NULL(strstr(UnityOutputCharSpy_Get(), "Buffer overrun detected during free()"));
#else
    TEST_IGNORE_MESSAGE("Enable USING_OUTPUT_SPY To Run This Test");
#endif
}

void test_BufferOverrunFoundDuringRealloc(void)
{
#ifdef USING_OUTPUT_SPY
    void* m = malloc(10);
    char* s = (char*)m;
    TEST_ASSERT_NOT_NULL(m);
    s[10] = (char)0xFF;
    UnityOutputCharSpy_Enable(1);
    EXPECT_ABORT_BEGIN
    m = realloc(m, 100);
    EXPECT_ABORT_END
    UnityOutputCharSpy_Enable(0);
    Unity.CurrentTestFailed = 0;
    TEST_ASSERT_NOT_NULL(strstr(UnityOutputCharSpy_Get(), "Buffer overrun detected during realloc()"));
#else
    TEST_IGNORE_MESSAGE("Enable USING_OUTPUT_SPY To Run This Test");
#endif
}

void test_BufferGuardWriteFoundDuringFree(void)
{
#ifdef USING_OUTPUT_SPY
    void* m = malloc(10);
    char* s = (char*)m;
    TEST_ASSERT_NOT_NULL(m);
    s[-1] = (char)0x00; /* Will not detect 0 */
    s[-2] = (char)0x01;
    UnityOutputCharSpy_Enable(1);
    EXPECT_ABORT_BEGIN
    free(m);
    EXPECT_ABORT_END
    UnityOutputCharSpy_Enable(0);
    Unity.CurrentTestFailed = 0;
    TEST_ASSERT_NOT_NULL(strstr(UnityOutputCharSpy_Get(), "Buffer overrun detected during free()"));
#else
    TEST_IGNORE_MESSAGE("Enable USING_OUTPUT_SPY To Run This Test");
#endif
}

void test_BufferGuardWriteFoundDuringRealloc(void)
{
#ifdef USING_OUTPUT_SPY
    void* m = malloc(10);
    char* s = (char*)m;
    TEST_ASSERT_NOT_NULL(m);
    s[-1] = (char)0x0A;
    UnityOutputCharSpy_Enable(1);
    EXPECT_ABORT_BEGIN
    m = realloc(m, 100);
    EXPECT_ABORT_END
    UnityOutputCharSpy_Enable(0);
    Unity.CurrentTestFailed = 0;
    TEST_ASSERT_NOT_NULL(strstr(UnityOutputCharSpy_Get(), "Buffer overrun detected during realloc()"));
#else
    TEST_IGNORE_MESSAGE("Enable USING_OUTPUT_SPY To Run This Test");
#endif
}

/*------------------------------------------------------------ */

#define TEST_ASSERT_MEMORY_ALL_FREE_LIFO_ORDER(first_mem_ptr, ptr) \
    ptr = malloc(10); free(ptr);                                   \
    TEST_ASSERT_EQUAL_PTR_MESSAGE(first_mem_ptr, ptr, "Memory was stranded, free in LIFO order");

void test_MallocPastBufferFails(void)
{
#ifdef UNITY_EXCLUDE_STDLIB_MALLOC
    void* m = malloc(UNITY_INTERNAL_HEAP_SIZE_BYTES/2 + 1);
    void* n = malloc(UNITY_INTERNAL_HEAP_SIZE_BYTES/2);
    free(m);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_NULL(n);
    TEST_ASSERT_MEMORY_ALL_FREE_LIFO_ORDER(m, n);
#else
    TEST_IGNORE_MESSAGE("Enable UNITY_EXCLUDE_STDLIB_MALLOC to Run This Test");
#endif
}

void test_CallocPastBufferFails(void)
{
#ifdef UNITY_EXCLUDE_STDLIB_MALLOC
    void* m = calloc(1, UNITY_INTERNAL_HEAP_SIZE_BYTES/2 + 1);
    void* n = calloc(1, UNITY_INTERNAL_HEAP_SIZE_BYTES/2);
    free(m);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_NULL(n);
    TEST_ASSERT_MEMORY_ALL_FREE_LIFO_ORDER(m, n);
#else
    TEST_IGNORE_MESSAGE("Enable UNITY_EXCLUDE_STDLIB_MALLOC to Run This Test");
#endif
}

void test_MallocThenReallocGrowsMemoryInPlace(void)
{
#ifdef UNITY_EXCLUDE_STDLIB_MALLOC
    void* m = malloc(UNITY_INTERNAL_HEAP_SIZE_BYTES/2 + 1);
    void* n = realloc(m, UNITY_INTERNAL_HEAP_SIZE_BYTES/2 + 9);
    free(n);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL(m, n);
    TEST_ASSERT_MEMORY_ALL_FREE_LIFO_ORDER(m, n);
#else
    TEST_IGNORE_MESSAGE("Enable UNITY_EXCLUDE_STDLIB_MALLOC to Run This Test");
#endif
}

void test_ReallocFailDoesNotFreeMem(void)
{
#ifdef UNITY_EXCLUDE_STDLIB_MALLOC
    void* m = malloc(UNITY_INTERNAL_HEAP_SIZE_BYTES/2);
    void* n1 = malloc(10);
    void* out_of_mem = realloc(n1, UNITY_INTERNAL_HEAP_SIZE_BYTES/2 + 1);
    void* n2 = malloc(10);

    free(n2);
    if (out_of_mem == NULL) free(n1);
    free(m);

    TEST_ASSERT_NOT_NULL(m);       /* Got a real memory location */
    TEST_ASSERT_NULL(out_of_mem);  /* The realloc should have failed */
    TEST_ASSERT_NOT_EQUAL(n2, n1); /* If n1 != n2 then realloc did not free n1 */
    TEST_ASSERT_MEMORY_ALL_FREE_LIFO_ORDER(m, n2);
#else
    TEST_IGNORE_MESSAGE("Enable UNITY_EXCLUDE_STDLIB_MALLOC to Run This Test");
#endif
}
