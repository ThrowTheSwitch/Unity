Example 5
=========

Demonstrate Details Stack usage to implement something similar to a stacktrace.
This allows locating the error source much faster in branching/iterating code.

Build and run with Make
---
Just run `make`.

Output
---
Below the output is annotated with source of the elements.

```
test/TestProductionCode.c:36:test_BitExtractor:FAIL: Expected 0 Was 1. During call BitExtractor. During call BitExtractor_down. Bit Position 6. Bit Mask 0x02. Unexpected bit value
```

| String                      | Source                                                                    |
|-----------------------------|---------------------------------------------------------------------------|
| `test/TestProductionCode.c` | `Unity.TestFile`                                                          |
| `36`                        | `UNITY_TEST_ASSERT_EQUAL_INT` line                                        |
| `test_BitExtractor`         | `RUN_TEST` name                                                           |
| `FAIL`                      | `UnityStrFail`                                                            |
| `Expected 1 Was 0`          | `UnityAssertEqualNumber`                                                  |
| `During call`               | Detail 0, Label                                                           |
| `BitExtractor`              | Detail 0, Value                                                           |
| `During call`               | Detail 0, Label                                                           |
| `BitExtractor`              | Detail 0, Value                                                           |
| `During call`               | Detail 1, Label                                                           |
| `BitExtractor_down`         | Detail 1, Value                                                           |
| `Bit Position`              | Detail 2, Label (literal starts with #\x18, so value is printed as INT32) |
| `6`                         | Detail 2 Value                                                            |
| `Bit Mask`                  | Detail 2, Label (literal starts with #\x41, so value is printed as HEX8)  |
| `0x02`                      | Detail 2 Value                                                            |
| `Unexpected bit value`      | `UNITY_TEST_ASSERT_EQUAL_INT` message                                     |

While this example is a bit contrived, the source of the error can be clearly located to be within the `test_BitExtractor->BitExtractor->BitExtractor_down`
