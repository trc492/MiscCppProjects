#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Twins.cpp" />
///
/// <summary>
///     This module contains the implementation of the Twins class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "twins.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param dimRegion Specifies the dimension of the region.
 */
Twins::Twins(
    __in int dimRegion
    ): Rule(dimRegion)
{
    TEnterVoid(RULE, WPPIndent);
    m_idxRule = LSBitPosition(CELLF_TWINS, MAX_NUM_RULES);
    TExitVoid(RULE, WPPIndent);
    return;
}   //Twins

/**
 *  Destructor to destroy an instance of the object.
 */
Twins::~Twins(
    void
    )
{
    TEnterVoid(RULE, WPPIndent);
    TExitVoid(RULE, WPPIndent);
    return;
}   //~Twins

/**
 *  This function process the cell with the Twins rule.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *
 *  @return Returns TRUE if the cell is solved, FALSE otherwise.
 */
BOOL
Twins::ProcessCell(
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
    if (cntChoices >= 2)
    {
        //
        // Process the cell only if the cell is not solved.
        //
        TwinsInRow(board, row, col, bitChoices);
        TwinsInCol(board, row, col, bitChoices);
        TwinsInRegion(board, row, col, bitChoices);
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
Twins::PrintSummary(
    __in Board *board
    )
{
    TEnter(RULE, WPPIndent, "board=%p", board);

    wprintf(L"       Number of Twins found = %d\n",
            board->GetRuleFinds(m_idxRule));
    wprintf(L"      Excluded by Twins rule = %d\n",
            board->GetRuleExcludes(m_idxRule));

    TExitVoid(RULE, WPPIndent);
    return;
}   //PrintSummary

/**
 *  This function determines if a given cell is a twin in the row.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param cellChoices Specifies the choices of the cell.
 *
 *  @return Returns TRUE if the cell is part of the twins in the row,
 *          FALSE otherwise.
 */
BOOL
Twins::TwinsInRow(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG cellChoices
    )
{
    BOOL fFoundNakedTwins = FALSE;
    BOOL fFoundHiddenTwins = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,cellChoices=%I64x",
           board, row, col, cellChoices);

    if (!(board->GetCellFlags(row, col) & CELLF_ROW_TWINS))
    {
        for (int c1 = 0;
             !fFoundNakedTwins && !fFoundHiddenTwins && (c1 < m_dimBoard);
             c1++)
        {
            if (c1 != col)
            {
                int cntChoices;
                ULONGLONG bitChoices;
                ULONGLONG bitTwins, bitCommon;
                int cntCommon;

                bitChoices = board->GetCellChoices(row, c1, &cntChoices);
                bitCommon = cellChoices & bitChoices;
                cntCommon = CountSetBits(bitCommon);
                if (cntCommon >= 2)
                {
                    if ((cntCommon == 2) && (cellChoices == bitChoices))
                    {
                        //
                        // Found naked twins.
                        //
                        fFoundNakedTwins = TRUE;
                        bitTwins = bitCommon;
                    }
                    else
                    {
                        ULONGLONG bit;
                        //
                        // For each of the common bits, check the rest of the
                        // row for one other occurrence.
                        //
                        bitTwins = 0;
                        while (bitCommon != 0)
                        {
                            bit = ~(-((LONGLONG)bitCommon)^bitCommon) &
                                  bitCommon;
                            bitCommon &= ~bit;
                            if (CountCellsInRowWithChoice(board,
                                                          row,
                                                          col,
                                                          bit) == 1)
                            {
                                bitTwins |= bit;
                            }
                        }

                        if (CountSetBits(bitTwins) == 2)
                        {
                            //
                            // Found hidden twins.
                            //
                            fFoundHiddenTwins = TRUE;
                        }
                    }

                    if (fFoundNakedTwins || fFoundHiddenTwins)
                    {
                        //
                        // We found a pair of twins.
                        //
                        if (g_dwfSudoku & SF_VERBOSE)
                        {
                            wprintf(L"Found %sTwinsInRow: [%d,%d]/[%d,%d] =",
                                    fFoundNakedTwins? L"Naked": L"Hidden",
                                    row + 1, col + 1, row + 1, c1 + 1);
                            PrintChoices(bitTwins, m_dimBoard);
                            wprintf(L"\n");
                        }
                        board->SetCellFlags(row, col, CELLF_ROW_TWINS);
                        board->SetCellFlags(row, c1, CELLF_ROW_TWINS);
                        board->IncrementRuleFinds(m_idxRule);

                        if (fFoundHiddenTwins)
                        {
                            //
                            // Clear all other possibilities for the twins.
                            //
                            if (board->ExcludeCellChoices(
                                            row,
                                            col,
                                            ~bitTwins & cellChoices))
                            {
                                board->IncrementRuleExcludes(m_idxRule);
                            }
                            if (board->ExcludeCellChoices(
                                            row,
                                            c1,
                                            ~bitTwins & bitChoices))
                            {
                                board->IncrementRuleExcludes(m_idxRule);
                            }
                        }
                        else if (fFoundNakedTwins)
                        {
                            //
                            // Exclude the twins from the rest of the row.
                            //
                            for (int c2 = 0; c2 < m_dimBoard; c2++)
                            {
                                if ((c2 != col) && (c2 != c1))
                                {
                                    if (board->ExcludeCellChoices(
                                                    row,
                                                    c2,
                                                    bitTwins))
                                    {
                                        board->IncrementRuleExcludes(
                                                    m_idxRule);
                                    }
                                }
                            }
                        }

                        if ((col/m_dimRegion)*m_dimRegion ==
                            (c1/m_dimRegion)*m_dimRegion)
                        {
                            //
                            // The twins are also in the same region, exclude
                            // them from the rest of the region.
                            //
                            for (int r = (row/m_dimRegion)*m_dimRegion;
                                 r < (row/m_dimRegion + 1)*m_dimRegion;
                                 r++)
                            {
                                for (int c = (col/m_dimRegion)*m_dimRegion;
                                    c < (col/m_dimRegion + 1)*m_dimRegion;
                                    c++)
                                {
                                    if (((r != row) || (c != col)) &&
                                        ((r != row) || (c != c1)))
                                    {
                                        if (board->ExcludeCellChoices(
                                                            r,
                                                            c,
                                                            bitTwins))
                                        {
                                            board->IncrementRuleExcludes(
                                                        m_idxRule);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TExitDec(RULE, WPPIndent, fFoundNakedTwins || fFoundHiddenTwins);
    return fFoundNakedTwins || fFoundHiddenTwins;
}   //TwinsInRow

/**
 *  This function determines if a given cell is a twin in the column.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param cellChoices Specifies the choices of the cell.
 *
 *  @return Returns TRUE if the cell is part of the twins in the column,
 *          FALSE otherwise.
 */
BOOL
Twins::TwinsInCol(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG cellChoices
    )
{
    BOOL fFoundNakedTwins = FALSE;
    BOOL fFoundHiddenTwins = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,cellChoices=%I64x",
           board, row, col, cellChoices);

    if (!(board->GetCellFlags(row, col) & CELLF_COL_TWINS))
    {
        for (int r1 = 0;
             !fFoundNakedTwins && !fFoundHiddenTwins && (r1 < m_dimBoard);
             r1++)
        {
            if (r1 != row)
            {
                int cntChoices;
                ULONGLONG bitChoices;
                ULONGLONG bitTwins, bitCommon;
                int cntCommon;

                bitChoices = board->GetCellChoices(r1, col, &cntChoices);
                bitCommon = cellChoices & bitChoices;
                cntCommon = CountSetBits(bitCommon);
                if (cntCommon >= 2)
                {
                    if ((cntCommon == 2) && (cellChoices == bitChoices))
                    {
                        //
                        // Found naked twins.
                        //
                        fFoundNakedTwins = TRUE;
                        bitTwins = bitCommon;
                    }
                    else
                    {
                        ULONGLONG bit;
                        //
                        // For each of the common bits, check the rest of the
                        // column for one other occurrence.
                        //
                        bitTwins = 0;
                        while (bitCommon != 0)
                        {
                            bit = ~(-((LONGLONG)bitCommon)^bitCommon) &
                                  bitCommon;
                            bitCommon &= ~bit;
                            if (CountCellsInColWithChoice(board,
                                                          row,
                                                          col,
                                                          bit) == 1)
                            {
                                bitTwins |= bit;
                            }
                        }

                        if (CountSetBits(bitTwins) == 2)
                        {
                            //
                            // Found hidden twins.
                            //
                            fFoundHiddenTwins = TRUE;
                        }
                    }

                    if (fFoundNakedTwins || fFoundHiddenTwins)
                    {
                        //
                        // We found a pair of twins.
                        //
                        if (g_dwfSudoku & SF_VERBOSE)
                        {
                            wprintf(L"Found %sTwinsInCol: [%d,%d]/[%d,%d] =",
                                    fFoundNakedTwins? L"Naked": L"Hidden",
                                    row + 1, col + 1, r1 + 1, col + 1);
                            PrintChoices(bitTwins, m_dimBoard);
                            wprintf(L"\n");
                        }
                        board->SetCellFlags(row, col, CELLF_COL_TWINS);
                        board->SetCellFlags(r1, col, CELLF_COL_TWINS);
                        board->IncrementRuleFinds(m_idxRule);

                        if (fFoundHiddenTwins)
                        {
                            //
                            // Clear all other possibilities for the twins.
                            //
                            if (board->ExcludeCellChoices(
                                                row,
                                                col,
                                                ~bitTwins & cellChoices))
                            {
                                board->IncrementRuleExcludes(m_idxRule);
                            }
                            if (board->ExcludeCellChoices(
                                                r1,
                                                col,
                                                ~bitTwins & bitChoices))
                            {
                                board->IncrementRuleExcludes(m_idxRule);
                            }
                        }
                        else if (fFoundNakedTwins)
                        {
                            //
                            // Exclude the twins from the rest of the column.
                            //
                            for (int r2 = 0; r2 < m_dimBoard; r2++)
                            {
                                if ((r2 != row) && (r2 != r1))
                                {
                                    if (board->ExcludeCellChoices(
                                                        r2,
                                                        col,
                                                        bitTwins))
                                    {
                                        board->IncrementRuleExcludes(
                                                    m_idxRule);
                                    }
                                }
                            }
                        }

                        if ((row/m_dimRegion)*m_dimRegion ==
                            (r1/m_dimRegion)*m_dimRegion)
                        {
                            //
                            // The twins are also in the same region, exclude
                            // them from the rest of the region.
                            //
                            for (int r = (row/m_dimRegion)*m_dimRegion;
                                 r < (row/m_dimRegion + 1)*m_dimRegion;
                                 r++)
                            {
                                for (int c = (col/m_dimRegion)*m_dimRegion;
                                    c < (col/m_dimRegion + 1)*m_dimRegion;
                                    c++)
                                {
                                    if (((r != row) || (c != col)) &&
                                        ((r != r1) || (c != col)))
                                    {
                                        if (board->ExcludeCellChoices(
                                                            r,
                                                            c,
                                                            bitTwins))
                                        {
                                            board->IncrementRuleExcludes(
                                                        m_idxRule);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TExitDec(RULE, WPPIndent, fFoundNakedTwins || fFoundHiddenTwins);
    return fFoundNakedTwins || fFoundHiddenTwins;
}   //TwinsInCol

/**
 *  This function determines if a given cell is a twin in the region.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param cellChoices Specifies the choices of the cell.
 *
 *  @return Returns TRUE if the cell is part of the twins in the region,
 *          FALSE otherwise.
 */
BOOL
Twins::TwinsInRegion(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG cellChoices
    )
{
    BOOL fFoundNakedTwins = FALSE;
    BOOL fFoundHiddenTwins = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,cellChoices=%I64x",
           board, row, col, cellChoices);

    if (!(board->GetCellFlags(row, col) & CELLF_REGION_TWINS))
    {
        for (int r1 = (row/m_dimRegion)*m_dimRegion;
             !fFoundNakedTwins && !fFoundHiddenTwins &&
             (r1 < (row/m_dimRegion + 1)*m_dimRegion);
             r1++)
        {
            for (int c1 = (col/m_dimRegion)*m_dimRegion;
                 !fFoundNakedTwins && !fFoundHiddenTwins &&
                 (c1 < (col/m_dimRegion + 1)*m_dimRegion);
                 c1++)
            {
                if ((r1 != row) || (c1 != col))
                {
                    int cntChoices;
                    ULONGLONG bitChoices;
                    ULONGLONG bitTwins, bitCommon;
                    int cntCommon;

                    bitChoices = board->GetCellChoices(r1, c1, &cntChoices);
                    bitCommon = cellChoices & bitChoices;
                    cntCommon = CountSetBits(bitCommon);
                    if (cntCommon >= 2)
                    {
                        if ((cntCommon == 2) && (cellChoices == bitChoices))
                        {
                            //
                            // Found naked twins.
                            //
                            fFoundNakedTwins = TRUE;
                            bitTwins = bitCommon;
                        }
                        else
                        {
                            ULONGLONG bit;
                            //
                            // For each of the common bits, check the rest of
                            // the region for one other occurrence.
                            //
                            bitTwins = 0;
                            while (bitCommon != 0)
                            {
                                bit = ~(-((LONGLONG)bitCommon)^bitCommon) &
                                      bitCommon;
                                bitCommon &= ~bit;
                                if (CountCellsInRegionWithChoice(board,
                                                                 row,
                                                                 col,
                                                                 bit) == 1)
                                {
                                    bitTwins |= bit;
                                }
                            }

                            if (CountSetBits(bitTwins) == 2)
                            {
                                //
                                // Found hidden twins.
                                //
                                fFoundHiddenTwins = TRUE;
                            }
                        }

                        if (fFoundNakedTwins || fFoundHiddenTwins)
                        {
                            //
                            // We found a pair of twins.
                            //
                            if (g_dwfSudoku & SF_VERBOSE)
                            {
                                wprintf(L"Found %sTwinsInRegion: [%d,%d]/[%d,%d] =",
                                        fFoundNakedTwins? L"Naked": L"Hidden",
                                        row + 1, col + 1, r1 + 1, c1 + 1);
                                PrintChoices(bitTwins, m_dimBoard);
                                wprintf(L"\n");
                            }
                            board->SetCellFlags(row, col, CELLF_REGION_TWINS);
                            board->SetCellFlags(r1, c1, CELLF_REGION_TWINS);
                            board->IncrementRuleFinds(m_idxRule);

                            if (fFoundHiddenTwins)
                            {
                                //
                                // Clear all other possibilities for the
                                // twins.
                                //
                                if (board->ExcludeCellChoices(
                                                    row,
                                                    col,
                                                    ~bitTwins & cellChoices))
                                {
                                    board->IncrementRuleExcludes(m_idxRule);
                                }
                                if (board->ExcludeCellChoices(
                                                    r1,
                                                    c1,
                                                    ~bitTwins & bitChoices))
                                {
                                    board->IncrementRuleExcludes(m_idxRule);
                                }
                            }
                            else if (fFoundNakedTwins)
                            {
                                //
                                // Exclude the twins from the rest of the row.
                                //
                                for (int r2 = (row/m_dimRegion)*m_dimRegion;
                                     r2 < ((row/m_dimRegion + 1)*m_dimRegion);
                                     r2++)
                                {
                                    for (int c2 = (col/m_dimRegion)*m_dimRegion;
                                         c2 < ((col/m_dimRegion + 1)*m_dimRegion);
                                         c2++)
                                    {
                                        if (((r2 != row) || (c2 != col)) &&
                                            ((r2 != r1) || (c2 != c1)))
                                        {
                                            if (board->ExcludeCellChoices(
                                                                r2,
                                                                c2,
                                                                bitTwins))
                                            {
                                                board->IncrementRuleExcludes(
                                                            m_idxRule);
                                            }
                                        }
                                    }
                                }
                            }

                            if (row == r1)
                            {
                                //
                                // The twins are also in the same row, exclude
                                // them from the rest of the row.
                                //
                                for (int c = 0; c < m_dimBoard; c++)
                                {
                                    if ((c != col) && (c != c1))
                                    {
                                        if (board->ExcludeCellChoices(
                                                            row,
                                                            c,
                                                            bitTwins))
                                        {
                                            board->IncrementRuleExcludes(
                                                        m_idxRule);
                                        }
                                    }
                                }
                            }
                            else if (col == c1)
                            {
                                //
                                // The twins are also in the same col, exclude
                                // them from the rest of the col.
                                //
                                for (int r = 0; r < m_dimBoard; r++)
                                {
                                    if ((r != row) && (r != r1))
                                    {
                                        if (board->ExcludeCellChoices(
                                                            r,
                                                            col,
                                                            bitTwins))
                                        {
                                            board->IncrementRuleExcludes(
                                                        m_idxRule);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TExitDec(RULE, WPPIndent, fFoundNakedTwins || fFoundHiddenTwins);
    return fFoundNakedTwins || fFoundHiddenTwins;
}   //TwinsInRegion

/**
 *  This function counts the number of cells in the row that have the
 *  given choice.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the choice to look for.
 *
 *  @returns> Returns the number of cells that has the choice.
 */
int
Twins::CountCellsInRowWithChoice(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    int count = 0;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int c = 0; c < m_dimBoard; c++)
    {
        if (c != col)
        {
            if (bitChoice & board->GetCellChoices(row, c, NULL))
            {
                count++;
            }
        }
    }

    TExitDec(RULE, WPPIndent, count);
    return count;
}   //CountCellsInRowWithChoice

/**
 *  This function counts the number of cells in the column that have the
 *  given choice.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the choice to look for.
 *
 *  @returns> Returns the number of cells that has the choice.
 */
int
Twins::CountCellsInColWithChoice(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    int count = 0;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int r = 0; r < m_dimBoard; r++)
    {
        if (r != row)
        {
            if (bitChoice & board->GetCellChoices(r, col, NULL))
            {
                count++;
            }
        }
    }

    TExitDec(RULE, WPPIndent, count);
    return count;
}   //CountCellsInColWithChoice

/**
 *  This function counts the number of cells in the region that have the
 *  given choice.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoice Specifies the choice to look for.
 *
 *  @returns> Returns the number of cells that has the choice.
 */
int
Twins::CountCellsInRegionWithChoice(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG bitChoice
    )
{
    int count = 0;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,bitChoice=%I64x",
           board, row, col, bitChoice);

    for (int r = (row/m_dimRegion)*m_dimRegion;
         r < ((row/m_dimRegion + 1)*m_dimRegion);
         r++)
    {
        for (int c = (col/m_dimRegion)*m_dimRegion;
             c < ((col/m_dimRegion + 1)*m_dimRegion);
             c++)
        {
            if ((r != row) || (c != col))
            {
                if (bitChoice & board->GetCellChoices(r, c, NULL))
                {
                    count++;
                }
            }
        }
    }

    TExitDec(RULE, WPPIndent, count);
    return count;
}   //CountCellsInRegionWithChoice

