#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Cell.h" />
///
/// <summary>
///     This module contains the definitions of the Cell class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

#define CELLF_SOLVED            0x80000000

class Cell
{
private:
    int         m_row;
    int         m_col;
    int         m_maxValue;
    int         m_value;
    int         m_cntChoices;
    ULONGLONG   m_bitChoices;
    DWORD       m_dwFlags;

public:
    Cell(
        __in int    row,
        __in int    col,
        __in int    maxValue
        );

    ~Cell(
        void
        );

    int
    GetValue(
        void
        );

    BOOL
    SetValue(
        __in int value,
        __in DWORD dwFlags
        );

    ULONGLONG
    GetChoices(
        __out_opt int *pcntChoices
        );

    ULONGLONG
    ExcludeChoices(
        __in ULONGLONG bitChoices
        );

    DWORD
    GetFlags(
        void
        );

    DWORD
    SetFlags(
        __in DWORD flags
        );
};

