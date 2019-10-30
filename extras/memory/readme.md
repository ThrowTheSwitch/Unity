# Unity Memory

This Framework is an optional add-on to Unity.  By including unity.h and then
unity_memory.h, you have the added ability to track malloc and free calls. This 
addon requires that the stdlib functions be overridden by its own defines. These
defines will still malloc / realloc / free etc, but will also track the calls
in order to ensure that you don't have any memory leaks in your programs.

Note that this is only useful in situations where a unit is in charge of both
the allocation and deallocation of memory. When it is not symmetric, unit testing 
can report a number of false failures. A more advanced runtime tool is required to
track complete system memory handling.

# Module API

## `UnityMalloc_StartTest` and `UnityMalloc_EndTest`

These must be called at the beginning and end of each test. For simplicity, they can
be added to `setUp` and `tearDown` in order to do their job. When using the test
runner generator scripts, these will be automatically added to the runner whenever
unity_memory.h is included.

## `UnityMalloc_MakeMallocFailAfterCount`

This can be called from the tests themselves. Passing this function a number will
force the reference counter to start keeping track of malloc calls. During that test,
if the number of malloc calls exceeds the number given, malloc will immediately 
start returning `NULL`. This allows you to test error conditions. Think of it as a 
simplified mock.

# Configuration

## `UNITY_MALLOC` and `UNITY_FREE`

By default, this module tries to use the real stdlib `malloc` and `free` internally. 
If you would prefer it to use something else, like FreeRTOS's `pvPortMalloc` and 
`pvPortFree`, then you can use these defines to make it so.

## `UNITY_EXCLUDE_STDLIB_MALLOC`

If you would like this library to ignore stdlib or other heap engines completely, and
manage the memory on its own, then define this. All memory will be handled internally
(and at likely lower overhead). Note that this is not a very featureful memory manager,
but is sufficient for most testing purposes.

## `UNITY_INTERNAL_HEAP_SIZE_BYTES`

When using the built-in memory manager (see `UNITY_EXCLUDE_STDLIB_MALLOC`) this define
allows you to set the heap size this library will use to manage the memory.
