# Unity Fixtures

This Framework is an optional add-on to Unity.
By including unity_framework.h in place of unity.h, you may now work with Unity in a manner similar to CppUTest.
This framework adds the concepts of test groups and gives finer control of your tests over the command line.

This framework is primarily supplied for those working through James Grenning's book on Embedded Test Driven Development, or those coming to Unity from CppUTest.
We should note that using this framework glosses over some of the features of Unity, and makes it more difficult to integrate with other testing tools like Ceedling and CMock.

## Dependency Notification

Fixtures, by default, uses the Memory addon as well.
This is to make it simple for those trying to follow along with James' book.
Using them together is completely optional.
You may choose to use Fixtures without Memory handling by defining `UNITY_FIXTURE_NO_EXTRAS`.
It will then stop automatically pulling in extras and leave you to do it as desired.

## Usage information

By default the test executables produced by Unity Fixtures run all tests once, but the behavior can be configured with command-line flags.
Run the test executable with the `--help` flag for more information.

It's possible to add a custom line at the end of the help message, typically to point to project-specific or company-specific unit test documentation.
Define `UNITY_CUSTOM_HELP_MSG` to provide a custom message, e.g.:

    #define UNITY_CUSTOM_HELP_MSG "If any test fails see https://example.com/troubleshooting"
