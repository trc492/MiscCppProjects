#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="cell.cpp" />
///
/// <summary>
///     This module contains the implementation of the Cell class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "cell.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param row Specifies the row index of the cell object.
 *  @param col Specifies the col index of the cell object.
 *  @param maxValue Specifies the maximum value of the cell data.
 */
Cell::Cell(
    __in int    row,
    __in int    col,
    __in int    maxValue
    ): m_row(row),
       m_col(col),
       m_maxValue(maxValue)
{
    TEnter(CELL, WPPIndent, "row=%d,col=%d,maxValue=%d",
           row, col, maxValue);

    TAssert(m_maxValue <= MAX_SUPPORTED_VALUE);
    TAssert((row >= 0) && (row < m_maxValue));
    TAssert((col >= 0) && (col < m_maxValue));
    m_value = 0;
    m_cntChoices = m_maxValue;
    m_bitChoices = ((ULONGLONG)1 << m_maxValue) - 1;
    m_dwFlags = 0;

    TExitVoid(CELL, WPPIndent);
    return;
}   //Cell

/**
 *  Destructor to destroy an instance of the object.
 */
Cell::~Cell(
    void
    )
{
    TEnterVoid(CELL, WPPIndent);
    TExitVoid(CELL, WPPIndent);
    return;
}   //~Cell

/**
 *  This function returns the value of the cell.
 *
 *  @return Returns the value of the cell.
 */
int
Cell::GetValue(
    void
    )
{
    TEnterVoid(CELL, WPPIndent);
    TExitDec(CELL, WPPIndent, m_value);
    return m_value;
}   //GetValue

/**
 *  This function sets the value of the cell.
 *
 *  @param value Specifies the value of the cell.
 *  @param dwFlags Specifies the flags to set.
 *
 *  @return Returns TRUE if the function is successful, FALSE otherwise.
 */
BOOL
Cell::SetValue(
    __in int value,
    __in DWORD dwFlags
    )
{
    BOOL rc = TRUE;

    TEnter(CELL, WPPIndent, "value=%d,flags=%x", value, dwFlags);

    if ((value <= 0) || (value > m_maxValue))
    {
        rc = FALSE;
        TMsg(Err, "Invalid value %d.", value);
    }
    else if (m_value != 0)
    {
        rc = FALSE;
        TMsg(Err, "Cell[%d,%d]=%d is already solved.",
             m_row, m_col, m_value);
    }
    else
    {
        m_value = value;
        m_bitChoices = 0;
        m_cntChoices = 0;
        m_dwFlags |= dwFlags;
    }

    TExitDec(CELL, WPPIndent, rc);
    return rc;
}   //SetValue

/**
 *  This function returns the choices of a cell.
 *
 *  @param pcntChoices Points to the variable to receive the number of
 *         choices of a cell.
 *
 *  @return Returns the choice bits of the cell.
 */
ULONGLONG
Cell::GetChoices(
    __out_opt int *pcntChoices
    )
{
    TEnter(CELL, WPPIndent, "pcntChoices=%p", pcntChoices);

    if (pcntChoices != NULL)
    {
        *pcntChoices = m_cntChoices;
    }

    TExitDecMsg(CELL, WPPIndent, m_cntChoices,
                " (Choices=%I64x)", m_bitChoices);
    return m_bitChoices;
}   //GetChoices

/**
 *  This function excludes the given choice bit mask for the cell.
 *
 *  @param bitChoices Specifies the choice bit mask.
 *
 *  @return Returns the new choices of the cell.
 */
ULONGLONG
Cell::ExcludeChoices(
    __in ULONGLONG bitChoices
    )
{
    int cntChoices;

    TEnter(CELL, WPPIndent, "bitChoices=%I64x", bitChoices);

    bitChoices &= m_bitChoices;
    cntChoices = CountSetBits(bitChoices);
    if (cntChoices > 0)
    {
        m_bitChoices &= ~bitChoices;
        m_cntChoices -= cntChoices;
    }

    TExitDecMsg(CELL, WPPIndent, m_cntChoices,
                " (Choices=%I64x)", m_bitChoices);
    return m_bitChoices;
}   //ExcludeChoices

/**
 *  This function returns the flags of the cell.
 *
 *  @return Returns the cell flags.
 */
DWORD
Cell::GetFlags(
    void
    )
{
    TEnterVoid(CELL, WPPIndent);
    TExitHex(CELL, WPPIndent, m_dwFlags);
    return m_dwFlags;
}   //GetFlags

/**
 *  This function set the flag bits of the cell.
 *
 *  @param flags Specifies the flag bits to set.
 *
 *  @return Returns the updated cell flags.
 */
DWORD
Cell::SetFlags(
    __in DWORD flags
    )
{
    TEnter(CELL, WPPIndent, "flags=%x", flags);
    m_dwFlags |= flags;
    TExitHex(CELL, WPPIndent, m_dwFlags);
    return m_dwFlags;
}   //SetFlags

