# Unity Test - Change Log

## A Note

This document captures significant features and fixes to the Unity project core source files 
and scripts. More detail can be found in the history on Github. 

This project is now tracking changes in more detail. Previous releases get less detailed as
we move back in histroy. 

Prior to 2012, the project was hosted on SourceForge.net
Prior to 2008, the project was an internal project and not released to the public.

## Log

### Unity 2.6.0 ()

New Features:

  - Fill out missing variations of arrays, within, etc. 
  - Add `TEST_PRINTF()`
  - Add `TEST_MATRIX()` and `TEST_RANGE()` options and documentation
  - Add support for searching `TEST_SOURCE_FILE()` for determining test dependencies
  - Add Unity BDD plugin
  - Add `UNITY_INCLUDE_EXEC_TIME` option to report test times
  - Allow user to override test abort underlying mechanism

Significant Bugfixes:

  - More portable validation of NaN and Infinity. Added `UNITY_IS_NAN` and `UNITY_IS_INF` options
  - Add `UNITY_PROGMEM` configuration option
  - Fix overflow detection of hex values when using arrays
  - Fix scripts broken by Ruby standard changes

Other:

  - Avoid pointer comparison when one is null to avoid compiler warnings
  - Significant improvements to documentation
  - Updates to match latest Ruby style specification
  - Meson, CMake, PlatformIO builds

### Unity 2.5.2 (January 2021)

  - improvements to RUN_TEST macro and generated RUN_TEST
  - Fix `UNITY_TEST_ASSERT_BIT(S)_HIGH`
  - Cleaner handling of details tracking by CMock

### Unity 2.5.1 (May 2020)

Mostly a bugfix and stability release.
Bonus Features:

  - Optional TEST_PRINTF macro
  - Improve self-testing procedures.

### Unity 2.5.0 (October 2019)

It's been a LONG time since the last release of Unity. Finally, here it is!
There are too many updates to list here, so some highlights:

  - more standards compliant (without giving up on supporting ALL compilers, no matter how quirky)
  - many more specialized assertions for better test feedback
  - more examples for integrating into your world
  - many many bugfixes and tweaks

### Unity 2.4.3 (November 2017)

  - Allow suiteSetUp() and suiteTearDown() to be povided as normal C functions
  - Fix & Expand Greater Than / Less Than assertions for integers
  - Built-in option to colorize test results
  - Documentation updates

### Unity 2.4.2 (September 2017)

  - Fixed bug in UNTY_TEST_ASSERT_EACH_EQUAL_*
  - Added TEST_ASSERT_GREATER_THAN and TEST_ASSERT_LESS_THAN
  - Updated Module Generator to stop changing names when no style given
  - Cleanup to custom float printing for accuracy
  - Cleanup incorrect line numbers are partial name matching
  - Reduce warnings from using popular function names as variable names

### Unity 2.4.1 (April 2017)

  - test runner generator can inject defines as well as headers
  - added a built-in floating point print routine instead of relying on printf
  - updated to new coding and naming standard
  - updated documentation to be markdown instead of pdf
  - fixed many many little bugs, most of which were supplied by the community (you people are awesome!)
  - coding standard actually enforced in CI

### Unity 2.4.0 (October, 2016)

  - port from SourceForge and numerous bugfixes
