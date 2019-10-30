/* ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

#include "unity.h"
#include "unity_memory.h"

extern void test_ForceMallocFail(void);
extern void test_ReallocSmallerIsUnchanged(void);
extern void test_ReallocSameIsUnchanged(void);
extern void test_ReallocLargerNeeded(void);
extern void test_ReallocNullPointerIsLikeMalloc(void);
extern void test_ReallocSizeZeroFreesMemAndReturnsNullPointer(void);
extern void test_CallocFillsWithZero(void);
extern void test_FreeNULLSafety(void);
extern void test_DetectsLeak(void);
extern void test_BufferOverrunFoundDuringFree(void);
extern void test_BufferOverrunFoundDuringRealloc(void);
extern void test_BufferGuardWriteFoundDuringFree(void);
extern void test_BufferGuardWriteFoundDuringRealloc(void);
extern void test_MallocPastBufferFails(void);
extern void test_CallocPastBufferFails(void);
extern void test_MallocThenReallocGrowsMemoryInPlace(void);
extern void test_ReallocFailDoesNotFreeMem(void);

int main(void)
{
    UnityBegin("unity_memory_Test.c");
    RUN_TEST(test_ForceMallocFail);
    RUN_TEST(test_ReallocSmallerIsUnchanged);
    RUN_TEST(test_ReallocSameIsUnchanged);
    RUN_TEST(test_ReallocLargerNeeded);
    RUN_TEST(test_ReallocNullPointerIsLikeMalloc);
    RUN_TEST(test_ReallocSizeZeroFreesMemAndReturnsNullPointer);
    RUN_TEST(test_CallocFillsWithZero);
    RUN_TEST(test_FreeNULLSafety);
    RUN_TEST(test_DetectsLeak);
    RUN_TEST(test_BufferOverrunFoundDuringFree);
    RUN_TEST(test_BufferOverrunFoundDuringRealloc);
    RUN_TEST(test_BufferGuardWriteFoundDuringFree);
    RUN_TEST(test_BufferGuardWriteFoundDuringRealloc);
    RUN_TEST(test_MallocPastBufferFails);
    RUN_TEST(test_CallocPastBufferFails);
    RUN_TEST(test_MallocThenReallocGrowsMemoryInPlace);
    RUN_TEST(test_ReallocFailDoesNotFreeMem);
    return UnityEnd();
}
