/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include "unity.h"
#include <stdio.h>
#include <string.h>

#define UNITY_FAIL_AND_BAIL   { Unity.CurrentTestFailed  = 1; UNITY_OUTPUT_CHAR('\n'); longjmp(Unity.AbortFrame, 1); }
#define UNITY_IGNORE_AND_BAIL { Unity.CurrentTestIgnored = 1; UNITY_OUTPUT_CHAR('\n'); longjmp(Unity.AbortFrame, 1); }

struct _Unity Unity = { 0 };

const char* UnityStrNull     = "NULL";
const char* UnityStrExpected = " Expected ";
const char* UnityStrWas      = " Was ";
const char* UnityStrTo       = " To ";
const char* UnityStrElement  = " Element ";
const char* UnityStrMemory   = " Memory Mismatch";
const char* UnityStrDelta    = " Values Not Within Delta ";
const char* UnityStrPointless= " You Asked Me To Compare Nothing, Which Was Pointless.";
const char* UnityStrSpacer   = ". ";

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
	          // printable characters plus CR & LF are printed
						if ( (*pch <= 126) && (*pch >= 32) || (*pch == 13) || (*pch == 10) )
						{
            		UNITY_OUTPUT_CHAR(*pch);
						}
						// unprintable characters are shown as codes
						else
						{
						    UNITY_OUTPUT_CHAR('{');
							  UNITY_OUTPUT_CHAR('\\');
								UnityPrintNumber((const _US32)*pch);
							  UNITY_OUTPUT_CHAR('}');
						}
            pch++;
        }
    }
}

//-----------------------------------------------
void UnityPrintNumberByStyle(const _US32 number, const UNITY_DISPLAY_STYLE_T style)
{
    switch (style)
    {
        case UNITY_DISPLAY_STYLE_HEX8:    UnityPrintNumberHex((_UU32)number, 2);           break;
        case UNITY_DISPLAY_STYLE_HEX16:   UnityPrintNumberHex((_UU32)number, 4);           break;
        case UNITY_DISPLAY_STYLE_HEX32:   UnityPrintNumberHex((_UU32)number, 8);           break;
        case UNITY_DISPLAY_STYLE_UINT:    UnityPrintNumberUnsigned((_UU32)number);         break;
        default:                          UnityPrintNumber(number);                        break;
    }
}

//-----------------------------------------------
/// basically do an itoa using as little ram as possible
void UnityPrintNumber(const _US32 number_to_print)
{
    _US32 divisor = 1;
    _US32 next_divisor;
    _US32 number = number_to_print;

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

//-----------------------------------------------
/// basically do an itoa using as little ram as possible
void UnityPrintNumberUnsigned(const _UU32 number)
{
    _UU32 divisor = 1;
    _UU32 next_divisor;

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

//-----------------------------------------------
void UnityPrintNumberHex(const _UU32 number, const char nibbles_to_print)
{
    _UU32 nibble;
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

//-----------------------------------------------
void UnityPrintMask(const _UU32 mask, const _UU32 number)
{
    _UU32 bit = 0x80000000;
    _US32 i;

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

//-----------------------------------------------
#ifdef UNITY_FLOAT_VERBOSE
void UnityPrintFloat(_UF number)
{
    char TempBuffer[32];
    sprintf(TempBuffer, "%.6f", number);
    UnityPrint(TempBuffer);
}
#endif

//-----------------------------------------------
void UnityTestResultsBegin(const char* file, const UNITY_LINE_TYPE line)
{
    UnityPrint(file);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber(line);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint(Unity.CurrentTestName);
    UNITY_OUTPUT_CHAR(':');
}

//-----------------------------------------------
void UnityTestResultsFailBegin(const UNITY_LINE_TYPE line)
{
    UnityTestResultsBegin(Unity.TestFile, line);
    UnityPrint("FAIL:");
}

//-----------------------------------------------
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

//-----------------------------------------------
void UnityAddMsgIfSpecified(const char* msg)
{
    if (msg)
    {
        UnityPrint(UnityStrSpacer);
        UnityPrint(msg);
    }
}

//-----------------------------------------------
void UnityPrintExpectedAndActualStrings(const char* expected, const char* actual)
{
		UnityPrint(UnityStrExpected);
		if (expected != NULL)
		{
		    UNITY_OUTPUT_CHAR('\'');
				UnityPrint(expected);
		    UNITY_OUTPUT_CHAR('\'');
		}
		else
		{
		  UnityPrint(UnityStrNull);					
		}
		UnityPrint(UnityStrWas);
		if (actual != NULL)
		{
		    UNITY_OUTPUT_CHAR('\'');
				UnityPrint(actual);
		    UNITY_OUTPUT_CHAR('\'');
		}
		else
		{
		  UnityPrint(UnityStrNull);					
		}
}


//-----------------------------------------------
// Assertion Functions
//-----------------------------------------------

void UnityAssertBits(const _US32 mask,
                     const _US32 expected,
                     const _US32 actual,
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

//-----------------------------------------------
void UnityAssertEqualNumber(const _US32 expected,
                            const _US32 actual,
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

//-----------------------------------------------
void UnityAssertEqualIntArray(const int* expected,
                              const int* actual,
                              const _UU32 num_elements,
                              const char* msg,
                              const UNITY_LINE_TYPE lineNumber,
                              const UNITY_DISPLAY_STYLE_T style)
{
    _UU32 elements = num_elements;
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
        case UNITY_DISPLAY_STYLE_INT8:
        case UNITY_DISPLAY_STYLE_UINT8:
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
        case UNITY_DISPLAY_STYLE_INT16:
        case UNITY_DISPLAY_STYLE_UINT16:
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

//-----------------------------------------------
#ifndef UNITY_EXCLUDE_FLOAT
void UnityAssertEqualFloatArray(const _UF* expected,
                                const _UF* actual,
                                const _UU32 num_elements,
                                const char* msg,
                                const UNITY_LINE_TYPE lineNumber)
{
    _UU32 elements = num_elements;
    const _UF* ptr_expected = expected;
    const _UF* ptr_actual = actual;
    _UF diff, tol;

    if (elements == 0)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrPointless);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }

    while (elements--)
    {
        diff = *ptr_expected - *ptr_actual;
        if (diff < 0.0)
          diff = 0.0 - diff;
        tol = UNITY_FLOAT_PRECISION * *ptr_expected;
        if (tol < 0.0)
            tol = 0.0 - tol;
        if (diff > tol)
        {
            UnityTestResultsFailBegin(lineNumber);
            UnityPrint(UnityStrElement);
            UnityPrintNumberByStyle((num_elements - elements - 1), UNITY_DISPLAY_STYLE_UINT);
#ifdef UNITY_FLOAT_VERBOSE
            UnityPrint(UnityStrExpected);
            UnityPrintFloat(*ptr_expected);
            UnityPrint(UnityStrWas);
            UnityPrintFloat(*ptr_actual);
#else
            UnityPrint(UnityStrDelta);
#endif
            UnityAddMsgIfSpecified(msg);
            UNITY_FAIL_AND_BAIL;
        }
        ptr_expected++;
        ptr_actual++;
    }
}

//-----------------------------------------------
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
#ifdef UNITY_FLOAT_VERBOSE
        UnityPrint(UnityStrExpected);
        UnityPrintFloat(expected);
        UnityPrint(UnityStrWas);
        UnityPrintFloat(actual);
#else
        UnityPrint(UnityStrDelta);
#endif
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}
#endif

//-----------------------------------------------
void UnityAssertNumbersWithin( const _US32 delta,
                               const _US32 expected,
                               const _US32 actual,
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
        if ((_UU32)actual > (_UU32)expected)
            Unity.CurrentTestFailed = ((_UU32)(actual - expected) > (_UU32)delta);
        else
            Unity.CurrentTestFailed = ((_UU32)(expected - actual) > (_UU32)delta);
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

//-----------------------------------------------
void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber)
{
    _UU32 i;

    // if both pointers not null compare the strings
    if (expected && actual)
    {
        for (i = 0; expected[i] || actual[i]; i++)
        {
            if (expected[i] != actual[i])
            {
                Unity.CurrentTestFailed = 1;
                break;
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
			UnityPrintExpectedAndActualStrings(expected, actual);
      UnityAddMsgIfSpecified(msg);
      UNITY_FAIL_AND_BAIL;
    }
}

//-----------------------------------------------
void UnityAssertEqualStringArray( const char** expected,
                                  const char** actual,
                                  const _UU32 num_elements,
                                  const char* msg,
                                  const UNITY_LINE_TYPE lineNumber)
{
    _UU32 i, j = 0;
    
    // if no elements, it's an error
    if (num_elements == 0)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrPointless);
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }

    // if both are null, it passes. if one is null, it fails
    if ((expected == NULL) || (actual == NULL))
    {
        if (expected == actual)
        {
            return;
        }
        else
        {
            Unity.CurrentTestFailed = 1;
            UNITY_FAIL_AND_BAIL;
        }
    }
    
    do
    {
        // if both pointers not null compare the strings
        if (expected[j] && actual[j])
        {
            for (i = 0; expected[j][i] || actual[j][i]; i++)
            {
                if (expected[j][i] != actual[j][i])
                {
                    Unity.CurrentTestFailed = 1;
                    break;
                }
            }
        }
        else
        { // handle case of one pointers being null (if both null, test should pass)
            if (expected[j] != actual[j])
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
                UnityPrintNumberByStyle((num_elements - j - 1), UNITY_DISPLAY_STYLE_UINT);
            }
						UnityPrintExpectedAndActualStrings(expected[j], actual[j]);
            UnityAddMsgIfSpecified(msg);
            UNITY_FAIL_AND_BAIL;
        } 
    } while (++j < num_elements);
}

//-----------------------------------------------
void UnityAssertEqualMemory( const void* expected,
                             const void* actual,
                             _UU32 length,
                             _UU32 num_elements,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber)
{
	  unsigned char* expected_ptr = (unsigned char*)expected;
	  unsigned char* actual_ptr = (unsigned char*)actual;
    _UU32 elements = num_elements;
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

//-----------------------------------------------
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

//-----------------------------------------------
void UnityBegin()
{
    Unity.NumberOfTests = 0;
}

//-----------------------------------------------
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
