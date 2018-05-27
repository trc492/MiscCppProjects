#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Rule.cpp" />
///
/// <summary>
///     This module contains the implementation of the Rule class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode
/// </remarks>
#endif

#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "rule.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param dimRegion Specifies the dimension of the region.
 */
Rule::Rule(
    __in int dimRegion
    ): m_dimRegion(dimRegion),
       m_dimBoard(dimRegion*dimRegion)
{
    TEnterVoid(RULE, WPPIndent);
    TExitVoid(RULE, WPPIndent);
    return;
}   //Rule

/**
 *  Destructor to destroy an instance of the object.
 */
Rule::~Rule(
    void
    )
{
    TEnterVoid(RULE, WPPIndent);
    TExitVoid(RULE, WPPIndent);
    return;
}   //~Rule

