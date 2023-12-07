#include "unity_fixture.h"

typedef void (*testcase)(void);

#define SECTION(x) __attribute__((used,section(x)))

#undef TEST_GROUP

#define RUN_ALL_TEST(func)                                                             \
void __test_group_start__(){}                                                          \
void __test_group_end__(){}                                                            \
const testcase test_group_start SECTION("testgroup.0") = __test_group_start__;         \
const testcase test_group_end SECTION("testgroup.2") = __test_group_end__;             \
void func()                                                                            \
{                                                                                      \
	for(const testcase *start = &test_group_start;start!=&test_group_end;start++)      \
	{                                                                                  \
		(*start)();                                                                    \
	}                                                                                  \
}

#undef TEST_SETUP
#define __TEST_SETUP__(group)                                                                     \
    void TEST_##group##_runner();                                                                 \
    const testcase test_##group##_runner SECTION("testgroup.1." #group) = TEST_##group##_runner;  \
    void TEST_##group##_start(){}                                                                 \
    void TEST_##group##_end(){}                                                                   \
    const testcase test_##group##_start SECTION("testcase." #group ".0") = TEST_##group##_start;  \
    const testcase test_##group##_end SECTION("testcase." #group ".2") = TEST_##group##_end;      \
    void TEST_##group##_runner()                                                                  \
    {                                                                                             \
        for(const testcase *start = &test_##group##_start;start != &test_##group##_end;start++)   \
        {                                                                                         \
            (*start)();                                                                           \
        }                                                                                         \
    }                                                                                             \
    void TEST_##group##_SETUP(void);                                                              \
    void TEST_##group##_SETUP(void)
		
#define TEST_SETUP_(group)	__TEST_SETUP__(group)
#define TEST_SETUP()		TEST_SETUP_(__GROUP__)

#undef TEST_TEAR_DOWN
#define __TEST_TEAR_DOWN__(group) void TEST_##group##_TEAR_DOWN(void);\
    void TEST_##group##_TEAR_DOWN(void)
		
#define TEST_TEAR_DOWN_(group)	__TEST_TEAR_DOWN__(group)
#define TEST_TEAR_DOWN()		TEST_TEAR_DOWN_(__GROUP__)

#undef TEST
#define __TEST__(group, name)                                                                                           \
    void TEST_##group##_##name##_(void);                                                                                \
    void TEST_##group##_##name##_run(void);                                                                             \
	const testcase test_##group##_##name##_run SECTION("testcase." #group ".1." #name) = TEST_##group##_##name##_run;   \
    void TEST_##group##_##name##_run(void)                                                                              \
    {                                                                                                                   \
        UnityTestRunner(TEST_##group##_SETUP,                                                                           \
            TEST_##group##_##name##_,                                                                                   \
            TEST_##group##_TEAR_DOWN,                                                                                   \
            "TEST(" #group ", " #name ")",#group, #name,                                                                \
            __FILE__, __LINE__);                                                                                        \
    }                                                                                                                   \
    void  TEST_##group##_##name##_(void)

#define TEST_(group,name)	__TEST__(group, name)
#define TEST(name)			TEST_(__GROUP__,name)
	
#undef IGNORE_TEST
#define __IGNORE_TEST__(group, name)                                                \
    void TEST_##group##_##name##_(void);                                            \
    void TEST_##group##_##name##_run(void);                                         \
    void TEST_##group##_##name##_run(void)                                          \
    {                                                                               \
        UnityIgnoreTest("IGNORE_TEST(" #group ", " #name ")", #group, #name);       \
    }                                                                               \
    void TEST_##group##_##name##_(void)

#define IGNORE_TEST_(group,name)	__IGNORE_TEST__(group, name)
#define IGNORE_TEST(name)			IGNORE_TEST_(__GROUP__,name)
