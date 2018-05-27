#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Twins.h" />
///
/// <summary>
///     This module contains the definitions of the Twins class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class Twins: public Rule
{
private:
    int m_idxRule;

public:
    Twins(
        __in int dimRegion
        );

    ~Twins(
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
    TwinsInRow(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    BOOL
    TwinsInCol(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    BOOL
    TwinsInRegion(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    int
    CountCellsInRowWithChoice(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG bitChoice
        );

    int
    CountCellsInColWithChoice(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG bitChoice
        );

    int
    CountCellsInRegionWithChoice(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG bitChoice
        );
};

