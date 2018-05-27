#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="NowhereElse.cpp" />
///
/// <summary>
///     This module contains the implementation of the NowhereElse class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "nowhereelse.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param dimRegion Specifies the dimension of the region.
 */
NowhereElse::NowhereElse(
    __in int dimRegion
    ): Rule(dimRegion)
{
    TEnterVoid(RULE, WPPIndent);
    m_idxRule = LSBitPosition(CELLF_NOWHEREELSE, MAX_NUM_RULES);
    TExitVoid(RULE, WPPIndent);
    return;
}   //NowhereElse

/**
 *  Destructor to destroy an instance of the object.
 */
NowhereElse::~NowhereElse(
    void
    )
{
    TEnterVoid(RULE, WPPIndent);
    TExitVoid(RULE, WPPIndent);
    return;
}   //~NowhereElse

/**
 *  This function process the cell with the NowhereElse rule.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *
 *  @return Returns TRUE if the cell is solved, FALSE otherwise.
 */
BOOL
NowhereElse::ProcessCell(
    __in Board *board,
    __in int row,
    __in int col
    )
{
    BOOL rc = FALSE;
    int cntChoices;
    ULONGLONG bitChoices;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d", board, row, col);

    bitChoices = board->GetCellChoices(row, col, &cntChoices);
    if (cntChoices > 1)
    {
        ULONGLONG bits;
        ULONGLONG bitMask;

        for (bits = bitChoices; bits != 0; bits &= ~bitMask)
        {
            bitMask = (-(LONGLONG)bits)^bits;
            bitMask = ~bitMask & bits;
            bits &= ~bitMask;
            if (NowhereElseInRow(board, row, col, bitMask) ||
                NowhereElseInCol(board, row, col, bitMask) ||
                NowhereElseInRegion(board, row, col, bitMask))
            {
                board->SetCellValue(row,
                                    col,
                                    LSBitPosition(bitMask, m_dimBoard) + 1,
                                    CELLF_SOLVED | CELLF_NOWHEREELSE);
                rc = TRUE;
                break;
            }
        }
    }

    TExitDec(RULE, WPPIndent, rc);
    return rc;
}   //ProcessCell

/**
 *  This function prints the rule summary.
 *
 *  @param board Points to the board object.
 */
void
NowhereElse::PrintSummary(
    __in Board *board
    )
{
    TEnter(RULE, WPPIndent, "board=%p", board);

    wprintf(L"  Solved by NowhereElse rule = %d\n",
            board->GetRuleFinds(m_idxRule));
    wprintf(L"Excluded by NoWhereElse rule = %d\n",
            board->GetRuleExcludes(m_idxRule));

    TExitVoid(RULE, WPPIndent);
    return;
}   //PrintSummary

/**
 *  This function determines if a given choice is nowhere else in the row.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the bit mask of the choice to check.
 *
 *  @return Returns TRUE if the choice is nowhere else in the row,
 *          FALSE otherwise.
 */
BOOL
NowhereElse::NowhereElseInRow(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    BOOL fFound = TRUE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int c = 0; c < m_dimBoard; c++)
    {
        if (c != col)
        {
            if (bitChoice & board->GetCellChoices(row, c, NULL))
            {
                fFound = FALSE;
                break;
            }
        }
    }

    if (fFound && (g_dwfSudoku & SF_VERBOSE))
    {
        wprintf(L"Found NowhereElseInRow: [%d,%d] =", row + 1, col + 1);
        PrintChoices(bitChoice, m_dimBoard);
        wprintf(L"\n");
    }

    TExitDec(RULE, WPPIndent, fFound);
    return fFound;
}   //NowhereElseInRow

/**
 *  This function determines if a given choice is nowhere else in the column.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the bit mask of the choice to check.
 *
 *  @return Returns TRUE if the choice is nowhere else in the column,
 *          FALSE otherwise.
 */
BOOL
NowhereElse::NowhereElseInCol(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    BOOL fFound = TRUE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int r = 0; r < m_dimBoard; r++)
    {
        if (r != row)
        {
            if (bitChoice & board->GetCellChoices(r, col, NULL))
            {
                fFound = FALSE;
                break;
            }
        }
    }

    if (fFound && (g_dwfSudoku & SF_VERBOSE))
    {
        wprintf(L"Found NowhereElseInCol: [%d,%d] =", row + 1, col + 1);
        PrintChoices(bitChoice, m_dimBoard);
        wprintf(L"\n");
    }

    TExitDec(RULE, WPPIndent, fFound);
    return fFound;
}   //NowhereElseInCol

/**
 *  This function determines if a given choice is nowhere else in the region.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the bit mask of the choice to check.
 *
 *  @return Returns TRUE if the choice is nowhere else in the region,
 *          FALSE otherwise.
 */
BOOL
NowhereElse::NowhereElseInRegion(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    BOOL fFound = TRUE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int r = (row/m_dimRegion)*m_dimRegion;
         fFound && (r < (row/m_dimRegion + 1)*m_dimRegion);
         r++)
    {
        for (int c = (col/m_dimRegion)*m_dimRegion;
             fFound && (c < (col/m_dimRegion + 1)*m_dimRegion);
             c++)
        {
            if ((r != row) || (c != col))
            {
                if (bitChoice & board->GetCellChoices(r, c, NULL))
                {
                    fFound = FALSE;
                }
            }
        }
    }

    if (fFound && (g_dwfSudoku & SF_VERBOSE))
    {
        wprintf(L"Found NowhereElseInRegion: [%d,%d] =", row + 1, col + 1);
        PrintChoices(bitChoice, m_dimBoard);
        wprintf(L"\n");
    }

    TExitDec(RULE, WPPIndent, fFound);
    return fFound;
}   //NowhereElseInRegion

