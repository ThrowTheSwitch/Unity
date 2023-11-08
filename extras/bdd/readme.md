# Unity Project - BDD Feature

Unity's Behavior-Driven Development (BDD) test feature. It allows developers to structure and describe various phases (Given, When, Then) of a test scenario in a BDD-style format.

## Introduction

This project is based on the Unity framework originally created by Mike Karlesky, Mark VanderVoord, and Greg Williams in 2007. The project extends Unity by providing macros to define BDD structures with descriptive elements. Feature added by Michael Gene Brockus (Dreamer).

## License

This project is distributed under the MIT License. See the [license.txt](license.txt) file for more information.

## Usage

### BDD Macros

The provided BDD macros allow you to structure your test scenarios in a descriptive manner. These macros are for descriptive purposes only and do not have functional behavior.

- `GIVEN(description)`: Describes the "Given" phase of a test scenario.
- `WHEN(description)`: Describes the "When" phase of a test scenario.
- `THEN(description)`: Describes the "Then" phase of a test scenario.

Example usage:

```c
GIVEN("a valid input") {
    // Test setup and context
    // ...

    WHEN("the input is processed") {
        // Perform the action
        // ...

        THEN("the expected outcome occurs") {
            // Assert the outcome
            // ...
        }
    }
}
```
