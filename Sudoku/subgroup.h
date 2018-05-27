#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Subgroup.h" />
///
/// <summary>
///     This module contains the definitions of the Subgroup class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 17-July-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class Subgroup: public Rule
{
private:
    int m_idxRule;

public:
    Subgroup(
        __in int dimRegion
        );

    ~Subgroup(
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
    SubgroupInRow(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    BOOL
    SubgroupInCol(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    BOOL
    SubgroupInRegion(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );
};

