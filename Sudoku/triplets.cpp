#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Triplets.cpp" />
///
/// <summary>
///     This module contains the implementation of the Triplets class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "triplets.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param dimRegion Specifies the dimension of the region.
 */
Triplets::Triplets(
    __in int dimRegion
    ): Rule(dimRegion)
{
    TEnterVoid(RULE, WPPIndent);
    m_idxRule = LSBitPosition(CELLF_TRIPLETS, MAX_NUM_RULES);
    TExitVoid(RULE, WPPIndent);
    return;
}   //Triplets

/**
 *  Destructor to destroy an instance of the object.
 */
Triplets::~Triplets(
    void
    )
{
    TEnterVoid(RULE, WPPIndent);
    TExitVoid(RULE, WPPIndent);
    return;
}   //~Triplets

/**
 *  This function process the cell with the Triplets rule.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *
 *  @return Returns TRUE if the cell is solved, FALSE otherwise.
 */
BOOL
Triplets::ProcessCell(
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
    if ((cntChoices >= 2) && (cntChoices <= 3))
    {
        //
        // Process the cell only if the cell is a potential triplet.
        //
        TripletsInRow(board, row, col, bitChoices);
        TripletsInCol(board, row, col, bitChoices);
        TripletsInRegion(board, row, col, bitChoices);
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
Triplets::PrintSummary(
    __in Board *board
    )
{
    TEnter(RULE, WPPIndent, "board=%p", board);

    wprintf(L"    Number of Triplets found = %d\n",
            board->GetRuleFinds(m_idxRule));
    wprintf(L"   Excluded by Triplets rule = %d\n",
            board->GetRuleExcludes(m_idxRule));

    TExitVoid(RULE, WPPIndent);
    return;
}   //PrintSummary

/**
 *  This function determines if a given cell is a triplet in the row.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param cellChoices Specifies the choices of the cell.
 *
 *  @return Returns TRUE if the cell is part of a triplet in the row,
 *          FALSE otherwise.
 */
BOOL
Triplets::TripletsInRow(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG cellChoices
    )
{
    BOOL fFound = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,cellChoices=%I64x",
           board, row, col, cellChoices);

    if (!(board->GetCellFlags(row, col) & CELLF_ROW_TRIPLETS))
    {
        for (int c1 = 0; !fFound && (c1 < m_dimBoard); c1++)
        {
            if (c1 != col)
            {
                int cntChoices;
                ULONGLONG bitChoices;
                ULONGLONG bitTriplets;

                bitChoices = board->GetCellChoices(row, c1, &cntChoices);
                if ((cntChoices >= 2) && (cntChoices <= 3))
                {
                    bitTriplets = cellChoices | bitChoices;
                    if (CountSetBits(bitTriplets) == 3)
                    {
                        int c2 = FindTripletInRow(board,
                                                  row,
                                                  col,
                                                  c1,
                                                  bitTriplets);
                        if (c2 >= 0)
                        {
                            fFound = TRUE;
                            //
                            // We found a triplet.
                            //
                            if (g_dwfSudoku & SF_VERBOSE)
                            {
                                wprintf(L"Found TripletsInRow: [%d,%d]/[%d,%d]/[%d,%d] =",
                                        row + 1, col + 1,
                                        row + 1, c1 + 1,
                                        row + 1, c2 + 1);
                                PrintChoices(bitTriplets, m_dimBoard);
                                wprintf(L"\n");
                            }
                            board->SetCellFlags(row, col, CELLF_ROW_TRIPLETS);
                            board->SetCellFlags(row, c1, CELLF_ROW_TRIPLETS);
                            board->SetCellFlags(row, c2, CELLF_ROW_TRIPLETS);
                            board->IncrementRuleFinds(m_idxRule);
                            //
                            // Exclude the triplets from the rest of the row.
                            //
                            for (int c3 = 0; c3 < m_dimBoard; c3++)
                            {
                                if ((c3 != col) && (c3 != c1) && (c3 != c2))
                                {
                                    if (board->ExcludeCellChoices(
                                                    row,
                                                    c3,
                                                    bitTriplets))
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

    TExitDec(RULE, WPPIndent, fFound);
    return fFound;
}   //TripletsInRow

/**
 *  This function determines if a given cell is a triplet in the column.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param cellChoices Specifies the choices of the cell.
 *
 *  @return Returns TRUE if the cell is part of a triplet in the column,
 *          FALSE otherwise.
 */
BOOL
Triplets::TripletsInCol(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG cellChoices
    )
{
    BOOL fFound = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,cellChoices=%I64x",
           board, row, col, cellChoices);

    if (!(board->GetCellFlags(row, col) & CELLF_COL_TRIPLETS))
    {
        for (int r1 = 0; !fFound && (r1 < m_dimBoard); r1++)
        {
            if (r1 != row)
            {
                int cntChoices;
                ULONGLONG bitChoices;
                ULONGLONG bitTriplets;

                bitChoices = board->GetCellChoices(r1, col, &cntChoices);
                if ((cntChoices >= 2) && (cntChoices <= 3))
                {
                    bitTriplets = cellChoices | bitChoices;
                    if (CountSetBits(bitTriplets) == 3)
                    {
                        int r2 = FindTripletInCol(board,
                                                  row,
                                                  col,
                                                  r1,
                                                  bitTriplets);
                        if (r2 >= 0)
                        {
                            fFound = TRUE;
                            //
                            // We found a triplet.
                            //
                            if (g_dwfSudoku & SF_VERBOSE)
                            {
                                wprintf(L"Found TripletsInCol: [%d,%d]/[%d,%d]/[%d,%d] =",
                                        row + 1, col + 1,
                                        r1 + 1, col + 1,
                                        r2 + 1, col + 1);
                                PrintChoices(bitTriplets, m_dimBoard);
                                wprintf(L"\n");
                            }
                            board->SetCellFlags(row, col, CELLF_COL_TRIPLETS);
                            board->SetCellFlags(r1, col, CELLF_COL_TRIPLETS);
                            board->SetCellFlags(r2, col, CELLF_COL_TRIPLETS);
                            board->IncrementRuleFinds(m_idxRule);
                            //
                            // Exclude the triplets from the rest of the row.
                            //
                            for (int r3 = 0; r3 < m_dimBoard; r3++)
                            {
                                if ((r3 != row) && (r3 != r1) && (r3 != r2))
                                {
                                    if (board->ExcludeCellChoices(
                                                    r3,
                                                    col,
                                                    bitTriplets))
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

    TExitDec(RULE, WPPIndent, fFound);
    return fFound;
}   //TripletsInCol

/**
 *  This function determines if a given cell is a triplet in the region.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param cellChoices Specifies the choices of the cell.
 *
 *  @return Returns TRUE if the cell is part of a triplet in the region,
 *          FALSE otherwise.
 */
BOOL
Triplets::TripletsInRegion(
    __in Board *board,
    __in int row,
    __in int col,
    __in ULONGLONG cellChoices
    )
{
    BOOL fFound = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,cellChoices=%I64x",
           board, row, col, cellChoices);

    if (!(board->GetCellFlags(row, col) & CELLF_REGION_TRIPLETS))
    {
        for (int r1 = (row/m_dimRegion)*m_dimRegion;
             !fFound && (r1 < (row/m_dimRegion + 1)*m_dimRegion);
             r1++)
        {
            for (int c1 = (col/m_dimRegion)*m_dimRegion;
                 !fFound && (c1 < (col/m_dimRegion + 1)*m_dimRegion);
                 c1++)
            {
                if ((r1 != row) || (c1 != col))
                {
                    int cntChoices;
                    ULONGLONG bitChoices;
                    ULONGLONG bitTriplets;

                    bitChoices = board->GetCellChoices(r1, c1, &cntChoices);
                    if ((cntChoices >= 2) && (cntChoices <= 3))
                    {
                        bitTriplets = cellChoices | bitChoices;
                        if (CountSetBits(bitTriplets) == 3)
                        {
                            int r2, c2;

                            r2 = FindTripletInRegion(board,
                                                     row,
                                                     col,
                                                     r1,
                                                     c1,
                                                     bitTriplets);
                            if (r2 >= 0)
                            {
                                fFound = TRUE;
                                c2 = (r2 >> 8) & 0xff;
                                r2 &= 0xff;
                                //
                                // We found a triplet.
                                //
                                if (g_dwfSudoku & SF_VERBOSE)
                                {
                                    wprintf(L"Found TripletsInRegion: [%d,%d]/[%d,%d]/[%d,%d] =",
                                            row + 1, col + 1,
                                            r1 + 1, c1 + 1,
                                            r2 + 1, c2 + 1);
                                    PrintChoices(bitTriplets, m_dimBoard);
                                    wprintf(L"\n");
                                }
                                board->SetCellFlags(row,
                                                    col,
                                                    CELLF_REGION_TRIPLETS);
                                board->SetCellFlags(r1,
                                                    c1,
                                                    CELLF_REGION_TRIPLETS);
                                board->SetCellFlags(r2,
                                                    c2,
                                                    CELLF_REGION_TRIPLETS);
                                board->IncrementRuleFinds(m_idxRule);
                                //
                                // Exclude the triplets from the rest of the
                                // row.
                                //
                                for (int r3 = (row/m_dimRegion)*m_dimRegion;
                                     r3 < (row/m_dimRegion + 1)*m_dimRegion;
                                     r3++)
                                {
                                    for (int c3 = (col/m_dimRegion)*m_dimRegion;
                                         c3 < (col/m_dimRegion + 1)*m_dimRegion;
                                         c3++)
                                    {
                                        if (((r3 != row) || (c3 != col)) &&
                                            ((r3 != r1) || (c3 != c1)) &&
                                            ((r3 != r2) || (c3 != c2)))
                                        {
                                            if (board->ExcludeCellChoices(
                                                        r3,
                                                        c3,
                                                        bitTriplets))
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
    }

    TExitDec(RULE, WPPIndent, fFound);
    return fFound;
}   //TripletsInRegion

/**
 *  This function finds one and only one given triplets in the row.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the primary cell.
 *  @param col Specifies the col index of the primary cell.
 *  @param c1 Specifies the col index of the 2nd cell in the triplet.
 *  @param bitTriplets Specifies the bits of the triplet to look for.
 *
 *  @return Success: Returns column index of the found triplet.
 *  @return Failure: Returns -1.
 */
int
Triplets::FindTripletInRow(
    __in Board *board,
    __in int row,
    __in int col,
    __in int c1,
    __in ULONGLONG bitTriplets
    )
{
    int c2;
    int cntChoices;
    ULONGLONG bitChoices;
    BOOL fFound = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,c1=%d,bitTriplets=%I64x",
           board, row, col, c1, bitTriplets);

    for (c2 = 0; !fFound && (c2 < m_dimBoard); c2++)
    {
        if ((c2 != col) && (c2 != c1))
        {
            bitChoices = board->GetCellChoices(row, c2, &cntChoices);
            if ((cntChoices >= 2) && (cntChoices <= 3) &&
                (CountSetBits(bitTriplets | bitChoices) == 3))
            {
                //
                // Found a potential triplet.
                //
                fFound = TRUE;
                break;
            }
        }
    }

    if (!fFound)
    {
        c2 = -1;
    }

    TExitDec(RULE, WPPIndent, c2);
    return c2;
}   //FindTripletInRow

/**
 *  This function finds one and only one given triplets in the column.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the primary cell.
 *  @param col Specifies the col index of the primary cell.
 *  @param r1 Specifies the row index of the 2nd cell in the triplet.
 *  @param bitTriplets Specifies the bits of the triplet to look for.
 *
 *  @return Success: Returns row index of the found triplet.
 *  @return Failure: Returns -1.
 */
int
Triplets::FindTripletInCol(
    __in Board *board,
    __in int row,
    __in int col,
    __in int r1,
    __in ULONGLONG bitTriplets
    )
{
    int r2;
    int cntChoices;
    ULONGLONG bitChoices;
    BOOL fFound = FALSE;

    TEnter(RULE, WPPIndent, "board=%p,row=%d,col=%d,r1=%d,bitTriplets=%I64x",
           board, row, col, r1, bitTriplets);

    for (r2 = 0; !fFound && (r2 < m_dimBoard); r2++)
    {
        if ((r2 != row) && (r2 != r1))
        {
            bitChoices = board->GetCellChoices(r2, col, &cntChoices);
            if ((cntChoices >= 2) && (cntChoices <= 3) &&
                (CountSetBits(bitTriplets | bitChoices) == 3))
            {
                //
                // Found a potential triplet.
                //
                fFound = TRUE;
                break;
            }
        }
    }

    if (!fFound)
    {
        r2 = -1;
    }

    TExitDec(RULE, WPPIndent, r2);
    return r2;
}   //FindTripletInCol

/**
 *  This function finds one and only one given triplets in the region.
 *
 *  @param board Points to the board object.
 *  @param row Specifies the row index of the primary cell.
 *  @param col Specifies the col index of the primary cell.
 *  @param r1 Specifies the row index of the 2nd cell in the triplet.
 *  @param c1 Specifies the col index of the 2nd cell in the triplet.
 *  @param bitTriplets Specifies the bits of the triplet to look for.
 *
 *  @return Success: Returns row index in lower byte and column index in
 *          second byte.
 *  @return Failure: Returns -1.
 */
int
Triplets::FindTripletInRegion(
    __in Board *board,
    __in int row,
    __in int col,
    __in int r1,
    __in int c1,
    __in ULONGLONG bitTriplets
    )
{
    int r2 = 0;
    int c2 = 0;
    int cntChoices;
    ULONGLONG bitChoices;
    BOOL fFound = FALSE;

    TEnter(RULE, WPPIndent,
           "board=%p,row=%d,col=%d,r1=%d,c1=%d,bitTriplets=%I64x",
           board, row, col, r1, c1, bitTriplets);

    for (r2 = (row/m_dimRegion)*m_dimRegion;
         !fFound && (r2 < (row/m_dimRegion + 1)*m_dimRegion);
         r2++)
    {
        for (c2 = (col/m_dimRegion)*m_dimRegion;
             !fFound && (c2 < (col/m_dimRegion + 1)*m_dimRegion);
             c2++)
        {
            if (((r2 != row) || (c2 != col)) &&
                ((r2 != r1) || (c2 != c1)))
            {
                bitChoices = board->GetCellChoices(r2, c2, &cntChoices);
                if ((cntChoices >= 2) && (cntChoices <= 3) &&
                    (CountSetBits(bitTriplets | bitChoices) == 3))
                {
                    //
                    // Found a potential triplet.
                    //
                    fFound = TRUE;
                    break;
                }
            }
        }

        if (fFound)
        {
            break;
        }
    }

    if (!fFound)
    {
        r2 = -1;
    }
    else
    {
        r2 = (r2 & 0xff) | ((c2 & 0xff) << 8);
    }

    TExitDec(RULE, WPPIndent, r2);
    return r2;
}   //FindTripletInRegion

