#include "unity.h"
#include <stdio.h>
#include <string.h>

#define UNITY_FAIL_AND_BAIL   { Unity.CurrentTestFailed  = 1; UNITY_OUTPUT_CHAR('\n'); longjmp(Unity.AbortFrame, 1); }
#define UNITY_IGNORE_AND_BAIL { Unity.CurrentTestIgnored = 1; UNITY_OUTPUT_CHAR('\n'); longjmp(Unity.AbortFrame, 1); }

struct _Unity Unity = { 0 };

const char* UnityStrExpected = " Expected ";
const char* UnityStrWas      = " Was ";
const char* UnityStrTo       = " To ";
const char* UnityStrElement  = " Element ";
const char* UnityStrMemory   = " Memory Mismatch.";
const char* UnityStrDelta    = " Values Not Within Delta ";
const char* UnityStrPointless= " You Asked Me To Compare Nothing, Which Was Pointless.";
const char* UnityStrSpacer   = ". ";

void UnityAddMsgIfSpecified(const char* msg);

//-----------------------------------------------
// Pretty Printers
//-----------------------------------------------

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

void UnityTestResultsBegin(const char* file, const UNITY_LINE_TYPE line)
{
    UnityPrint(file);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber(line);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint(Unity.CurrentTestName);
    UNITY_OUTPUT_CHAR(':');
}

void UnityTestResultsFailBegin(const UNITY_LINE_TYPE line)
{
    UnityTestResultsBegin(Unity.TestFile, line);
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

void UnityAddMsgIfSpecified(const char* msg)
{
    if (msg)
    {
        UnityPrint(UnityStrSpacer);
        UnityPrint(msg);
    }
}

//-----------------------------------------------
// Assertion Functions
//-----------------------------------------------

void UnityAssertBits(const long mask,
                     const long expected,
                     const long actual,
                     const char* msg,
                     const UNITY_LINE_TYPE lineNumber)
{
    if ((mask & expected) != (mask & actual))
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintMask(mask, expected);
        UnityPrint(UnityStrWas);
        UnityPrintMask(mask, actual);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}

void UnityAssertEqualNumber(const long expected,
                            const long actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber,
                            const UNITY_DISPLAY_STYLE_T style)
{
    if (expected != actual)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintNumberByStyle(expected, style);
        UnityPrint(UnityStrWas);
        UnityPrintNumberByStyle(actual, style);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}

void UnityAssertEqualIntArray(const int* expected,
                              const int* actual,
                              const unsigned long num_elements,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber,
                              const UNITY_DISPLAY_STYLE_T style)
{
    unsigned long elements = num_elements;
    const _US32* ptr_exp32 = (_US32*)expected;
    const _US16* ptr_exp16 = (_US16*)expected;
    const _US8*  ptr_exp8  = (_US8*)expected;
    const _US32* ptr_act32 = (_US32*)actual;
    const _US16* ptr_act16 = (_US16*)actual;
    const _US8*  ptr_act8  = (_US8*)actual;

    if (elements == 0)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrPointless);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }

    switch(style)
    {
        case UNITY_DISPLAY_STYLE_HEX8:
            while (elements--)
            {
                if (*ptr_exp8++ != *ptr_act8++)
                {
                    UnityTestResultsFailBegin(lineNumber);
                    UnityPrint(UnityStrElement);
                    UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
                    UnityPrint(UnityStrExpected);
                    UnityPrintNumberByStyle(*--ptr_exp8, style);
                    UnityPrint(UnityStrWas);
                    UnityPrintNumberByStyle(*--ptr_act8, style);
                    UnityAddMsgIfSpecified(msg);
                    UNITY_FAIL_AND_BAIL;
                }
            }
            break;
        case UNITY_DISPLAY_STYLE_HEX16:
            while (elements--)
            {
                if (*ptr_exp16++ != *ptr_act16++)
                {
                    UnityTestResultsFailBegin(lineNumber);
                    UnityPrint(UnityStrElement);
                    UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
                    UnityPrint(UnityStrExpected);
                    UnityPrintNumberByStyle(*--ptr_exp16, style);
                    UnityPrint(UnityStrWas);
                    UnityPrintNumberByStyle(*--ptr_act16, style);
                    UnityAddMsgIfSpecified(msg);
                    UNITY_FAIL_AND_BAIL;
                }
            }
            break;
        default:
            while (elements--)
            {
                if (*ptr_exp32++ != *ptr_act32++)
                {
                    UnityTestResultsFailBegin(lineNumber);
                    UnityPrint(UnityStrElement);
                    UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
                    UnityPrint(UnityStrExpected);
                    UnityPrintNumberByStyle(*--ptr_exp32, style);
                    UnityPrint(UnityStrWas);
                    UnityPrintNumberByStyle(*--ptr_act32, style);
                    UnityAddMsgIfSpecified(msg);
                    UNITY_FAIL_AND_BAIL;
                }
            }
            break;
    }
}

void UnityAssertEqualFloatArray(const _UF* expected,
                                const _UF* actual,
                                const unsigned long num_elements,
                                const char* msg,
                                const UNITY_LINE_TYPE lineNumber)
{
    unsigned long elements = num_elements;
    const _UF* ptr_expected = expected;
    const _UF* ptr_actual = actual;

    if (elements == 0)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrPointless);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }

    while (elements--)
    {
        if ((*ptr_expected - *ptr_actual) > (UNITY_FLOAT_PRECISION * *ptr_expected))
        {
            UnityTestResultsFailBegin(lineNumber);
            UnityPrint(UnityStrElement);
            UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
            UnityPrint(UnityStrDelta);
            UnityAddMsgIfSpecified(msg);
            UNITY_FAIL_AND_BAIL;
        }
        ptr_expected++;
        ptr_actual++;
    }
}

#ifndef UNITY_EXCLUDE_FLOAT
void UnityAssertFloatsWithin(const _UF delta,
                             const _UF expected,
                             const _UF actual,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber)
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
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrDelta);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}
#endif

void UnityAssertNumbersWithin( const long delta,
                               const long expected,
                               const long actual,
                               const char* msg,
                               const UNITY_LINE_TYPE lineNumber,
                               const UNITY_DISPLAY_STYLE_T style)
{
    if (style == UNITY_DISPLAY_STYLE_INT)
    {
        if (actual > expected)
          Unity.CurrentTestFailed = ((actual - expected) > delta);
        else
          Unity.CurrentTestFailed = ((expected - actual) > delta);
    }
    else
    {
        if ((unsigned long)actual > (unsigned long)expected)
            Unity.CurrentTestFailed = ((unsigned long)(actual - expected) > (unsigned long)delta);
        else
            Unity.CurrentTestFailed = ((unsigned long)(expected - actual) > (unsigned long)delta);
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrDelta);
        UnityPrintNumberByStyle(delta, style);
        UnityPrint(UnityStrExpected);
        UnityPrintNumberByStyle(expected, style);
        UnityPrint(UnityStrWas);
        UnityPrintNumberByStyle(actual, style);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber)
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
        UnityPrint(UnityStrExpected);
        UNITY_OUTPUT_CHAR('\'');
        UnityPrint(expected);
        UNITY_OUTPUT_CHAR('\'');
        UnityPrint(UnityStrWas);
        UNITY_OUTPUT_CHAR('\'');
        UnityPrint(actual);
        UNITY_OUTPUT_CHAR('\'');
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}

void UnityAssertEqualMemory( const void* expected,
                             const void* actual,
                             unsigned long length,
                             unsigned long num_elements,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber)
{
	  unsigned char* expected_ptr = (unsigned char*)expected;
	  unsigned char* actual_ptr = (unsigned char*)actual;	
    unsigned long elements = num_elements;
    if ((elements == 0) || (length == 0))
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrPointless);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
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
        if (num_elements > 1)
        {
            UnityPrint(UnityStrElement);
            UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
        }
        UnityPrint(UnityStrMemory);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}

//-----------------------------------------------
// Control Functions
//-----------------------------------------------

void UnityFail(const char* msg, const UNITY_LINE_TYPE line)
{
    UnityTestResultsBegin(Unity.TestFile, line);
    UnityPrint("FAIL");
    if (msg != NULL)
    {
      UNITY_OUTPUT_CHAR(':');
      UnityPrint(msg);
    }
    UNITY_FAIL_AND_BAIL;
}

void UnityIgnore(const char* msg, const UNITY_LINE_TYPE line)
{
    UnityTestResultsBegin(Unity.TestFile, line);
    UnityPrint("IGNORE");
    if (msg != NULL)
    {
      UNITY_OUTPUT_CHAR(':');
      UnityPrint(msg);
    }
    UNITY_IGNORE_AND_BAIL;
}

void UnityBegin()
{
    Unity.NumberOfTests = 0;
}

void UnityEnd(void)
{
    UnityPrint("-----------------------\n");
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
