#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Triplets.h" />
///
/// <summary>
///     This module contains the definitions of the Triplets class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class Triplets: public Rule
{
private:
    int m_idxRule;

public:
    Triplets(
        __in int dimRegion
        );

    ~Triplets(
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
    TripletsInRow(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    BOOL
    TripletsInCol(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    BOOL
    TripletsInRegion(
        __in Board *board,
        __in int row,
        __in int col,
        __in ULONGLONG cellChoices
        );

    int
    FindTripletInRow(
        __in Board *board,
        __in int row,
        __in int col,
        __in int c1,
        __in ULONGLONG bitTriplets
        );

    int
    FindTripletInCol(
        __in Board *board,
        __in int row,
        __in int col,
        __in int r1,
        __in ULONGLONG bitTriplets
        );

    int
    FindTripletInRegion(
        __in Board *board,
        __in int row,
        __in int col,
        __in int r1,
        __in int c1,
        __in ULONGLONG bitTriplets
        );
};

