#include "unity.h"
#include <stdio.h>
#include <string.h>

struct _Unity Unity =
{
    NULL,
    NULL,
    0,
    0,
    0,
    0,
    0,
    1e-4f,
    0,
};

void UnityPrintChar(const char ch)
{
    putchar(ch);
}

void UnityPrint(const char* string)
{
    unsigned char* pch = (unsigned char*)string;

    if (pch != NULL)
    {
        while (*pch)
        {
            UnityPrintChar(*pch);
            pch++;
        }
    }
}

void UnityPrintNumberByStyle(const long number, const UNITY_DISPLAY_STYLE_T style)
{
    switch (style)
    {
    case UNITY_DISPLAY_STYLE_HEX8:    UnityPrintNumberHex(number,2);            break;
    case UNITY_DISPLAY_STYLE_HEX16:   UnityPrintNumberHex(number,4);            break;
    case UNITY_DISPLAY_STYLE_HEX32:   UnityPrintNumberHex(number,8);            break;
    case UNITY_DISPLAY_STYLE_UINT:    UnityPrintNumberUnsigned(number);         break;
    default:                          UnityPrintNumber(number);                 break;
    }
}

/// basically do an itoa using as little ram as possible
void UnityPrintNumber(const long number_to_print)
{
    unsigned long divisor = 10;
    long number = number_to_print;

    if (number < 0)
    {
        UnityPrintChar('-');
        number = -number;
    }

    // figure out initial divisor
    while (number / divisor)
    {
        divisor *= 10;
    }

    // now divide number by divisor, mod and print, then divide divisor
    do
    {
        divisor /= 10;
        UnityPrintChar((char)('0' + (number / divisor % 10)));
    }
    while (divisor > 1U);
}

/// basically do an itoa using as little ram as possible
void UnityPrintNumberUnsigned(const unsigned long number)
{
    unsigned long divisor = 10;

    // figure out initial divisor
    while (number / divisor)
    {
        divisor *= 10;
    }

    // now divide number by divisor, mod and print, then divide divisor
    do
    {
        divisor /= 10;
        UnityPrintChar((char)('0' + (number / divisor % 10)));
    }
    while (divisor > 1U);
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
            UnityPrintChar((char)('0' + nibble));
        }
        else
        {
            UnityPrintChar((char)('A' - 10 + nibble));
        }
    }
}

void UnityPrintMask(const unsigned long mask, const unsigned long number)
{
    unsigned long bit = 0x00000001;
    int i;

    for (i = 0; i < 32; i++)
    {
        if (bit & mask)
        {
            if (bit & number)
            {
                UnityPrintChar('1');
            }
            else
            {
                UnityPrintChar('0');
            }
        }
        else
        {
            UnityPrintChar('X');
        }
        bit = bit << 1;
    }
}

void UnityTestResultsBegin(const int line)
{
    UnityPrint(Unity.TestFile);
    UnityPrintChar(':');
    UnityPrintNumber(line);
    UnityPrintChar(':');
    UnityPrint(Unity.CurrentTestName);
    UnityPrintChar(':');
}

void UnityConcludeTest()
{
    if (Unity.CurrentTestIgnored)
    {
        Unity.TestIgnores++;
    }
    else if (!Unity.CurrentTestFailed)
    {
        UnityPrint(Unity.CurrentTestName);
        UnityPrint("::: PASS\n");
    }
    else
    {
        Unity.TestFailures++;
    }

    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
}

void UnityAssertBits(const int mask,
                     const int expected,
                     const int actual,
                     const char* msg,
                     const unsigned short lineNumber)
{
    if ((mask & expected) != (mask & actual))
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsBegin(lineNumber);
        UnityPrint("Expected ");
        UnityPrintMask(mask, expected);
        UnityPrint(" was ");
        UnityPrintMask(mask, actual);
        UnityPrintChar('.');
        if (msg)
        {
            UnityPrintChar(' ');
            UnityPrint(msg);
        }
        UnityPrintChar('\n');
    }
}

void UnityAssertEqualInt(const int expected,
                         const int actual,
                         const char* msg,
                         const unsigned short lineNumber,
                         const UNITY_DISPLAY_STYLE_T style)
{
    if (expected != actual)
    {
        Unity.CurrentTestFailed = 1;

        UnityTestResultsBegin(lineNumber);
        UnityPrint("Expected ");
        UnityPrintNumberByStyle(expected, style);
        UnityPrint(" was ");
        UnityPrintNumberByStyle(actual, style);
        UnityPrintChar('.');
        if (msg)
        {
            UnityPrintChar(' ');
            UnityPrint(msg);
        }
        UnityPrintChar('\n');
    }
}

void UnityAssertFloatsWithin(const float delta,
                             const float expected,
                             const float actual,
                             const char* msg,
                             const unsigned short lineNumber)
{
    float diff = actual - expected;

    if (diff < 0)
    {
        diff = -diff;
    }

    if (delta < diff)
    {
        Unity.CurrentTestFailed = 1;
        UnityTestResultsBegin(lineNumber);
        UnityPrint("Floats not within delta.");
        if (msg)
        {
            UnityPrintChar(' ');
            UnityPrint(msg);
        }
        UnityPrintChar('\n');
    }
}

void UnityAssertIntsWithin(const int delta,
                           const int expected,
                           const int actual,
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
        UnityTestResultsBegin(lineNumber);
        UnityPrint("Ints not within delta.");
        if (msg)
        {
            UnityPrintChar(' ');
            UnityPrint(msg);
        }
        UnityPrintChar('\n');
    }
}

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            unsigned short lineNumber)
{
    unsigned int i;

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
        UnityTestResultsBegin(lineNumber);
        UnityPrint("Expected '");
        UnityPrint(expected);
        UnityPrint("' was '");
        UnityPrint(actual);
        UnityPrintChar('\'');
        UnityPrintChar('.');
        if (msg)
        {
            UnityPrintChar(' ');
            UnityPrint(msg);
        }
        UnityPrintChar('\n');
    }
}


void UnityAssertEqualMemory(void* expected,
                            void* actual,
                            unsigned int length,
                            const char* msg,
                            unsigned short lineNumber)
{
    if (length == 0)
        return;

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
        UnityTestResultsBegin(lineNumber);
        UnityPrint("Memory Mismatch.");
        if (msg)
        {
            UnityPrintChar(' ');
            UnityPrint(msg);
        }
        UnityPrintChar('\n');
    }
}

void UnityFail(const char* message, const int line)
{
    Unity.CurrentTestFailed = 1;
    UnityTestResultsBegin(line);
    UnityPrint(message);
    UnityPrintChar('\n');
}

void UnityIgnore(const char* message, const int line)
{
    Unity.CurrentTestIgnored = 1;
    UnityTestResultsBegin(line);
    UnityPrint(message);
    UnityPrint(" IGNORED\n");
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

int UnityGetNumFailures(void)
{
    return Unity.TestFailures;
}
