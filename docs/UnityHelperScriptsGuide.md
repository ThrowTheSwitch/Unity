# Unity Helper Scripts

## With a Little Help From Our Friends

Sometimes what it takes to be a really efficient C programmer is a little non-C.
The Unity project includes a couple of Ruby scripts for making your life just a tad easier.
They are completely optional.
If you choose to use them, you'll need a copy of Ruby, of course.
Just install whatever the latest version is, and it is likely to work. You can find Ruby at [ruby-lang.org][].

### `generate_test_runner.rb`

Are you tired of creating your own `main` function in your test file?
Do you keep forgetting to add a `RUN_TEST` call when you add a new test case to your suite?
Do you want to use CMock or other fancy add-ons but don't want to figure out how to create your own `RUN_TEST` macro?

Well then we have the perfect script for you!

The `generate_test_runner` script processes a given test file and automatically creates a separate test runner file that includes ?main?to execute the test cases within the scanned test file.
All you do then is add the generated runner to your list of files to be compiled and linked, and presto you're done!

This script searches your test file for void function signatures having a function name beginning with "test" or "spec".
It treats each of these functions as a test case and builds up a test suite of them.
For example, the following includes three test cases:

```C
void testVerifyThatUnityIsAwesomeAndWillMakeYourLifeEasier(void)
{
  ASSERT_TRUE(1);
}
void test_FunctionName_should_WorkProperlyAndReturn8(void) {
  ASSERT_EQUAL_INT(8, FunctionName());
}
void spec_Function_should_DoWhatItIsSupposedToDo(void) {
  ASSERT_NOT_NULL(Function(5));
}
```

You can run this script a couple of ways.
The first is from the command line:

```Shell
ruby generate_test_runner.rb TestFile.c NameOfRunner.c
```

Alternatively, if you include only the test file parameter, the script will copy the name of the test file and automatically append `_Runner` to the name of the generated file.
The example immediately below will create TestFile_Runner.c.

```Shell
ruby generate_test_runner.rb TestFile.c
```

You can also add a [YAML][] file to configure extra options.
Conveniently, this YAML file is of the same format as that used by Unity and CMock.
So if you are using YAML files already, you can simply pass the very same file into the generator script.

```Shell
ruby generate_test_runner.rb TestFile.c my_config.yml
```

The contents of the YAML file `my_config.yml` could look something like the example below.
If you're wondering what some of these options do, you're going to love the next section of this document.

```YAML
:unity:
  :includes:
    - stdio.h
    - microdefs.h
  :cexception: 1
  :suite_setup: "blah = malloc(1024);"
  :suite_teardown: "free(blah);"
```

If you would like to force your generated test runner to include one or more header files, you can just include those at the command line too.
Just make sure these are _after_ the YAML file, if you are using one:

```Shell
ruby generate_test_runner.rb TestFile.c my_config.yml extras.h
```

Another option, particularly if you are already using Ruby to orchestrate your builds - or more likely the Ruby-based build tool Rake - is requiring this script directly.
Anything that you would have specified in a YAML file can be passed to the script as part of a hash.
Let's push the exact same requirement set as we did above but this time through Ruby code directly:

```Ruby
require "generate_test_runner.rb"
options = {
  :includes => ["stdio.h", "microdefs.h"],
  :cexception => 1,
  :suite_setup => "blah = malloc(1024);",
  :suite_teardown => "free(blah);"
}
UnityTestRunnerGenerator.new.run(testfile, runner_name, options)
```

If you have multiple files to generate in a build script (such as a Rakefile), you might want to instantiate a generator object with your options and call it to generate each runner afterwards.
Like thus:

```Ruby
gen = UnityTestRunnerGenerator.new(options)
test_files.each do |f|
  gen.run(f, File.basename(f,'.c')+"Runner.c"
end
```

#### Options accepted by generate_test_runner.rb

The following options are available when executing `generate_test_runner`.
You may pass these as a Ruby hash directly or specify them in a YAML file, both of which are described above.
In the `examples` directory, Example 3's Rakefile demonstrates using a Ruby hash.

##### `:includes`

This option specifies an array of file names to be `#include`'d at the top of your runner C file.
You might use it to reference custom types or anything else universally needed in your generated runners.

##### `:defines`

This option specifies an array of definitions to be `#define`'d at the top of your runner C file.
Each definition will be wrapped in an `#ifndef`.

##### `:suite_setup`

Define this option with C code to be executed _before any_ test cases are run.

Alternatively, if your C compiler supports weak symbols, you can leave this option unset and instead provide a `void suiteSetUp(void)` function in your test suite.
The linker will look for this symbol and fall back to a Unity-provided stub if it is not found.

##### `:suite_teardown`

Define this option with C code to be executed _after all_ test cases have finished.
An integer variable `num_failures` is available for diagnostics.
The code should end with a `return` statement; the value returned will become the exit code of `main`.
You can normally just return `num_failures`.

Alternatively, if your C compiler supports weak symbols, you can leave this option unset and instead provide a `int suiteTearDown(int num_failures)` function in your test suite.
The linker will look for this symbol and fall back to a Unity-provided stub if it is not found.

##### `:enforce_strict_ordering`

This option should be defined if you have the strict order feature enabled in CMock (see CMock documentation).
This generates extra variables required for everything to run smoothly.
If you provide the same YAML to the generator as used in CMock's configuration, you've already configured the generator properly.

##### `:externc`

This option should be defined if you are mixing C and CPP and want your test runners to automatically include extern "C" support when they are generated.

##### `:mock_prefix` and `:mock_suffix`

Unity automatically generates calls to Init, Verify and Destroy for every file included in the main test file that starts with the given mock prefix and ends with the given mock suffix, file extension not included.
By default, Unity assumes a `Mock` prefix and no suffix.

##### `:plugins`

This option specifies an array of plugins to be used (of course, the array can contain only a single plugin).
This is your opportunity to enable support for CException support, which will add a check for unhandled exceptions in each test, reporting a failure if one is detected.
To enable this feature using Ruby:

```Ruby
:plugins => [ :cexception ]
```

Or as a yaml file:

```YAML
:plugins:
  -:cexception
```

If you are using CMock, it is very likely that you are already passing an array of plugins to CMock.
You can just use the same array here.
This script will just ignore the plugins that don't require additional support.

##### `:include_extensions`

This option specifies the pattern for matching acceptable header file extensions.
By default it will accept hpp, hh, H, and h files.
If you need a different combination of files to search, update this from the default `'(?:hpp|hh|H|h)'`.

##### `:source_extensions`

This option specifies the pattern for matching acceptable source file extensions.
By default it will accept cpp, cc, C, c, and ino files.
If you need a different combination of files to search, update this from the default `'(?:cpp|cc|ino|C|c)'`.

##### `:use_param_tests`

This option enables parameterized test usage.
That tests accepts arguments from `TEST_CASE` and `TEST_RANGE` macros,
that are located above current test definition.
By default, Unity assumes, that parameterized tests are disabled.

Few usage examples can be found in `/test/tests/test_unity_parameterized.c` file.

You should define `UNITY_SUPPORT_TEST_CASES` macro for tests success compiling,
if you enable current option.

You can see list of supported macros list in the
[Parameterized tests provided macros](#parameterized-tests-provided-macros)
section that follows.

##### `:cmdline_args`

When set to `true`, the generated test runner can accept a number of
options to modify how the test(s) are run.

Ensure Unity is compiled with `UNITY_USE_COMMAND_LINE_ARGS` defined or else
the required functions will not exist.

These are the available options:

| Option    | Description                                       |
| --------- | ------------------------------------------------- |
| `-l`      | List all tests and exit                           |
| `-f NAME` | Filter to run only tests whose name includes NAME |
| `-n NAME` | (deprecated) alias of -f                          |
| `-h`      | show the Help menu that lists these options       |
| `-q`      | Quiet/decrease verbosity                          |
| `-v`      | increase Verbosity                                |
| `-x NAME` | eXclude tests whose name includes NAME            |

##### `:setup_name`

Override the default test `setUp` function name.

##### `:teardown_name`

Override the default test `tearDown` function name.

##### `:test_reset_name`

Override the default test `resetTest` function name.

##### `:test_verify_name`

Override the default test `verifyTest` function name.

##### `:main_name`

Override the test's `main()` function name (from `main` to whatever is specified).
The sentinel value `:auto` will use the test's filename with the `.c` extension removed prefixed
with `main_` as the "main" function.

To clarify, if `:main_name == :auto` and the test filename is "test_my_project.c", then the
generated function name will be `main_test_my_project(int argc, char** argv)`.

##### `main_export_decl`

Provide any `cdecl` for the `main()` test function. Is empty by default.

##### `:omit_begin_end`

If `true`, the `UnityBegin` and `UnityEnd` function will not be called for
Unity test state setup and cleanup.

#### Parameterized tests provided macros

Unity provides support for few param tests generators, that can be combined
with each other. You must define test function as usual C function with usual
C arguments, and test generator will pass what you tell as a list of arguments.

Let's show how all of them works on the following test function definitions:

```C
/* Place your test generators here, usually one generator per one or few lines */
void test_demoParamFunction(int a, int b, int c)
{
  TEST_ASSERT_GREATER_THAN_INT(a + b, c);
}
```

##### `TEST_CASE`

Test case is a basic generator, that can be used for param testing.
One call of that macro will generate only one call for test function.
It can be used with different args, such as numbers, enums, strings,
global variables, another preprocessor defines.

If we use replace comment before test function with the following code:

```C
TEST_CASE(1, 2, 5)
TEST_CASE(10, 7, 20)
```

script will generate 2 test calls:

```C
test_demoParamFunction(1, 2, 5);
test_demoParamFunction(10, 7, 20);
```

That calls will be wrapped with `setUp`, `tearDown` and other
usual Unity calls, as for independent unit tests.
The following output can be generated after test executable startup:

```Log
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(1, 2, 5):PASS
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(10, 7, 20):PASS
```

##### `TEST_RANGE`

Test range is an advanced generator. It single call can be converted to zero,
one or few `TEST_CASE` equivalent commands.

That generator can be used for creating numeric ranges in decimal representation
only: integers & floating point numbers. It uses few formats for every parameter:

1. `[start, stop, step]` is stop-inclusive format
2. `<start, stop, step>` is stop-exclusive formats

Format providers 1 and 2 accept only three arguments:

* `start` is start number
* `stop` is end number (can or cannot exists in result sequence for format 1,
will be always skipped for format 2)
* `step` is incrementing step: can be either positive or negative value.

Let's use our `test_demoParamFunction` test for checking, what ranges
will be generated for our single `TEST_RANGE` row:

```C
TEST_RANGE([3, 4, 1], [10, 5, -2], <30, 31, 1>)
```

Tests execution output will be similar to that text:

```Log
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(3, 10, 30):PASS
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(3, 8, 30):PASS
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(3, 6, 30):PASS
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(4, 10, 30):PASS
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(4, 8, 30):PASS
tests/test_unity_parameterizedDemo.c:14:test_demoParamFunction(4, 6, 30):PASS
```

As we can see:

| Parameter | Format | Possible values | Total of values | Format number |
|---|---|---|---|---|
| `a` | `[3, 4, 1]` | `3`, `4` | 2 | Format 1 |
| `b` | `[10, 5, -2]` | `10`, `8`, `6` | 3 | Format 1, negative step, end number is not included |
| `c` | `<30, 31, 1>` | `30` | 1 | Format 2 |

_Note_, that format 2 also supports negative step.

We totally have 2 * 3 * 1 = 6 equal test cases, that can be written as following:

```C
TEST_CASE(3, 10, 30)
TEST_CASE(3, 8, 30)
TEST_CASE(3, 6, 30)
TEST_CASE(4, 10, 30)
TEST_CASE(4, 8, 30)
TEST_CASE(4, 6, 30)
```

##### `TEST_MATRIX`

Test matix is an advanced generator. It single call can be converted to zero,
one or few `TEST_CASE` equivalent commands.

That generator will create tests for all cobinations of the provided list. Each argument has to be given as a list of one or more elements in the format `[<parm1>, <param2>, ..., <paramN-1>, <paramN>]`.

All parameters supported by the `TEST_CASE` is supported as arguments:
- Numbers incl type specifiers e.g. `<1>`, `<1u>`, `<1l>`, `<2.3>`, or `<2.3f>`
- Strings incl string concatianion e.g. `<"string">`, or `<"partial" "string">`
- Chars e.g. `<'c'>`
- Enums e.g. `<ENUM_NAME>`
- Elements of arrays e.g. `<data[0]>`

Let's use our `test_demoParamFunction` test for checking, what ranges
will be generated for our single `TEST_RANGE` row:

```C
TEST_MATRIX([3, 4, 7], [10, 8, 2, 1],[30u, 20.0f])
```

Tests execution output will be similar to that text:

```Log
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 10, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 10, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 8, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 8, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 2, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 2, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 1, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(3, 1, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 10, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 10, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 8, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 8, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 2, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 2, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 1, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(4, 1, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 10, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 10, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 8, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 8, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 2, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 2, 20.0f):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 1, 30u):PASS
tests/test_unity_parameterizedDemo.c:18:test_demoParamFunction(7, 1, 20.0f):PASS
```

As we can see:

| Parameter | Format | Count of values |
|---|---|---|
| `a` | `[3, 4, 7]` | 2 |
| `b` | `[10, 8, 2, 1]` | 4 |
| `c` | `[30u, 20.0f]` | 2 |

We totally have 2 * 4 * 2 = 16 equal test cases, that can be written as following:

```C
TEST_CASE(3, 10, 30u)
TEST_CASE(3, 10, 20.0f)
TEST_CASE(3, 8, 30u)
TEST_CASE(3, 8, 20.0f)
TEST_CASE(3, 2, 30u)
TEST_CASE(3, 2, 20.0f)
TEST_CASE(3, 1, 30u)
TEST_CASE(3, 1, 20.0f)
TEST_CASE(4, 10, 30u)
TEST_CASE(4, 10, 20.0f)
TEST_CASE(4, 8, 30u)
TEST_CASE(4, 8, 20.0f)
TEST_CASE(4, 2, 30u)
TEST_CASE(4, 2, 20.0f)
TEST_CASE(4, 1, 30u)
TEST_CASE(4, 1, 20.0f)
TEST_CASE(7, 10, 30u)
TEST_CASE(7, 10, 20.0f)
TEST_CASE(7, 8, 30u)
TEST_CASE(7, 8, 20.0f)
TEST_CASE(7, 2, 30u)
TEST_CASE(7, 2, 20.0f)
TEST_CASE(7, 1, 30u)
TEST_CASE(7, 1, 20.0f)
```

### `unity_test_summary.rb`

A Unity test file contains one or more test case functions.
Each test case can pass, fail, or be ignored.
Each test file is run individually producing results for its collection of test cases.
A given project will almost certainly be composed of multiple test files.
Therefore, the suite of tests is comprised of one or more test cases spread across one or more test files.
This script aggregates individual test file results to generate a summary of all executed test cases.
The output includes how many tests were run, how many were ignored, and how many failed. In addition, the output includes a listing of which specific tests were ignored and failed.
A good example of the breadth and details of these results can be found in the `examples` directory.
Intentionally ignored and failing tests in this project generate corresponding entries in the summary report.

If you're interested in other (prettier?) output formats, check into the [Ceedling][] build tool project that works with Unity and CMock and supports xunit-style xml as well as other goodies.

This script assumes the existence of files ending with the extensions `.testpass` and `.testfail`.
The contents of these files includes the test results summary corresponding to each test file executed with the extension set according to the presence or absence of failures for that test file.
The script searches a specified path for these files, opens each one it finds, parses the results, and aggregates and prints a summary.
Calling it from the command line looks like this:

```Shell
ruby unity_test_summary.rb build/test/
```

You can optionally specify a root path as well.
This is really helpful when you are using relative paths in your tools' setup, but you want to pull the summary into an IDE like Eclipse for clickable shortcuts.

```Shell
ruby unity_test_summary.rb build/test/ ~/projects/myproject/
```

Or, if you're more of a Windows sort of person:

```Shell
ruby unity_test_summary.rb build\teat\ C:\projects\myproject\
```

When configured correctly, you'll see a final summary, like so:

```Shell
--------------------------
UNITY IGNORED TEST SUMMARY
--------------------------
blah.c:22:test_sandwiches_should_HaveBreadOnTwoSides:IGNORE

-------------------------
UNITY FAILED TEST SUMMARY
-------------------------
blah.c:87:test_sandwiches_should_HaveCondiments:FAIL:Expected 1 was 0
meh.c:38:test_soda_should_BeCalledPop:FAIL:Expected "pop" was "coke"

--------------------------
OVERALL UNITY TEST SUMMARY
--------------------------
45 TOTAL TESTS 2 TOTAL FAILURES 1 IGNORED
```

How convenient is that?

*Find The Latest of This And More at [ThrowTheSwitch.org][]*

[ruby-lang.org]: https://ruby-lang.org/
[YAML]: http://www.yaml.org/
[Ceedling]: http://www.throwtheswitch.org/ceedling
[ThrowTheSwitch.org]: https://throwtheswitch.org
