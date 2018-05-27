#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="board.cpp" />
///
/// <summary>
///     This module contains the implementation of the Board class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "board.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param dimRegion Specifies the dimension of the region.
 */
Board::Board(
    __in int dimRegion
    ): m_dimRegion(dimRegion),
       m_dimBoard(dimRegion*dimRegion),
       m_fProgress(FALSE)
{
    TEnter(BOARD, WPPIndent, "dimRegion=%d", dimRegion);

    m_cntInitCells = 0;
    m_cntSolvedCells = 0;
    for (int i = 0; i < MAX_NUM_RULES; i++)
    {
        m_cntRuleFinds[i] = 0;
        m_cntRuleExcludes[i] = 0;
    }

    m_Cells = new Cell**[m_dimBoard];
    if (m_Cells == NULL)
    {
        TMsg(Fatal, "Failed to allocate %d rows on the board.", m_dimBoard);
    }
    else
    {
        for (int r = 0; r < m_dimBoard; r++)
        {
            m_Cells[r] = new Cell*[m_dimBoard];
            if (m_Cells[r] == NULL)
            {
                TMsg(Fatal, "Failed to allocate %d cols on row %d.",
                     m_dimBoard, r);
            }
            else
            {
                for (int c = 0; c < m_dimBoard; c++)
                {
                    m_Cells[r][c] = new Cell(r, c, m_dimBoard);
                    if (m_Cells[r][c] == NULL)
                    {
                        TMsg(Fatal, "Failed to allocate cell[%d,%d].", r, c);
                    }
                }
            }
        }
    }

    TExitVoid(BOARD, WPPIndent);
    return;
}   //Board

/**
 *  Destructor to destroy an instance of the object.
 */
Board::~Board(
    void
    )
{
    TEnterVoid(BOARD, WPPIndent);

    if (m_Cells != NULL)
    {
        for (int r = 0; r < m_dimBoard; r++)
        {
            if (m_Cells[r] != NULL)
            {
                for (int c = 0; c < m_dimBoard; c++)
                {
                    if (m_Cells[r][c] != NULL)
                    {
                        delete m_Cells[r][c];
                        m_Cells[r][c] = NULL;
                    }
                }
                delete [] m_Cells[r];
                m_Cells[r] = NULL;
            }
        }
        delete [] m_Cells;
        m_Cells = NULL;
    }

    TExitVoid(BOARD, WPPIndent);
    return;
}   //~Board

/**
 *  This function returns the value of the cell.
 *
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *
 *  @return Success: Returns the value of the cell.
 *  @return Failure: Returns -1.
 */
int
Board::GetCellValue(
    __in int row,
    __in int col
    )
{
    int value = -1;

    TEnter(BOARD, WPPIndent, "row=%d,col=%d", row, col);

    TAssert(m_Cells != NULL);
    if (IsValidCell(row, col))
    {
        value = m_Cells[row][col]->GetValue();
    }
    else
    {
        TMsg(Err, "Invalid cell coordinates (%d,%d).", row, col);
    }

    TExitDec(BOARD, WPPIndent, value);
    return value;
}   //GetCellValue

/**
 *  This function sets the value of the cell either for initializing it or
 *  solving it.
 *
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param value Specifies the value of the cell.
 *  @param dwFlags Specifies the flags to set.
 *
 *  @return Returns TRUE if the function is successful, FALSE otherwise.
 */
BOOL
Board::SetCellValue(
    __in int row,
    __in int col,
    __in int value,
    __in DWORD dwFlags
    )
{
    BOOL rc = FALSE;

    TEnter(BOARD, WPPIndent, "row=%d,col=%d,value=%d,flags=%x",
           row, col, value, dwFlags);

    TAssert(m_Cells != NULL);
    if (IsValidCell(row, col))
    {
        rc = m_Cells[row][col]->SetValue(value, dwFlags);
        if (rc)
        {
            DWORD dwRule = dwFlags & CELLF_RULE_MASK;
            int idxRule = (dwFlags & CELLF_SOLVED)?
                          LSBitPosition(dwRule, MAX_NUM_RULES): 0;
            //
            // Exclude this value from the rest of the row.
            //
            for (int c = 0; c < m_dimBoard; c++)
            {
                if (c != col)
                {
                    if (ExcludeCellValue(row, c, value) &&
                        (dwFlags & CELLF_SOLVED))
                    {
                        m_cntRuleExcludes[idxRule]++;
                    }
                }
            }
            //
            // Exclude this value from the rest of the col.
            //
            for (int r = 0; r < m_dimBoard; r++)
            {
                if (r != row)
                {
                    if (ExcludeCellValue(r, col, value) &&
                        (dwFlags & CELLF_SOLVED))
                    {
                        m_cntRuleExcludes[idxRule]++;
                    }
                }
            }
            //
            // Exclude this value from the rest of the region.
            //
            for (int r = (row/m_dimRegion)*m_dimRegion;
                 r < (row/m_dimRegion + 1)*m_dimRegion;
                 r++)
            {
                for (int c = (col/m_dimRegion)*m_dimRegion;
                     c < (col/m_dimRegion + 1)*m_dimRegion;
                     c++)
                {
                    if ((r != row) || (c != col))
                    {
                        if (ExcludeCellValue(r, c, value) &&
                            (dwFlags & CELLF_SOLVED))
                        {
                            m_cntRuleExcludes[idxRule]++;
                        }
                    }
                }
            }
            //
            // Update statistics.
            //
            if (dwFlags & CELLF_SOLVED)
            {
                TAssert(dwRule != 0);
                TAssert((dwRule & ~CELLF_SOLVERULES_MASK) == 0);
                m_cntSolvedCells++;
                m_cntRuleFinds[idxRule]++;
            }
            else
            {
                m_cntInitCells++;
            }
            m_fProgress = TRUE;
        }
    }
    else
    {
        TMsg(Err, "Invalid cell coordinates (%d,%d).", row, col);
    }


    TExitDec(BOARD, WPPIndent, rc);
    return rc;
}   //SetCellValue

/**
 *  This function returns the choices of a given cell.
 *
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param pcntChoices Points to the variable to receive the number of
 *         choices of a cell.
 *
 *  @return Returns the number of choices.
 */
ULONGLONG
Board::GetCellChoices(
    __in      int row,
    __in      int col,
    __out_opt int *pcntChoices
    )
{
    ULONGLONG bitChoices = 0;

    TEnter(BOARD, WPPIndent, "row=%d,col=%d,pcntChoices=%p",
           row, col, pcntChoices);

    TAssert(m_Cells != NULL);
    if (IsValidCell(row, col))
    {
        bitChoices = m_Cells[row][col]->GetChoices(pcntChoices);
    }
    else
    {
        TMsg(Err, "Invalid cell coordinates (%d,%d).", row, col);
        if (pcntChoices != NULL)
        {
            *pcntChoices = 0;
        }
    }

    TExitDecMsg(BOARD, WPPIndent, pcntChoices? *pcntChoices: 0,
                " (Choices=%I64x)", bitChoices);
    return bitChoices;
}   //GetCellChoices

/**
 *  This function excludes the given choice bit mask for the cell.
 *
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param bitChoices Specifies the choice bit mask.
 *
 *  @return Returns TRUE if choices have been excluded from the cell,
 *          FALSE otherwise.
 */
BOOL
Board::ExcludeCellChoices(
    __in int row,
    __in int col,
    __in ULONGLONG bitChoices
    )
{
    BOOL rc = FALSE;

    TEnter(BOARD, WPPIndent, "row=%d,col=%d,bitChoices=%I64x",
           row, col, bitChoices);

    TAssert(m_Cells != NULL);
    if (IsValidCell(row, col))
    {
        ULONGLONG oldChoices, newChoices;

        oldChoices = m_Cells[row][col]->GetChoices(NULL);
        newChoices = m_Cells[row][col]->ExcludeChoices(bitChoices);
        if (oldChoices != newChoices)
        {
            m_fProgress = TRUE;
            rc = TRUE;
        }
    }
    else
    {
        TMsg(Err, "Invalid cell coordinates (%d,%d).", row, col);
    }

    TExitDec(BOARD, WPPIndent, rc);
    return rc;
}   //ExcludeCellChoices

/**
 *  This function excludes the given value as one of the choices for
 *  the spcified cell.
 *
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param value Specifies the value to exclude.
 *
 *  @return Returns TRUE if choices have been excluded from the cell,
 *          FALSE otherwise.
 */
BOOL
Board::ExcludeCellValue(
    __in int row,
    __in int col,
    __in int value
    )
{
    BOOL rc = FALSE;

    TEnter(BOARD, WPPIndent, "row=%d,col=%d,value=%d", row, col, value);

    TAssert(m_Cells != NULL);
    if (IsValidCell(row, col))
    {
        if ((value <= 0) || (value > m_dimBoard))
        {
            TMsg(Err, "Invalid value %d.", value);
        }
        else
        {
            rc = ExcludeCellChoices(row, col, (ULONGLONG)1 << (value - 1));
        }
    }
    else
    {
        TMsg(Err, "Invalid cell coordinates (%d,%d).", row, col);
    }

    TExitHex(BOARD, WPPIndent, rc);
    return rc;
}   //ExcludeCellValue

/**
 *  This function returns the flags of the given cell.
 *
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *
 *  @return Returns the cell flags.
 */
DWORD
Board::GetCellFlags(
    __in int row,
    __in int col
    )
{
    DWORD dwFlags = 0;

    TEnter(BOARD, WPPIndent, "row=%d,col=%d", row, col);

    TAssert(m_Cells != NULL);
    if (IsValidCell(row, col))
    {
        dwFlags = m_Cells[row][col]->GetFlags();
    }
    else
    {
        TMsg(Err, "Invalid cell coordinates (%d,%d).", row, col);
    }

    TExitHex(BOARD, WPPIndent, dwFlags);
    return dwFlags;
}   //GetCellFlags

/**
 *  This function sets the flag bits of the given cell.
 *
 *  @param row Specifies the row index of the cell.
 *  @param col Specifies the col index of the cell.
 *  @param flags Specifies the flag bits to set.
 *
 *  @return Returns the updated cell flags.
 */
DWORD
Board::SetCellFlags(
    __in int row,
    __in int col,
    __in DWORD flags
    )
{
    DWORD dwFlags = 0;

    TEnter(BOARD, WPPIndent, "row=%d,col=%d,flags=%x", row, col, flags);

    TAssert(m_Cells != NULL);
    if (IsValidCell(row, col))
    {
        DWORD dwOldRules, dwNewRules;

        dwOldRules = m_Cells[row][col]->GetFlags() & CELLF_RULE_MASK;
        dwFlags = m_Cells[row][col]->SetFlags(flags);
        dwNewRules = dwFlags & CELLF_RULE_MASK;
        if ((dwOldRules^dwNewRules) == (flags & CELLF_RULE_MASK))
        {
            m_fProgress = TRUE;
        }
    }
    else
    {
        TMsg(Err, "Invalid cell coordinates (%d,%d).", row, col);
    }

    TExitHex(BOARD, WPPIndent, dwFlags);
    return dwFlags;
}   //SetCellFlags

/**
 *  This function returns the value of the progress flags.
 *
 *  @return Returns the progress flag.
 */
BOOL
Board::GetProgress(
    void
    )
{
    TEnterVoid(BOARD, WPPIndent);
    TExitDec(BOARD, WPPIndent, m_fProgress);
    return m_fProgress;
}   //GetProgress

/**
 *  This function clears the progress flags.
 */
void
Board::ClearProgress(
    void
    )
{
    TEnterVoid(BOARD, WPPIndent);
    m_fProgress = FALSE;
    TExitVoid(BOARD, WPPIndent);
    return;
}   //ClearProgress

/**
 *  This function returns the number of initial cells.
 *
 *  @return Returns the number of initial cells.
 */
int
Board::GetInitCellCount(
    void
    )
{
    TEnterVoid(BOARD, WPPIndent);
    TExitDec(BOARD, WPPIndent, m_cntInitCells);
    return m_cntInitCells;
}   //GetInitialCellCount

/**
 *  This function returns the number of solved cells.
 *
 *  @return Returns the number of solved cells.
 */
int
Board::GetSolvedCellCount(
    void
    )
{
    TEnterVoid(BOARD, WPPIndent);
    TExitDec(BOARD, WPPIndent, m_cntSolvedCells);
    return m_cntSolvedCells;
}   //GetSolvedCellCount

/**
 *  This function returns the count of successful application of a particular
 *  rule.
 *
 *  @param idx Specifies the rule index.
 *
 *  @return Returns the count.
 */
int
Board::GetRuleFinds(
    __in int idx
    )
{
    TEnter(BOARD, WPPIndent, "idx=%d", idx);
    TAssert(idx < ARRAYSIZE(m_cntRuleFinds));
    TExitDec(BOARD, WPPIndent, m_cntRuleFinds[idx]);
    return m_cntRuleFinds[idx];
}   //GetRuleFinds

/**
 *  This function returns the count of exclusions resulted from a particular
 *  rule.
 *
 *  @param idx Specifies the rule index.
 *
 *  @return Returns the count.
 */
int
Board::GetRuleExcludes(
    __in int idx
    )
{
    TEnter(BOARD, WPPIndent, "idx=%d", idx);
    TAssert(idx < ARRAYSIZE(m_cntRuleExcludes));
    TExitDec(BOARD, WPPIndent, m_cntRuleExcludes[idx]);
    return m_cntRuleExcludes[idx];
}   //GetRuleExcludes

/**
 *  This function increments the find count of the specified rule.
 *
 *  @param idx Specifies the rule index.
 *
 *  @return Returns the updated count.
 */
int
Board::IncrementRuleFinds(
    __in int idx
    )
{
    TEnter(BOARD, WPPIndent, "idx=%d", idx);

    TAssert(idx < ARRAYSIZE(m_cntRuleFinds));
    m_cntRuleFinds[idx]++;

    TExitDec(BOARD, WPPIndent, m_cntRuleFinds[idx]);
    return m_cntRuleFinds[idx];
}   //IncrementRuleFinds

/**
 *  This function increments the exclusion count of the specified rule.
 *
 *  @param idx Specifies the rule index.
 *
 *  @return Returns the updated count.
 */
int
Board::IncrementRuleExcludes(
    __in int idx
    )
{
    TEnter(BOARD, WPPIndent, "idx=%d", idx);

    TAssert(idx < ARRAYSIZE(m_cntRuleExcludes));
    m_cntRuleExcludes[idx]++;

    TExitDec(BOARD, WPPIndent, m_cntRuleExcludes[idx]);
    return m_cntRuleExcludes[idx];
}   //IncrementRuleExcludes

/**
 *  This function processes all the cells on the board and applying the
 *  given rule to each cell.
 *
 *  @param RuleTable Points to the rule table.
 *  @param NumRules Specifies the number of rules in the rule table.
 */
void
Board::ProcessBoard(
    __in Rule **RuleTable,
    __in int    NumRules
    )
{
    TEnter(BOARD, WPPIndent, "RuleTable=%p,NumRules=%d", RuleTable, NumRules);

    for (int r = 0; r < m_dimBoard; r++)
    {
        for (int c = 0; c < m_dimBoard; c++)
        {
            for (int i = 0; i < NumRules; i++)
            {
                if (RuleTable[i] != NULL)
                {
                    if (RuleTable[i]->ProcessCell(this, r, c))
                    {
                        break;
                    }
                }
            }
        }
    }

    TExitVoid(BOARD, WPPIndent);
    return;
}   //ProcessBoard

/**
 *  This function prints the board.
 */
void
Board::PrintBoard(
    void
    )
{
    int row, col, i, j;
    ULONGLONG bitChoices;
    int cntChoices;
    DWORD dwCellFlags;

    TEnterVoid(BOARD, WPPIndent);

    wprintf(L"\n   ");
    for (col = 0; col < m_dimBoard; col++)
    {
        wprintf(L" ");
        for (i = 0; i < m_dimRegion; i++)
        {
            if (i == m_dimRegion/2)
            {
                wprintf(L"%2d", col + 1);
            }
            else
            {
                wprintf(L"  ");
            }
        }
    }
    wprintf(L"\n");

    for (row = 0; row < m_dimBoard; row++)
    {
        //
        // Top border
        //
        wprintf(L"   ");
        for (col = 0; col < m_dimBoard; col++)
        {
            wprintf(L"+");
            for (i = 0; i < m_dimRegion; i++)
            {
                wprintf(L"%s", ((row%m_dimRegion) == 0)? L"--": L"..");
            }
        }
        wprintf(L"+\n");

        for (i = 0; i < m_dimRegion; i++)
        {
            //
            // Row label
            //
            if (i == m_dimRegion/2)
            {
                wprintf(L"%2d ", row + 1);
            }
            else
            {
                wprintf(L"   ");
            }

            for (col = 0; col < m_dimBoard; col++)
            {
                wprintf(L"%c", ((col%m_dimRegion) == 0)? L'|': L':');
                bitChoices = GetCellChoices(row, col, &cntChoices);
                dwCellFlags = GetCellFlags(row, col);
                for (j = 0; j < m_dimRegion; j++)
                {
                    if (cntChoices > 0)
                    {
                        if (bitChoices &
                            ((ULONGLONG)1 << (i*m_dimRegion + j)))
                        {
                            wprintf(L"%2d", i*m_dimRegion + j + 1);
                        }
                        else
                        {
                            wprintf(L"  ");
                        }
                    }
                    else if (i != m_dimRegion/2)
                    {
                        wprintf(L"  ");
                    }
                    else if (j == m_dimRegion/2 - 1)
                    {
                        wprintf(L" %c",
                                (dwCellFlags & CELLF_SOLVED)? L'=': L'#');
                    }
                    else if (j == m_dimRegion/2)
                    {
                        wprintf(L"%2d", GetCellValue(row, col));
                    }
                    else
                    {
                        wprintf(L"  ");
                    }
                }
            }
            wprintf(L"|\n");
        }
    }

    wprintf(L"   ");
    for (col = 0; col < m_dimBoard; col++)
    {
        wprintf(L"+");
        for (i = 0; i < m_dimRegion; i++)
        {
            wprintf(L"--");
        }
    }
    wprintf(L"+\n");

    TExitVoid(BOARD, WPPIndent);
    return;
}   //PrintBoard

/**
 *  This function prints the game summary.
 */
void
Board::PrintSummary(
    void
    )
{
    TEnterVoid(BOARD, WPPIndent);

    wprintf(L"       Number of given cells = %d\n", m_cntInitCells);
    wprintf(L"      Number of solved cells = %d\n", m_cntSolvedCells);

    TExitVoid(BOARD, WPPIndent);
    return;
}   //PrintSummary

