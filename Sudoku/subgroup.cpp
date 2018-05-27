#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Subgroup.cpp" />
///
/// <summary>
///     This module contains the implementation of the Subgroup class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 17-July-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "subgroup.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param dimRegion Specifies the dimension of the region.
 */
Subgroup::Subgroup(
    __in int dimRegion
    ): Rule(dimRegion)
{
    TEnterVoid(RULE, WPPIndent);
    m_idxRule = LSBitPosition(CELLF_SUBGROUP, MAX_NUM_RULES);
    TExitVoid(RULE, WPPIndent);
    return;
}   //Subgroup

/**
 *  Destructor to destroy an instance of the object.
 */
Subgroup::~Subgroup(
    void
    )
{
    TEnterVoid(RULE, WPPIndent);
    TExitVoid(RULE, WPPIndent);
    return;
}   //~Subgroup

/**
 *  This function process the cell with the Subgroup rule.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *
 *  @return Returns TRUE if the cell is solved, FALSE otherwise.
 */
BOOL
Subgroup::ProcessCell(
    __in Board *board,
    __in int row,
    __in int col
    )
{
    BOOL rc = FALSE;
    ULONGLONG bitChoices;
    int cntChoices;

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
            //
            // Once we found a subgroup for a bit, there is no need to look
            // for the same bit in other directions because that choice
            // should have been cleared out everywhere.
            //
            if (!SubgroupInRow(board, row, col, bitMask))
            {
                if (!SubgroupInCol(board, row, col, bitMask))
                {
                    SubgroupInRegion(board, row, col, bitMask);
                }
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
Subgroup::PrintSummary(
    __in Board *board
    )
{
    TEnter(RULE, WPPIndent, "board=%p", board);

    wprintf(L"   Number of Subgroups found = %d\n",
            board->GetRuleFinds(m_idxRule));
    wprintf(L"   Excluded by Subgroup rule = %d\n",
            board->GetRuleExcludes(m_idxRule));

    TExitVoid(RULE, WPPIndent);
    return;
}   //PrintSummary

/**
 *  This function determines if a given bit is part of a subgroup in the row.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the bit mask of the choice to check.
 *
 *  @return Returns TRUE if the bit is part of the Subgroup in the row,
 *          FALSE otherwise.
 */
BOOL
Subgroup::SubgroupInRow(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    BOOL rc = FALSE;
    ULONGLONG bits = 0;
    ULONGLONG maskRegion = ((ULONGLONG)1 << m_dimRegion) - 1;
    int numBits;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int c = 0; c < m_dimBoard; c++)
    {
        if (c != col)
        {
            if (bitChoice & board->GetCellChoices(row, c, NULL))
            {
                bits |= (ULONGLONG)1 << c;
            }
        }
    }

    maskRegion <<= (col/m_dimRegion)*m_dimRegion;
    numBits = CountSetBits(bits);
    if ((numBits >= 1) && (numBits < m_dimRegion) &&
        ((bits & ~maskRegion) == 0))
    {
        //
        // Found a choice in the row that only occurs in the same region.
        //
        rc = TRUE;
        if (g_dwfSudoku & SF_VERBOSE)
        {
            wprintf(L"Found SubgroupInRow: [%d,%d] =",
                    row + 1, col + 1);
            PrintChoices(bitChoice, m_dimBoard);
            wprintf(L"\n");
        }
        //
        // We totally overcounted this rule. We counted both this one and its
        // partner and we counted them again in the next iteration. We cannot
        // use the trick other rules used that marks the cells and skip the
        // processing of the cells that are already marked subgroup because a
        // cell can be subgroups for multiple choices. So we cannot skip
        // processing a cell even if it was already marked a subgroup because
        // it may be a subgroup for a different choice. It is not easy to
        // count this rule correctly without keeping track of subgroup state
        // of all choices of a cell.
        //
        board->SetCellFlags(row, col, CELLF_ROW_SUBGROUP);
        for (int c = (col/m_dimRegion)*m_dimRegion;
             c < (col/m_dimRegion + 1)*m_dimRegion;
             c++)
        {
            if (bits & ((ULONGLONG)1 << c))
            {
                board->SetCellFlags(row, c, CELLF_ROW_SUBGROUP);
            }
        }
        board->IncrementRuleFinds(m_idxRule);
        //
        // Exclude the subgroup bit from the rest of the region.
        //
        for (int r = (row/m_dimRegion)*m_dimRegion;
             (r < (row/m_dimRegion + 1)*m_dimRegion);
             r++)
        {
            if (r == row)
            {
                continue;
            }

            for (int c = (col/m_dimRegion)*m_dimRegion;
                 (c < (col/m_dimRegion + 1)*m_dimRegion);
                 c++)
            {
                if (board->ExcludeCellChoices(r, c, bitChoice))
                {
                    board->IncrementRuleExcludes(m_idxRule);
                }
            }
        }
    }

    TExitDec(RULE, WPPIndent, rc);
    return rc;
}   //SubgroupInRow

/**
 *  This function determines if a given bit is part of a subgroup in the col.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the bit mask of the choice to check.
 *
 *  @return Returns TRUE if the bit is part of the Subgroup in the col,
 *          FALSE otherwise.
 */
BOOL
Subgroup::SubgroupInCol(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    BOOL rc = FALSE;
    ULONGLONG bits = 0;
    ULONGLONG maskRegion = ((ULONGLONG)1 << m_dimRegion) - 1;
    int numBits;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int r = 0; r < m_dimBoard; r++)
    {
        if (r != row)
        {
            if (bitChoice & board->GetCellChoices(r, col, NULL))
            {
                bits |= (ULONGLONG)1 << r;
            }
        }
    }

    maskRegion <<= (row/m_dimRegion)*m_dimRegion;
    numBits = CountSetBits(bits);
    if ((numBits >= 1) && (numBits < m_dimRegion) &&
        ((bits & ~maskRegion) == 0))
    {
        //
        // Found a choice in the col that only occurs in the same region.
        //
        rc = TRUE;
        if (g_dwfSudoku & SF_VERBOSE)
        {
            wprintf(L"Found SubgroupInCol: [%d,%d] =",
                    row + 1, col + 1);
            PrintChoices(bitChoice, m_dimBoard);
            wprintf(L"\n");
        }
        //
        // We totally overcounted this rule. We counted both this one and its
        // partner and we counted them again in the next iteration. We cannot
        // use the trick other rules used that marks the cells and skip the
        // processing of the cells that are already marked subgroup because a
        // cell can be subgroups for multiple choices. So we cannot skip
        // processing a cell even if it was already marked a subgroup because
        // it may be a subgroup for a different choice. It is not easy to
        // count this rule correctly without keeping track of subgroup state
        // of all choices of a cell.
        //
        board->SetCellFlags(row, col, CELLF_COL_SUBGROUP);
        for (int r = (row/m_dimRegion)*m_dimRegion;
             r < (row/m_dimRegion + 1)*m_dimRegion;
             r++)
        {
            if (bits & ((ULONGLONG)1 << r))
            {
                board->SetCellFlags(r, col, CELLF_COL_SUBGROUP);
            }
        }
        board->IncrementRuleFinds(m_idxRule);
        //
        // Exclude the subgroup bit from the rest of the region.
        //
        for (int r = (row/m_dimRegion)*m_dimRegion;
             (r < (row/m_dimRegion + 1)*m_dimRegion);
             r++)
        {
            for (int c = (col/m_dimRegion)*m_dimRegion;
                 (c < (col/m_dimRegion + 1)*m_dimRegion);
                 c++)
            {
                if (c == col)
                {
                    continue;
                }
                if (board->ExcludeCellChoices(r, c, bitChoice))
                {
                    board->IncrementRuleExcludes(m_idxRule);
                }
            }
        }
    }

    TExitDec(RULE, WPPIndent, rc);
    return rc;
}   //SubgroupInCol

/**
 *  This function determines if a given bit is part of a subgroup in the
 *  region.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the bit mask of the choice to check.
 *
 *  @return Returns TRUE if the bit is part of the Subgroup in the region,
 *          FALSE otherwise.
 */
BOOL
Subgroup::SubgroupInRegion(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    BOOL rc = TRUE;
    ULONGLONG bitsRow = 0;
    ULONGLONG bitsCol = 0;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int r = (row/m_dimRegion)*m_dimRegion;
         rc && (r < (row/m_dimRegion + 1)*m_dimRegion);
         r++)
    {
        for (int c = (col/m_dimRegion)*m_dimRegion;
             rc && (c < (col/m_dimRegion + 1)*m_dimRegion);
             c++)
        {
            if ((r != row) || (c != col))
            {
                if (bitChoice & board->GetCellChoices(r, c, NULL))
                {
                    if (r == row)
                    {
                        bitsCol |= (ULONGLONG)1 << c;
                    }
                    else if (c == col)
                    {
                        bitsRow |= (ULONGLONG)1 << r;
                    }
                    else
                    {
                        rc = FALSE;
                    }
                }
            }
        }
    }

    if (rc)
    {
        if ((bitsRow != 0) && (bitsCol == 0) ||
            (bitsRow == 0) && (bitsCol != 0))
        {
            //
            // Found a choice in the region that only occurs in the same row
            // or the same col.
            //
            if (g_dwfSudoku & SF_VERBOSE)
            {
                wprintf(L"Found SubgroupInRegion: [%d,%d] =",
                        row + 1, col + 1);
                PrintChoices(bitChoice, m_dimBoard);
                wprintf(L"\n");
            }
            //
            // We totally overcounted this rule. We counted both this one and
            // its partner and we counted them again in the next iteration.
            // We cannot use the trick other rules used that marks the cells
            // and skip the processing of the cells that are already marked
            // subgroup because a cell can be subgroups for multiple choices.
            // So we cannot skip processing a cell even if it was already
            // marked a subgroup because it may be a subgroup for a different
            // choice. It is not easy to count this rule correctly without
            // keeping track of subgroup state of all choices of a cell.
            //
            board->SetCellFlags(row, col, CELLF_REGION_SUBGROUP);
            if (bitsRow != 0)
            {
                for (int r = (row/m_dimRegion)*m_dimRegion;
                     r < (row/m_dimRegion + 1)*m_dimRegion;
                     r++)
                {
                    if (bitsRow & ((ULONGLONG)1 << r))
                    {
                        board->SetCellFlags(r, col, CELLF_REGION_SUBGROUP);
                    }
                }
            }
            else
            {
                for (int c = (col/m_dimRegion)*m_dimRegion;
                     c < (col/m_dimRegion + 1)*m_dimRegion;
                     c++)
                {
                    if (bitsCol & ((ULONGLONG)1 << c))
                    {
                        board->SetCellFlags(row, c, CELLF_REGION_SUBGROUP);
                    }
                }
            }
            board->IncrementRuleFinds(m_idxRule);
            //
            // Exclude the subgroup bit from the rest of the region.
            //
            if (bitsRow != 0)
            {
                for (int r = 0; r < m_dimBoard; r++)
                {
                    if ((r != row) &&
                        ((bitsRow & ((ULONGLONG)1 << r)) == 0))
                    {
                        if (board->ExcludeCellChoices(r, col, bitChoice))
                        {
                            board->IncrementRuleExcludes(m_idxRule);
                        }
                    }
                }
            }
            else
            {
                for (int c = 0; c < m_dimBoard; c++)
                {
                    if ((c != col) &&
                        ((bitsCol & ((ULONGLONG)1 << c)) == 0))
                    {
                        if (board->ExcludeCellChoices(row, c, bitChoice))
                        {
                            board->IncrementRuleExcludes(m_idxRule);
                        }
                    }
                }
            }
        }
        else
        {
            rc = FALSE;
        }
    }

    TExitDec(RULE, WPPIndent, rc);
    return rc;
}   //SubgroupInRegion

