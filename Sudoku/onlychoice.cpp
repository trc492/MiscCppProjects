#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="OnlyChoice.cpp" />
///
/// <summary>
///     This module contains the implementation of the OnlyChoice class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "onlychoice.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param dimRegion Specifies the dimension of the region.
 */
OnlyChoice::OnlyChoice(
    __in int dimRegion
    ): Rule(dimRegion)
{
    TEnterVoid(RULE, WPPIndent);
    m_idxRule = LSBitPosition(CELLF_ONLYCHOICE, MAX_NUM_RULES);
    TExitVoid(RULE, WPPIndent);
    return;
}   //OnlyChoice

/**
 *  Destructor to destroy an instance of the object.
 */
OnlyChoice::~OnlyChoice(
    void
    )
{
    TEnterVoid(RULE, WPPIndent);
    TExitVoid(RULE, WPPIndent);
    return;
}   //~OnlyChoice

/**
 *  This function process the cell with the OnlyChoice rule.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *
 *  @return Returns TRUE if the cell is solved, FALSE otherwise.
 */
BOOL
OnlyChoice::ProcessCell(
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
    if (cntChoices == 1)
    {
        board->SetCellValue(row,
                            col,
                            LSBitPosition(bitChoices, m_dimBoard) + 1,
                            CELLF_SOLVED | CELLF_ONLYCHOICE);
        if (g_dwfSudoku & SF_VERBOSE)
        {
            wprintf(L"Found OnlyChoice: [%d,%d] =", row + 1, col + 1);
            PrintChoices(bitChoices, m_dimBoard);
            wprintf(L"\n");
        }
        rc = TRUE;
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
OnlyChoice::PrintSummary(
    __in Board *board
    )
{
    TEnter(RULE, WPPIndent, "board=%p", board);

    wprintf(L"   Solved by OnlyChoice rule = %d\n",
            board->GetRuleFinds(m_idxRule));
    wprintf(L" Excluded by OnlyChoice rule = %d\n",
            board->GetRuleExcludes(m_idxRule));

    TExitVoid(RULE, WPPIndent);
    return;
}   //PrintSummary

