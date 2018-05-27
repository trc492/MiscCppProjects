#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Board.h" />
///
/// <summary>
///     This module contains the definitions of the Board class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class Board
{
private:
    #define IsValidCell(r,c)    (((r) >= 0) && ((r) < m_dimBoard) && \
                                 ((c) >= 0) && ((c) < m_dimBoard))
    int     m_dimRegion;
    int     m_dimBoard;
    BOOL    m_fProgress;
    int     m_cntInitCells;
    int     m_cntSolvedCells;
    int     m_cntRuleFinds[MAX_NUM_RULES];
    int     m_cntRuleExcludes[MAX_NUM_RULES];
    Cell ***m_Cells;

public:
    Board(
        __in int dimRegion
        );

    ~Board(
        void
        );

    int
    GetCellValue(
        __in int row,
        __in int col
        );

    BOOL
    SetCellValue(
        __in int row,
        __in int col,
        __in int value,
        __in DWORD dwFlags
        );

    ULONGLONG
    GetCellChoices(
        __in      int row,
        __in      int col,
        __out_opt int *pcntChoices
        );

    BOOL
    ExcludeCellChoices(
        __in int row,
        __in int col,
        __in ULONGLONG bitChoices
        );

    BOOL
    ExcludeCellValue(
        __in int row,
        __in int col,
        __in int value
        );

    DWORD
    GetCellFlags(
        __in int row,
        __in int col
        );

    DWORD
    SetCellFlags(
        __in int row,
        __in int col,
        __in DWORD flags
        );

    BOOL
    GetProgress(
        void
        );

    void
    ClearProgress(
        void
        );

    int
    GetInitCellCount(
        void
        );

    int
    GetSolvedCellCount(
        void
        );

    int
    GetRuleFinds(
        __in int idx
        );

    int
    GetRuleExcludes(
        __in int idx
        );

    int
    IncrementRuleFinds(
        __in int idx
        );

    int
    IncrementRuleExcludes(
        __in int idx
        );

    void
    ProcessBoard(
        __in Rule **RuleTable,
        __in int    NumRules
        );

    void
    PrintBoard(
        void
        );

    void
    PrintSummary(
        void
        );
};

