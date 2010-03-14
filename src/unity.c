#include "unity.h"
#include <stdio.h>
#include <string.h>

struct _Unity Unity = { 0 };

void UnityPrint(const char* string)
{
    unsigned char* pch = (unsigned char*)string;

    if (pch != NULL)
    {
        while (*pch)
        {
            UNITY_OUTPUT_CHAR(*pch);
            pch++;
        }
    }
}

void UnityPrintNumberByStyle(const long number, const UNITY_DISPLAY_STYLE_T style)
{
    switch (style)
    {
    case UNITY_DISPLAY_STYLE_HEX8:    UnityPrintNumberHex((unsigned long)number, 2);           break;
    case UNITY_DISPLAY_STYLE_HEX16:   UnityPrintNumberHex((unsigned long)number, 4);           break;
    case UNITY_DISPLAY_STYLE_HEX32:   UnityPrintNumberHex((unsigned long)number, 8);           break;
    case UNITY_DISPLAY_STYLE_UINT:    UnityPrintNumberUnsigned((unsigned long)number);         break;
    default:                          UnityPrintNumber(number);                                break;
    }
}

/// basically do an itoa using as little ram as possible
void UnityPrintNumber(const long number_to_print)
{
    long divisor = 1;
    long next_divisor;
    long number = number_to_print;

    if (number < 0)
    {
        UNITY_OUTPUT_CHAR('-');
        number = -number;
    }

    // figure out initial divisor
    while (number / divisor > 9)
    {
        next_divisor = divisor * 10;
        if (next_divisor > divisor)
            divisor = next_divisor;
        else
            break;
    }

    // now mod and print, then divide divisor
    do
    {
        UNITY_OUTPUT_CHAR((char)('0' + (number / divisor % 10)));
        divisor /= 10;
    }
    while (divisor > 0);
}

/// basically do an itoa using as little ram as possible
void UnityPrintNumberUnsigned(const unsigned long number)
{
    unsigned long divisor = 1;
    unsigned long next_divisor;

    // figure out initial divisor
    while (number / divisor > 9)
    {
        next_divisor = divisor * 10;
        if (next_divisor > divisor)
            divisor = next_divisor;
        else
            break;
    }

    // now mod and print, then divide divisor
    do
    {
        UNITY_OUTPUT_CHAR((char)('0' + (number / divisor % 10)));
        divisor /= 10;
    }
    while (divisor > 0);
}

void UnityPrintNumberHex(const unsigned long number, const char nibbles_to_print)
{
    unsigned long nibble;
	char nibbles = nibbles_to_print;
    UnityPrint("0x");

    while (nibbles > 0)
    {
        nibble = (number >> (--nibbles << 2)) & 0x0000000F;
        if (nibble <= 9)
        {
            UNITY_OUTPUT_CHAR((char)('0' + nibble));
        }
        else
        {
            UNITY_OUTPUT_CHAR((char)('A' - 10 + nibble));
        }
    }
}

void UnityPrintMask(const unsigned long mask, const unsigned long number)
{
    unsigned long bit = 0x80000000;
    long i;

    for (i = 0; i < 32; i++)
    {
        if (bit & mask)
        {
            if (bit & number)
            {
                UNITY_OUTPUT_CHAR('1');
            }
            else
            {
                UNITY_OUTPUT_CHAR('0');
            }
        }
        else
        {
            UNITY_OUTPUT_CHAR('X');
        }
        bit = bit >> 1;
    }
}

void UnityTestResultsBegin(const char* file, const long line)
{
    UnityPrint(file);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber(line);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint(Unity.CurrentTestName);
    UNITY_OUTPUT_CHAR(':');
}

void UnityTestResultsFailBegin(const long line)
{
    UnityTestResultsBegin(Unity.AssertContainerFile, line);
    UnityPrint("FAIL:");
}

void UnityConcludeTest()
{
    if (Unity.CurrentTestIgnored)
    {
        Unity.TestIgnores++;
    }
    else if (!Unity.CurrentTestFailed)
    {
        UnityTestResultsBegin(Unity.TestFile, Unity.CurrentTestLineNumber);
        UnityPrint("PASS\n");
    }
    else
    {
        Unity.TestFailures++;
    }

    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
}

void UnityAssertBits(const long mask,
                     const long expected,
                     const long actual,
                     const char* msg,
                     const unsigned short lineNumber)
{
    if ((mask & expected) != (mask & actual))
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Expected ");
        UnityPrintMask(mask, expected);
        UnityPrint(" was ");
        UnityPrintMask(mask, actual);
        UNITY_OUTPUT_CHAR('.');
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

void UnityAssertEqualNumber(const long expected,
                         const long actual,
                         const char* msg,
                         const unsigned short lineNumber,
                         const UNITY_DISPLAY_STYLE_T style)
{
    if (expected != actual)
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Expected ");
        UnityPrintNumberByStyle(expected, style);
        UnityPrint(" was ");
        UnityPrintNumberByStyle(actual, style);
        UNITY_OUTPUT_CHAR('.');
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

void UnityAssertEqualNumberUnsigned(const unsigned long expected,
                         const unsigned long actual,
                         const char* msg,
                         const unsigned short lineNumber,
                         const UNITY_DISPLAY_STYLE_T style)
{
    if (expected != actual)
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Expected ");
        UnityPrintNumberByStyle(expected, style);
        UnityPrint(" was ");
        UnityPrintNumberByStyle(actual, style);
        UNITY_OUTPUT_CHAR('.');
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

void UnityAssertEqualIntArray(const int* expected,
                              const int* actual,
                              const unsigned long num_elements,
                              const char* msg,
                              const unsigned short lineNumber,
                              const UNITY_DISPLAY_STYLE_T style)
{
    unsigned long elements = num_elements;
    const int* ptr_expected = expected;
    const int* ptr_actual = actual;

    if (elements == 0)
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("You asked me to compare 0 elements of an array, which was pointless.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
        return;
    }

    while (elements--)
    {
        if (*ptr_expected++ != *ptr_actual++)
        {
            Unity.CurrentTestFailed = 1;

            UnityTestResultsFailBegin(lineNumber);
            UnityPrint("Element ");
            UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
            UnityPrint(" Expected ");
            UnityPrintNumberByStyle(*--ptr_expected, style);
            UnityPrint(" was ");
            UnityPrintNumberByStyle(*--ptr_actual, style);
            UNITY_OUTPUT_CHAR('.');
            if (msg)
            {
                UNITY_OUTPUT_CHAR(' ');
                UnityPrint(msg);
            }
            UNITY_OUTPUT_CHAR('\n');
            return;
        }
    }
}

void UnityAssertEqualUnsignedIntArray(const unsigned int* expected,
                              const unsigned int* actual,
                              const unsigned long num_elements,
                              const char* msg,
                              const unsigned short lineNumber,
                              const UNITY_DISPLAY_STYLE_T style)
{
    unsigned long elements = num_elements;
    const unsigned int* ptr_expected = expected;
    const unsigned int* ptr_actual = actual;

    if (elements == 0)
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("You asked me to compare nothing, which was pointless.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
        return;
    }

    while (elements--)
    {
        if (*ptr_expected++ != *ptr_actual++)
        {
            Unity.CurrentTestFailed = 1;

            UnityTestResultsFailBegin(lineNumber);
            UnityPrint("Element ");
            UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
            UnityPrint(" Expected ");
            UnityPrintNumberByStyle(*--ptr_expected, style);
            UnityPrint(" was ");
            UnityPrintNumberByStyle(*--ptr_actual, style);
            UNITY_OUTPUT_CHAR('.');
            if (msg)
            {
                UNITY_OUTPUT_CHAR(' ');
                UnityPrint(msg);
            }
            UNITY_OUTPUT_CHAR('\n');
            return;
        }
    }
}

#ifndef UNITY_EXCLUDE_FLOAT
void UnityAssertFloatsWithin(const _UF delta,
                             const _UF expected,
                             const _UF actual,
                             const char* msg,
                             const unsigned short lineNumber)
{
    _UF diff = actual - expected;
    _UF pos_delta = delta;
    
    if (diff < 0)
    {
        diff = 0.0f - diff;
    }
    if (pos_delta < 0)
    {
        pos_delta = 0.0f - pos_delta;
    }
    
    if (pos_delta < diff)
    {
        Unity.CurrentTestFailed = 1;
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Floats not within delta.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}
#endif

void UnityAssertNumbersWithin(const long delta,
                           const long expected,
                           const long actual,
                           const char* msg,
                           const unsigned short lineNumber)
{
    int diff = actual - expected;

    if (diff < 0)
    {
        diff = -diff;
    }

    if (delta < diff)
    {
        Unity.CurrentTestFailed = 1;
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Values not within delta.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

void UnityAssertNumbersUnsignedWithin(const unsigned long delta,
                           const unsigned long expected,
                           const unsigned long actual,
                           const char* msg,
                           const unsigned short lineNumber)
{
    unsigned int diff = actual - expected;

    if (delta < diff)
    {
        Unity.CurrentTestFailed = 1;
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Values not within delta.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const unsigned short lineNumber)
{
    unsigned long i;

    // if both pointers not null compare the strings
    if (expected && actual)
    {
        for (i = 0; expected[i] || actual[i]; i++)
        {
            if (expected[i] != actual[i])
            {
                Unity.CurrentTestFailed = 1;
            }
        }
    }
    else
    { // handle case of one pointers being null (if both null, test should pass)
        if (expected != actual)
        {
            Unity.CurrentTestFailed = 1;
        }
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Expected '");
        UnityPrint(expected);
        UnityPrint("' was '");
        UnityPrint(actual);
        UNITY_OUTPUT_CHAR('\'');
        UNITY_OUTPUT_CHAR('.');
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}


void UnityAssertEqualMemory(const void* expected,
                            const void* actual,
                            unsigned long length,
                            const char* msg,
                            const unsigned short lineNumber)
{
    if (length == 0)
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("You asked me to compare nothing, which was pointless.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
        return;
    }

    // if both pointers not null compare the memory
    if (expected && actual)
    {
        if (memcmp(expected, actual, length) != 0)
        {
            Unity.CurrentTestFailed = 1;
        }
    }
    else
    { // handle case of one pointers being null (if both null, test should pass)
        if (expected != actual)
        {
            Unity.CurrentTestFailed = 1;
        }
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Memory Mismatch.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

void UnityAssertEqualMemoryArray(const void* expected,
                                 const void* actual,
                                 unsigned long length,
                                 unsigned long num_elements,
                                 const char* msg,
                                 const unsigned short lineNumber)
{
	  unsigned char* expected_ptr = (unsigned char*)expected;
	  unsigned char* actual_ptr = (unsigned char*)actual;	
    unsigned long elements = num_elements;
    if ((elements == 0) || (length == 0))
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("You asked me to compare nothing, which was pointless.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
        return;
    }
    
    // if both pointers not null compare the memory
    if (expected_ptr && actual_ptr)
    {
        while (elements--)
        {
            if (memcmp((const void*)expected_ptr, (const void*)actual_ptr, length) != 0)
            {
                Unity.CurrentTestFailed = 1;
                break;
            }
            expected_ptr += length;
            actual_ptr += length;
        }
    }
    else
    { // handle case of one pointers being null (if both null, test should pass) 
        if (expected_ptr != actual_ptr)
        {
            Unity.CurrentTestFailed = 1;
        }
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint("Element ");
        UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
        UnityPrint(" Memory Mismatch.");
        if (msg)
        {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

void UnityFail(const char* message, const long line)
{
    Unity.CurrentTestFailed = 1;
    UnityTestResultsBegin(Unity.AssertContainerFile, line);
    UnityPrint("FAIL");
    if (message != NULL)
    {
      UNITY_OUTPUT_CHAR(':');
      UnityPrint(message);
    }
    UNITY_OUTPUT_CHAR('\n');
}

void UnityIgnore(const char* message, const long line)
{
    Unity.CurrentTestIgnored = 1;
    UnityTestResultsBegin(Unity.AssertContainerFile, line);
    UnityPrint("IGNORE");
    if (message != NULL)
    {
      UNITY_OUTPUT_CHAR(':');
      UnityPrint(message);
    }
    UNITY_OUTPUT_CHAR('\n');
}

void UnityBegin()
{
    Unity.NumberOfTests = 0;
}

void UnityEnd(void)
{
    UnityPrintNumber(Unity.NumberOfTests);
    UnityPrint(" Tests ");
    UnityPrintNumber(Unity.TestFailures);
    UnityPrint(" Failures ");
    UnityPrintNumber(Unity.TestIgnores);
    UnityPrint(" Ignored\n");
    if (Unity.TestFailures == 0U)
    {
        UnityPrint("OK\n");
    }
    else
    {
        UnityPrint("FAIL\n");
    }
}
