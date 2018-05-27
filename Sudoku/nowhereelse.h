#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="NowhereElse.h" />
///
/// <summary>
///     This module contains the definitions of the NowhereElse class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class NowhereElse: public Rule
{
private:
    int m_idxRule;

public:
    NowhereElse(
        __in int dimRegion
        );

    ~NowhereElse(
        void
        );

    virtual
    BOOL
    ProcessCell(
        __in Board *board,
        __in int row,
        __in int col
        );

    virtual
    void
    PrintSummary(
        __in Board *board
        );

private:
    BOOL
    NowhereElseInRow(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG bitChoice
        );

    BOOL
    NowhereElseInCol(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG bitChoice
        );

    BOOL
    NowhereElseInRegion(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG bitChoice
        );
};

