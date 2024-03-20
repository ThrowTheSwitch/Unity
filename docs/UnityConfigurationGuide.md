# Unity Configuration Guide

## C Standards, Compilers and Microcontrollers

The embedded software world contains its challenges.
Compilers support different revisions of the C Standard.
They ignore requirements in places, sometimes to make the language more usable in some special regard.
Sometimes it's to simplify their support.
Sometimes it's due to specific quirks of the microcontroller they are targeting.
Simulators add another dimension to this menagerie.

Unity is designed to run on almost anything that is targeted by a C compiler.
It would be awesome if this could be done with zero configuration.
While there are some targets that come close to this dream, it is sadly not universal.
It is likely that you are going to need at least a couple of the configuration options described in this document.

All of Unity's configuration options are `#defines`.
Most of these are simple definitions.
A couple are macros with arguments.
They live inside the unity_internals.h header file.
We don't necessarily recommend opening that file unless you really need to.
That file is proof that a cross-platform library is challenging to build.
From a more positive perspective, it is also proof that a great deal of complexity can be centralized primarily to one place to provide a more consistent and simple experience elsewhere.

### Using These Options

It doesn't matter if you're using a target-specific compiler and a simulator or a native compiler.
In either case, you've got a couple choices for configuring these options:

1. Because these options are specified via C defines, you can pass most of these options to your compiler through command line compiler flags. Even if you're using an embedded target that forces you to use their overbearing IDE for all configuration, there will be a place somewhere in your project to configure defines for your compiler.
2. You can create a custom `unity_config.h` configuration file (present in your toolchain's search paths).
  In this file, you will list definitions and macros specific to your target. All you must do is define `UNITY_INCLUDE_CONFIG_H` and Unity will rely on `unity_config.h` for any further definitions it may need.

Unfortunately, it doesn't usually work well to just #define these things in the test itself.
These defines need to take effect where ever unity.h is included.
This would be test test, the test runner (if you're generating one), and from unity.c when it's compiled.

## The Options

### Integer Types

If you've been a C developer for long, you probably already know that C's concept of an integer varies from target to target.
The C Standard has rules about the `int` matching the register size of the target microprocessor.
It has rules about the `int` and how its size relates to other integer types.
An `int` on one target might be 16 bits while on another target it might be 64.
There are more specific types in compilers compliant with C99 or later, but that's certainly not every compiler you are likely to encounter.
Therefore, Unity has a number of features for helping to adjust itself to match your required integer sizes.
It starts off by trying to do it automatically.

#### `UNITY_EXCLUDE_STDINT_H`

The first thing that Unity does to guess your types is check `stdint.h`.
This file includes defines like `UINT_MAX` that Unity can use to learn a lot about your system.
It's possible you don't want it to do this (um. why not?) or (more likely) it's possible that your system doesn't support `stdint.h`.
If that's the case, you're going to want to define this.
That way, Unity will know to skip the inclusion of this file and you won't be left with a compiler error.

_Example:_

```C
#define UNITY_EXCLUDE_STDINT_H
```

#### `UNITY_EXCLUDE_LIMITS_H`

The second attempt to guess your types is to check `limits.h`.
Some compilers that don't support `stdint.h` could include `limits.h` instead.
If you don't want Unity to check this file either, define this to make it skip the inclusion.

_Example:_

```C
#define UNITY_EXCLUDE_LIMITS_H
```

If you've disabled both of the automatic options above, you're going to have to do the configuration yourself.
Don't worry.
Even this isn't too bad... there are just a handful of defines that you are going to specify if you don't like the defaults.

#### `UNITY_INT_WIDTH`

Define this to be the number of bits an `int` takes up on your system.
The default, if not autodetected, is 32 bits.

_Example:_

```C
#define UNITY_INT_WIDTH 16
```

#### `UNITY_LONG_WIDTH`

Define this to be the number of bits a `long` takes up on your system.
The default, if not autodetected, is 32 bits.
This is used to figure out what kind of 64-bit support your system can handle.
Does it need to specify a `long` or a `long long` to get a 64-bit value.
On 16-bit systems, this option is going to be ignored.

_Example:_

```C
#define UNITY_LONG_WIDTH 16
```

#### `UNITY_POINTER_WIDTH`

Define this to be the number of bits a pointer takes up on your system.
The default, if not autodetected, is 32-bits.
If you're getting ugly compiler warnings about casting from pointers, this is the one to look at.

_Hint:_ In order to support exotic processors (for example TI C55x with a pointer width of 23-bit), choose the next power of two (in this case 32-bit).

_Supported values:_ 16, 32 and 64

_Example:_

```C
// Choose on of these #defines to set your pointer width (if not autodetected)
//#define UNITY_POINTER_WIDTH 16
//#define UNITY_POINTER_WIDTH 32
#define UNITY_POINTER_WIDTH 64 // Set UNITY_POINTER_WIDTH to 64-bit
```

#### `UNITY_COMPARE_PTRS_ON_ZERO_ARRAY`

Define this to make all array assertions compare pointers instead of contents when a length of zero is specified. When not enabled,
defining a length of zero will always result in a failure and a message warning the user that they have tried to compare empty
arrays.

#### `UNITY_SUPPORT_64`

Unity will automatically include 64-bit support if it auto-detects it, or if your `int`, `long`, or pointer widths are greater than 32-bits.
Define this to enable 64-bit support if none of the other options already did it for you.
There can be a significant size and speed impact to enabling 64-bit support on small targets, so don't define it if you don't need it.

_Example:_

```C
#define UNITY_SUPPORT_64
```

### Floating Point Types

In the embedded world, it's not uncommon for targets to have no support for floating point operations at all or to have support that is limited to only single precision.
We are able to guess integer sizes on the fly because integers are always available in at least one size.
Floating point, on the other hand, is sometimes not available at all.
Trying to include `float.h` on these platforms would result in an error. This leaves manual configuration as the only option.

#### `UNITY_INCLUDE_FLOAT`

#### `UNITY_EXCLUDE_FLOAT`

#### `UNITY_INCLUDE_DOUBLE`

#### `UNITY_EXCLUDE_DOUBLE`

By default, Unity guesses that you will want single precision floating point support, but not double precision.
It's easy to change either of these using the include and exclude options here.
You may include neither, either, or both, as suits your needs.
For features that are enabled, the following floating point options also become available.

_Example:_

```C
//what manner of strange processor is this?
#define UNITY_EXCLUDE_FLOAT
#define UNITY_INCLUDE_DOUBLE
```

#### `UNITY_EXCLUDE_FLOAT_PRINT`

Unity aims for as small of a footprint as possible and avoids most standard library calls (some embedded platforms don’t have a standard library!).
Because of this, its routines for printing integer values are minimalist and hand-coded.
Therefore, the display of floating point values during a failure are optional.
By default, Unity will print the actual results of floating point assertion failure (e.g. ”Expected 4.56 Was 4.68”).
To not include this extra support, you can use this define to instead respond to a failed assertion with a message like ”Values Not Within Delta”.
If you would like verbose failure messages for floating point assertions, use these options to give more explicit failure messages.

_Example:_

```C
#define UNITY_EXCLUDE_FLOAT_PRINT
```

#### `UNITY_FLOAT_TYPE`

If enabled, Unity assumes you want your `FLOAT` asserts to compare standard C floats.
If your compiler supports a specialty floating point type, you can always override this behavior by using this definition.

_Example:_

```C
#define UNITY_FLOAT_TYPE float16_t
```

#### `UNITY_DOUBLE_TYPE`

If enabled, Unity assumes you want your `DOUBLE` asserts to compare standard C doubles.
If you would like to change this, you can specify something else by using this option.
For example, defining `UNITY_DOUBLE_TYPE` to `long double` could enable gargantuan floating point types on your 64-bit processor instead of the standard `double`.

_Example:_

```C
#define UNITY_DOUBLE_TYPE long double
```

#### `UNITY_FLOAT_PRECISION`

#### `UNITY_DOUBLE_PRECISION`

If you look up `UNITY_ASSERT_EQUAL_FLOAT` and `UNITY_ASSERT_EQUAL_DOUBLE` as documented in the big daddy Unity Assertion Guide, you will learn that they are not really asserting that two values are equal but rather that two values are "close enough" to equal.
"Close enough" is controlled by these precision configuration options.
If you are working with 32-bit floats and/or 64-bit doubles (the normal on most processors), you should have no need to change these options.
They are both set to give you approximately 1 significant bit in either direction.
The float precision is 0.00001 while the double is 10-12.
For further details on how this works, see the appendix of the Unity Assertion Guide.

_Example:_

```C
#define UNITY_FLOAT_PRECISION 0.001f
```

#### `UNITY_IS_NAN` and `UNITY_IS_INF`

If your toolchain defines `isnan` and `isinf` in `math.h` as macros, nothing needs to be done. If your toolchain doesn't define these, Unity
will create these macros itself. You may override either or both of these defines to specify how you want to evaluate if a number is NaN or Infinity.

_Example:_

```C
#define UNITY_IS_NAN(n) ((n != n) ? 1 : 0)
```

### Miscellaneous

#### `UNITY_EXCLUDE_STDDEF_H`

Unity uses the `NULL` macro, which defines the value of a null pointer constant, defined in `stddef.h` by default.
If you want to provide your own macro for this, you should exclude the `stddef.h` header file by adding this define to your configuration.

_Example:_

```C
#define UNITY_EXCLUDE_STDDEF_H
```

#### `UNITY_INCLUDE_PRINT_FORMATTED`

Unity provides a simple (and very basic) printf-like string output implementation, which is able to print a string modified by the following format string modifiers:

- __%d__ - signed value (decimal)
- __%i__ - same as __%d__
- __%u__ - unsigned value (decimal)
- __%f__ - float/Double (if float support is activated)
- __%g__ - same as __%f__
- __%b__ - binary prefixed with "0b"
- __%x__ - hexadecimal (upper case) prefixed with "0x"
- __%X__ - same as __%x__
- __%p__ - pointer (same as __%x__ or __%X__)
- __%c__ - a single character
- __%s__ - a string (e.g. "string")
- __%%__ - The "%" symbol (escaped)

Length specifiers are also supported. If you are using long long types, make sure UNITY_SUPPORT_64 is true to ensure they are printed correctly.

- __%ld__ - signed long value (decimal)
- __%lld__ - signed long long value (decimal)
- __%lu__ - unsigned long value (decimal)
- __%llu__ - unsigned long long value (decimal)
- __%lx__ - unsigned long value (hexadecimal)
- __%llx__ - unsigned long long value (hexadecimal)

_Example:_

```C
#define UNITY_INCLUDE_PRINT_FORMATTED

int a = 0xfab1;
TEST_PRINTF("Decimal   %d\n", -7);
TEST_PRINTF("Unsigned  %u\n", 987);
TEST_PRINTF("Float     %f\n", 3.1415926535897932384);
TEST_PRINTF("Binary    %b\n", 0xA);
TEST_PRINTF("Hex       %X\n", 0xFAB);
TEST_PRINTF("Pointer   %p\n", &a);
TEST_PRINTF("Character %c\n", 'F');
TEST_PRINTF("String    %s\n", "My string");
TEST_PRINTF("Percent   %%\n");
TEST_PRINTF("Unsigned long long %llu\n", 922337203685477580);
TEST_PRINTF("Color Red \033[41mFAIL\033[0m\n");
TEST_PRINTF("\n");
TEST_PRINTF("Multiple (%d) (%i) (%u) (%x)\n", -100, 0, 200, 0x12345);
```

### Toolset Customization

In addition to the options listed above, there are a number of other options which will come in handy to customize Unity's behavior for your specific toolchain.
It is possible that you may not need to touch any of these... but certain platforms, particularly those running in simulators, may need to jump through extra hoops to run properly.
These macros will help in those situations.

#### `UNITY_OUTPUT_CHAR(a)`

#### `UNITY_OUTPUT_FLUSH()`

#### `UNITY_OUTPUT_START()`

#### `UNITY_OUTPUT_COMPLETE()`

By default, Unity prints its results to `stdout` as it runs.
This works perfectly fine in most situations where you are using a native compiler for testing.
It works on some simulators as well so long as they have `stdout` routed back to the command line.
There are times, however, where the simulator will lack support for dumping results or you will want to route results elsewhere for other reasons.
In these cases, you should define the `UNITY_OUTPUT_CHAR` macro.
This macro accepts a single character at a time (as an `int`, since this is the parameter type of the standard C `putchar` function most commonly used).
You may replace this with whatever function call you like.

_Example:_
Say you are forced to run your test suite on an embedded processor with no `stdout` option.
You decide to route your test result output to a custom serial `RS232_putc()` function you wrote like thus:

```C
#include "RS232_header.h"
...
#define UNITY_OUTPUT_CHAR(a)    RS232_putc(a)
#define UNITY_OUTPUT_START()    RS232_config(115200,1,8,0)
#define UNITY_OUTPUT_FLUSH()    RS232_flush()
#define UNITY_OUTPUT_COMPLETE() RS232_close()
```

_Note:_
`UNITY_OUTPUT_FLUSH()` can be set to the standard out flush function simply by specifying `UNITY_USE_FLUSH_STDOUT`.
No other defines are required.

#### `UNITY_OUTPUT_FOR_ECLIPSE`

#### `UNITY_OUTPUT_FOR_IAR_WORKBENCH`

#### `UNITY_OUTPUT_FOR_QT_CREATOR`

When managing your own builds, it is often handy to have messages output in a format which is recognized by your IDE.
These are some standard formats which can be supported.
If you're using Ceedling to manage your builds, it is better to stick with the standard format (leaving these all undefined) and allow Ceedling to use its own decorators.

#### `UNITY_PTR_ATTRIBUTE`

Some compilers require a custom attribute to be assigned to pointers, like `near` or `far`.
In these cases, you can give Unity a safe default for these by defining this option with the attribute you would like.

_Example:_

```C
#define UNITY_PTR_ATTRIBUTE __attribute__((far))
#define UNITY_PTR_ATTRIBUTE near
```

#### `UNITY_PRINT_EOL`

By default, Unity outputs \n at the end of each line of output.
This is easy to parse by the scripts, by Ceedling, etc, but it might not be ideal for YOUR system.
Feel free to override this and to make it whatever you wish.

_Example:_

```C
#define UNITY_PRINT_EOL { UNITY_OUTPUT_CHAR('\r'); UNITY_OUTPUT_CHAR('\n'); }
```

#### `UNITY_EXCLUDE_DETAILS`

This is an option for if you absolutely must squeeze every byte of memory out of your system.
Unity stores a set of internal scratchpads which are used to pass extra detail information around.
It's used by systems like CMock in order to report which function or argument flagged an error.
If you're not using CMock and you're not using these details for other things, then you can exclude them.

_Example:_

```C
#define UNITY_EXCLUDE_DETAILS
```

#### `UNITY_PRINT_TEST_CONTEXT`

This option allows you to specify your own function to print additional context as part of the error message when a test has failed.
It can be useful if you want to output some specific information about the state of the test at the point of failure, and `UNITY_SET_DETAILS` isn't flexible enough for your needs.

_Example:_

```C
#define UNITY_PRINT_TEST_CONTEXT PrintIterationCount

extern int iteration_count;

void PrintIterationCount(void)
{
  UnityPrintFormatted("At iteration #%d: ", iteration_count);
}
```

#### `UNITY_EXCLUDE_SETJMP`

If your embedded system doesn't support the standard library setjmp, you can exclude Unity's reliance on this by using this define.
This dropped dependence comes at a price, though.
You will be unable to use custom helper functions for your tests, and you will be unable to use tools like CMock.
Very likely, if your compiler doesn't support setjmp, you wouldn't have had the memory space for those things anyway, though... so this option exists for those situations.

_Example:_

```C
#define UNITY_EXCLUDE_SETJMP
```

#### `UNITY_TEST_PROTECT`

#### `UNITY_TEST_ABORT`

Unity handles test failures via `setjmp`/`longjmp` pair by default. As mentioned above, you can disable this with `UNITY_EXCLUDE_SETJMP`. You can also customise what happens on every `TEST_PROTECT` and `TEST_ABORT` call. This can be accomplished by providing user-defined `UNITY_TEST_PROTECT` and `UNITY_TEST_ABORT` macros (and these may be defined independently).

`UNITY_TEST_PROTECT` is used as an `if` statement expression, and has to evaluate to `true` on the first call (when saving stack environment with `setjmp`), and to `false` when it returns as a result of a `TEST_ABORT` (when restoring the stack environment with `longjmp`).

Whenever an assert macro fails, `TEST_ABORT` is used to restore the stack environment previously set by `TEST_PROTECT`. This part may be overriden with `UNITY_TEST_ABORT`, e.g. if custom failure handling is needed.

_Example 1:_

Calling `longjmp` on your target is possible, but has a platform-specific (or implementation-specific) set of prerequisites, e.g. privileged access level. You can extend the default behaviour of `TEST_PROTECT` and `TEST_ABORT` as:

`unity_config.h`:

```C
#include "my_custom_test_handlers.h"

#define UNITY_TEST_PROTECT() custom_test_protect()
#define UNITY_TEST_ABORT()   custom_test_abort()
```

`my_custom_test_handlers.c`:

```C
int custom_test_protect(void) {
  platform_specific_code();
  return setjmp(Unity.AbortFrame) == 0;
}

UNITY_NORETURN void custom_test_abort(void) {
  more_platform_specific_code();
  longjmp(Unity.AbortFrame, 1);
}
```

_Example 2:_

Unity is used to provide the assertion macros only, and an external test harness/runner is used for test orchestration/reporting. In this case you can easily plug your code by overriding `TEST_ABORT` as:

`unity_config.h`:

```C
#include "my_custom_test_handlers.h"

#define UNITY_TEST_PROTECT() 1
#define UNITY_TEST_ABORT()   custom_test_abort()
```

`my_custom_test_handlers.c`:

```C
void custom_test_abort(void) {
  if (Unity.CurrentTestFailed == 1) {
    custom_failed_test_handler();
  } else if (Unity.CurrentTestIgnored == 1) {
    custom_ignored_test_handler();
  }
}
```

#### `UNITY_OUTPUT_COLOR`

If you want to add color using ANSI escape codes you can use this define.

_Example:_

```C
#define UNITY_OUTPUT_COLOR
```

#### `UNITY_INCLUDE_EXEC_TIME`

Define this to measure and report execution time for each test in the suite. When enabled, Unity will do
it's best to automatically find a way to determine the time in milliseconds. On most Windows, macos, or 
Linux environments, this is automatic. If not, you can give Unity more information.

#### `UNITY_CLOCK_MS`

If you're working on a system (embedded or otherwise) which has an accessible millisecond timer. You can
define `UNITY_CLOCK_MS` to be the name of the function which returns the millisecond timer. It will then
attempt to use that function for timing purposes.

#### `UNITY_EXEC_TIME_START`

Define this hook to start a millisecond timer if necessary.

#### `UNITY_EXEC_TIME_STOP`

Define this hook to stop a millisecond timer if necessary.

#### `UNITY_PRINT_EXEC_TIME`

Define this hook to print the current execution time. Used to report the milliseconds elapsed.

#### `UNITY_TIME_TYPE`

Finally, this can be set to the type which holds the millisecond timer.

#### `UNITY_SHORTHAND_AS_INT`

#### `UNITY_SHORTHAND_AS_MEM`

#### `UNITY_SHORTHAND_AS_RAW`

#### `UNITY_SHORTHAND_AS_NONE`

These options give you control of the `TEST_ASSERT_EQUAL` and the `TEST_ASSERT_NOT_EQUAL` shorthand assertions.
Historically, Unity treated the former as an alias for an integer comparison.
It treated the latter as a direct comparison using `!=`.
This asymmetry was confusing, but there was much disagreement as to how best to treat this pair of assertions.
These four options will allow you to specify how Unity will treat these assertions.

- AS INT - the values will be cast to integers and directly compared.
  Arguments that don't cast easily to integers will cause compiler errors.
- AS MEM - the address of both values will be taken and the entire object's memory footprint will be compared byte by byte.
  Directly placing constant numbers like `456` as expected values will cause errors.
- AS_RAW - Unity assumes that you can compare the two values using `==` and `!=` and will do so.
  No details are given about mismatches, because it doesn't really know what type it's dealing with.
- AS_NONE - Unity will disallow the use of these shorthand macros altogether, insisting that developers choose a more descriptive option.

#### `UNITY_SUPPORT_VARIADIC_MACROS`

This will force Unity to support variadic macros when using its own built-in RUN_TEST macro.
This will rarely be necessary. Most often, Unity will automatically detect if the compiler supports variadic macros by checking to see if it's C99+ compatible.
In the event that the compiler supports variadic macros, but is primarily C89 (ANSI), defining this option will allow you to use them.
This option is also not necessary when using Ceedling or the test runner generator script.

#### `UNITY_SUPPORT_TEST_CASES`

Unity can automatically define all supported parameterized tests macros.
That feature is disabled by default.
To enable it, use the following example:

```C
#define UNITY_SUPPORT_TEST_CASES
```

You can manually provide required `TEST_CASE`, `TEST_RANGE` or `TEST_MATRIX` macro definitions
before including `unity.h`, and they won't be redefined.
If you provide one of the following macros, some of default definitions will not be
defined:
| User defines macro | Unity will _not_ define following macro |
|---|---|
| `UNITY_EXCLUDE_TEST_CASE` | `TEST_CASE` |
| `UNITY_EXCLUDE_TEST_RANGE` | `TEST_RANGE` |
| `UNITY_EXCLUDE_TEST_MATRIX` | `TEST_MATRIX` |
| `TEST_CASE` | `TEST_CASE` |
| `TEST_RANGE` | `TEST_RANGE` |
| `TEST_MATRIX` | `TEST_MATRIX` |

`UNITY_EXCLUDE_TEST_*` defines is not processed by test runner generator script.
If you exclude one of them from definition, you should provide your own definition
for them or avoid using undefined `TEST_*` macro as a test generator.
Otherwise, compiler cannot build source code file with provided call.

_Note:_
That feature requires variadic macro support by compiler. If required feature
is not detected, it will not be enabled, even though preprocessor macro is defined.

## Getting Into The Guts

There will be cases where the options above aren't quite going to get everything perfect.
They are likely sufficient for any situation where you are compiling and executing your tests with a native toolchain (e.g. clang on Mac).
These options may even get you through the majority of cases encountered in working with a target simulator run from your local command line.
But especially if you must run your test suite on your target hardware, your Unity configuration will
require special help.
This special help will usually reside in one of two places: the `main()` function or the `RUN_TEST` macro.
Let's look at how these work.

### `main()`

Each test module is compiled and run on its own, separate from the other test files in your project.
Each test file, therefore, has a `main` function.
This `main` function will need to contain whatever code is necessary to initialize your system to a workable state.
This is particularly true for situations where you must set up a memory map or initialize a communication channel for the output of your test results.

A simple main function looks something like this:

```C
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_TheFirst);
    RUN_TEST(test_TheSecond);
    RUN_TEST(test_TheThird);
    return UNITY_END();
}
```

You can see that our main function doesn't bother taking any arguments.
For our most barebones case, we'll never have arguments because we just run all the tests each time.
Instead, we start by calling `UNITY_BEGIN`.
We run each test (in whatever order we wish).
Finally, we call `UNITY_END`, returning its return value (which is the total number of failures).

It should be easy to see that you can add code before any test cases are run or after all the test cases have completed.
This allows you to do any needed system-wide setup or teardown that might be required for your special circumstances.

#### `RUN_TEST`

The `RUN_TEST` macro is called with each test case function.
Its job is to perform whatever setup and teardown is necessary for executing a single test case function.
This includes catching failures, calling the test module's `setUp()` and `tearDown()` functions, and calling `UnityConcludeTest()`.
If using CMock or test coverage, there will be additional stubs in use here.
A simple minimalist RUN_TEST macro looks something like this:

```C
#define RUN_TEST(testfunc) \
    UNITY_NEW_TEST(#testfunc) \
    if (TEST_PROTECT()) { \
        setUp(); \
        testfunc(); \
    } \
    if (TEST_PROTECT() && (!TEST_IS_IGNORED)) \
        tearDown(); \
    UnityConcludeTest();
```

So that's quite a macro, huh?
It gives you a glimpse of what kind of stuff Unity has to deal with for every single test case.
For each test case, we declare that it is a new test.
Then we run `setUp` and our test function.
These are run within a `TEST_PROTECT` block, the function of which is to handle failures that occur during the test.
Then, assuming our test is still running and hasn't been ignored, we run `tearDown`.
No matter what, our last step is to conclude this test before moving on to the next.

Let's say you need to add a call to `fsync` to force all of your output data to flush to a file after each test.
You could easily insert this after your `UnityConcludeTest` call.
Maybe you want to write an xml tag before and after each result set.
Again, you could do this by adding lines to this macro.
Updates to this macro are for the occasions when you need an action before or after every single test case throughout your entire suite of tests.

## Happy Porting

The defines and macros in this guide should help you port Unity to just about any C target we can imagine.
If you run into a snag or two, don't be afraid of asking for help on the forums.
We love a good challenge!

*Find The Latest of This And More at [ThrowTheSwitch.org][]*

[ThrowTheSwitch.org]: https://throwtheswitch.org
