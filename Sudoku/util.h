#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Sudoku.h" />
///
/// <summary>
///     This module contains definitions of the utility functions.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

//
// Function prototypes.
//
int
CountSetBits(
    __in ULONGLONG data
    );

int
LSBitPosition(
    __in ULONGLONG data,
    __in int numBits
    );

void
PrintChoices(
    __in ULONGLONG bitChoices,
    __in int maxChoices
    );

