#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="OnlyChoice.h" />
///
/// <summary>
///     This module contains the definitions of the OnlyChoice class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class OnlyChoice: public Rule
{
private:
    int m_idxRule;

public:
    OnlyChoice(
        __in int dimRegion
        );

    ~OnlyChoice(
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
};

