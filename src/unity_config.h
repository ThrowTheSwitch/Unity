/* ==========================================
	Unity Project - A Test Framework for C
	Copyright (c) 2007-21 Mike Karlesky, Mark VanderVoord, Greg Williams, David Koch
	[Released under MIT License. Please refer to license.txt for details]
========================================== */

/* Unity Configuration
 * As of May 11th, 2016 at ThrowTheSwitch/Unity commit 837c529
 * Update: December 29th, 2016
 * See Also: Unity/docs/UnityConfigurationGuide.pdf
 *
 * Unity is designed to run on almost anything that is targeted by a C compiler.
 * It would be awesome if this could be done with zero configuration. While
 * there are some targets that come close to this dream, it is sadly not
 * universal. It is likely that you are going to need at least a couple of the
 * configuration options described in this document.
 *
 * All of Unity's configuration options are `#defines`. Most of these are simple
 * definitions. A couple are macros with arguments. They live inside the
 * unity_internals.h header file. We don't necessarily recommend opening that
 * file unless you really need to. That file is proof that a cross-platform
 * library is challenging to build. From a more positive perspective, it is also
 * proof that a great deal of complexity can be centralized primarily to one
 * place in order to provide a more consistent and simple experience elsewhere.
 *
 * Using These Options
 * It doesn't matter if you're using a target-specific compiler and a simulator
 * or a native compiler. In either case, you've got a couple choices for
 * configuring these options:
 *
 *  1. Because these options are specified via C defines, you can pass most of
 *     these options to your compiler through command line compiler flags. Even
 *     if you're using an embedded target that forces you to use their
 *     overbearing IDE for all configuration, there will be a place somewhere in
 *     your project to configure defines for your compiler.
 *  2. You can create a custom `unity_config.h` configuration file (present in
 *     your toolchain's search paths). In this file, you will list definitions
 *     and macros specific to your target. All you must do is define
 *     `UNITY_INCLUDE_CONFIG_H` and Unity will rely on `unity_config.h` for any
 *     further definitions it may need.
 */

#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

/* ************************* AUTOMATIC INTEGER TYPES ***************************
 * C's concept of an integer varies from target to target. The C Standard has
 * rules about the `int` matching the register size of the target
 * microprocessor. It has rules about the `int` and how its size relates to
 * other integer types. An `int` on one target might be 16 bits while on another
 * target it might be 64. There are more specific types in compilers compliant
 * with C99 or later, but that's certainly not every compiler you are likely to
 * encounter. Therefore, Unity has a number of features for helping to adjust
 * itself to match your required integer sizes. It starts off by trying to do it
 * automatically.
 **************************************************************************** */

/* The first attempt to guess your types is to check `limits.h`. Some compilers
 * that don't support `stdint.h` could include `limits.h`. If you don't
 * want Unity to check this file, define this to make it skip the inclusion.
 * Unity looks at UINT_MAX & ULONG_MAX, which were available since C89.
 */
#if 0
	#define UNITY_EXCLUDE_LIMITS_H
#endif

/* The second thing that Unity does to guess your types is check `stdint.h`.
 * This file defines `UINTPTR_MAX`, since C99, that Unity can make use of to
 * learn about your system. It's possible you don't want it to do this or it's
 * possible that your system doesn't support `stdint.h`. If that's the case,
 * you're going to want to define this. That way, Unity will know to skip the
 * inclusion of this file and you won't be left with a compiler error.
 */
#if 0
	#define UNITY_EXCLUDE_STDINT_H
#endif


/* ********************** MANUAL INTEGER TYPE DEFINITION ***********************
 * If you've disabled all of the automatic options above, you're going to have
 * to do the configuration yourself. There are just a handful of defines that
 * you are going to specify if you don't like the defaults.
 **************************************************************************** */

 /* Define this to be the number of bits an `int` takes up on your system. The
 * default, if not auto-detected, is 32 bits.
 *
 * Example:
 */
#if 0
	#define UNITY_INT_WIDTH		16
#endif

/* Define this to be the number of bits a `long` takes up on your system. The
 * default, if not autodetected, is 32 bits. This is used to figure out what
 * kind of 64-bit support your system can handle.  Does it need to specify a
 * `long` or a `long long` to get a 64-bit value. On 16-bit systems, this option
 * is going to be ignored.
 *
 * Example:
 */
#if 0
	#define UNITY_LONG_WIDTH	16
#endif

/* Define this to be the number of bits a pointer takes up on your system. The
 * default, if not autodetected, is 32-bits. If you're getting ugly compiler
 * warnings about casting from pointers, this is the one to look at.
 *
 * Example:
 */
#if 0
	#define UNITY_POINTER_WIDTH	64
#endif

/* Unity will automatically include 64-bit support if it auto-detects it, or if
 * your `int`, `long`, or pointer widths are greater than 32-bits. Define this
 * to enable 64-bit support if none of the other options already did it for you.
 * There can be a significant size and speed impact to enabling 64-bit support
 * on small targets, so don't define it if you don't need it.
 */
#if 0
	#define UNITY_INCLUDE_64
#endif


/* *************************** FLOATING POINT TYPES ****************************
 * In the embedded world, it's not uncommon for targets to have no support for
 * floating point operations at all or to have support that is limited to only
 * single precision. We are able to guess integer sizes on the fly because
 * integers are always available in at least one size. Floating point, on the
 * other hand, is sometimes not available at all. Trying to include `float.h` on
 * these platforms would result in an error. This leaves manual configuration as
 * the only option.
 **************************************************************************** */

 /* By default, Unity guesses that you will want single precision floating point
  * support, but not double precision. It's easy to change either of these using
  * the include and exclude options here. You may include neither, just float,
  * or both, as suits your needs.
  */
#if 0
	#define UNITY_EXCLUDE_FLOAT
#endif
#if 0
	#define UNITY_INCLUDE_DOUBLE
#endif
#if 0
	#define UNITY_EXCLUDE_DOUBLE
#endif

/* For features that are enabled, the following floating point options also
 * become available.
 */

/* Unity aims for as small of a footprint as possible and avoids most standard
 * library calls (some embedded platforms don't have a standard library!).
 * Because of this, its routines for printing integer values are minimalist and
 * hand-coded. To keep Unity universal, though, we eventually chose to develop
 * our own floating point print routines. Still, the display of floating point
 * values during a failure are optional. By default, Unity will print the
 * actual results of floating point assertion failures. So a failed assertion
 * will produce a message like "Expected 4.0 Was 4.25". If you would like less
 * verbose failure messages for floating point assertions, use this option to
 * give a failure message `"Values Not Within Delta"` and trim the binary size.
 */
#if 0
	#define UNITY_EXCLUDE_FLOAT_PRINT
#endif

/* If enabled, Unity assumes you want your `FLOAT` asserts to compare standard C
 * floats. If your compiler supports a specialty floating point type, you can
 * always override this behavior by using this definition.
 *
 * Example:
 */
#if 0
	#define UNITY_FLOAT_TYPE	float16_t
#endif

/* If enabled, Unity assumes you want your `DOUBLE` asserts to compare standard
 * C doubles. If you would like to change this, you can specify something else
 * by using this option. For example, defining `UNITY_DOUBLE_TYPE` to `long
 * double` could enable gargantuan floating point types on your 64-bit processor
 * instead of the standard `double`.
 *
 * Example:
 */
#if 0
	#define UNITY_DOUBLE_TYPE	long double
#endif

/* If you look up `UNITY_ASSERT__EQUAL__FLOAT` and `UNITY_ASSERT__EQUAL__DOUBLE` as
 * documented in the Unity Assertion Guide, you will learn that they are not
 * really asserting that two values are equal but rather that two values are
 * "close enough" to equal. "Close enough" is controlled by these precision
 * configuration options. If you are working with 32-bit floats and/or 64-bit
 * doubles (the normal on most processors), you should have no need to change
 * these options. They are both set to give you approximately 1 significant bit
 * in either direction. The float precision is 0.00001 while the double is
 * 10^-12. For further details on how this works, see the appendix of the Unity
 * Assertion Guide.
 *
 * Example:
 */
#define UNITY_FLOAT_PRECISION	0.001f
#define UNITY_DOUBLE_PRECISION	0.001f


/* *************************** MISCELLANEOUS ***********************************
 * Miscellaneous configuration options for Unity
 **************************************************************************** */

/* Unity uses the stddef.h header included in the C standard library for the
 * "NULL" macro. Define this in order to disable the include of stddef.h. If you
 * do this, you have to make sure to provide your own "NULL" definition.
 */
#if 0
	#define UNITY_EXCLUDE_STDDEF_H
#endif

/* Define this to enable the unity formatted print macro:
 * "TEST_PRINTF"
 */
#if 0
	#define UNITY_INCLUDE_PRINT_FORMATTED
#endif


/* *************************** TOOLSET CUSTOMIZATION ***************************
 * In addition to the options listed above, there are a number of other options
 * which will come in handy to customize Unity's behavior for your specific
 * toolchain. It is possible that you may not need to touch any of these but
 * certain platforms, particularly those running in simulators, may need to jump
 * through extra hoops to operate properly. These macros will help in those
 * situations.
 **************************************************************************** */

/* By default, Unity prints its results to `stdout` as it runs. This works
 * perfectly fine in most situations where you are using a native compiler for
 * testing. It works on some simulators as well so long as they have `stdout`
 * routed back to the command line. There are times, however, where the
 * simulator will lack support for dumping results or you will want to route
 * results elsewhere for other reasons. In these cases, you should define the
 * `UNITY_OUTPUT_CHAR` macro. This macro accepts a single character at a time
 * (as an `int`, since this is the parameter type of the standard C `putchar`
 * function most commonly used). You may replace this with whatever function
 * call you like.
 *
 * Example:
 * Say you are forced to run your test suite on an embedded processor with no
 * `stdout` option. You decide to route your test result output to a custom
 * serial `RS232_putc()` function you wrote like thus:
 */
#if 0
	#define UNITY_OUTPUT_CHAR(a)					RS232_putc(a)
	#define UNITY_OUTPUT_CHAR_HEADER_DECLARATION	RS232_putc(int)
	#define UNITY_OUTPUT_FLUSH()					RS232_flush()
	#define UNITY_OUTPUT_FLUSH_HEADER_DECLARATION	RS232_flush(void)
	#define UNITY_OUTPUT_START()					RS232_config(115200,1,8,0)
	#define UNITY_OUTPUT_COMPLETE()					RS232_close()
#endif

/* Some compilers require a custom attribute to be assigned to pointers, like
 * `near` or `far`. In these cases, you can give Unity a safe default for these
 * by defining this option with the attribute you would like.
 *
 * Example:
 */
#if 0
	#define UNITY_PTR_ATTRIBUTE	__attribute__((far))
	#define UNITY_PTR_ATTRIBUTE	near
#endif

/* Print execution time of each test when executed in verbose mode
 *
 * Example:
 *
 * TEST - PASS (10 ms)
 */
#if 0
	#define UNITY_INCLUDE_EXEC_TIME
#endif


/* ****************************** TEST FRAMEWORK *******************************
 * Use of the embedded test framework
 **************************************************************************** */

/* Unity now provides a basic test framework to organize and sequence the tests.
 */
#if 1
	#define UNITY_INCLUDE_FRAMEWORK
#endif

#endif  /* UNITY_CONFIG_H */
