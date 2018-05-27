#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="util.cpp" />
///
/// <summary>
///     This module contains the utility functions.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "util.tmh"
#endif

/**
 *  This function returns the number of set bits in the given data.
 *
 *  @param data Specifies the data.
 *
 *  @return Returns the number of set bits.
 */
int
CountSetBits(
    __in ULONGLONG data
    )
{
    int cBits = 0;

    TEnter(Util, WPPIndent, "data=%I64x", data);

    while (data != 0)
    {
        data = (-((LONGLONG)data)^data) & data;
        cBits++;
    }

    TExitDec(Util, WPPIndent, cBits);
    return cBits;
}   //CountSetBits

/**
 *  This function returns the bit position of the least significant set bit.
 *
 *  @param data Specifies the data.
 *  @param numBits Specifies the maximum number of bits in data.
 *
 *  @return Returns bit position of the least significant set bit.
 */
int
LSBitPosition(
    __in ULONGLONG data,
    __in int numBits
    )
{
    int bitPosition;

    TEnter(Util, WPPIndent, "data=%I64x", data);

    TAssert(data != 0);
    for (bitPosition = 0; bitPosition < numBits; bitPosition++)
    {
        if (data & ((ULONGLONG)1 << bitPosition))
        {
            break;
        }
    }

    TExitDec(Util, WPPIndent, bitPosition);
    return bitPosition;
}   //LSBitPosition

/**
 *  This function prints the choices with the given bit mask.
 *
 *  @param bitChoices Specifies the choice bit mask.
 *  @param maxChoices Specifies the maximum number of bits.
 */
void
PrintChoices(
    __in ULONGLONG bitChoices,
    __in int maxChoices
    )
{
    TEnter(Util, WPPIndent, "bitChoices=%I64x,maxChoices=%d",
           bitChoices, maxChoices);

    for (int i = 0; i < maxChoices; i++)
    {
        if (bitChoices & ((ULONGLONG)1 << i))
        {
            wprintf(L" %d", i + 1);
        }
    }

    TExitVoid(Util, WPPIndent);
    return;
}   //PrintChoices

