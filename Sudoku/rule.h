#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Rule.h" />
///
/// <summary>
///     This module contains the definitions of the Rule class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 16-June-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

#define MAX_NUM_RULES                   8
#define CELLF_ROW_RULE_MASK             0x000000ff
#define CELLF_COL_RULE_MASK             0x0000ff00
#define CELLF_REGION_RULE_MASK          0x00ff0000
#define CELLF_RULE_MASK                 (CELLF_ROW_RULE_MASK | \
                                         CELLF_COL_RULE_MASK | \
                                         CELLF_REGION_RULE_MASK)

#define ROW_RULE(f)                     ((f) && CELLF_ROW_RULE_MASK)
#define COL_RULE(f)                     (((f) << 8) & CELLF_COL_RULE_MASK)
#define REGION_RULE(f)                  (((f) << 16) & CELLF_REGION_RULE_MASK)

#define CELLF_ONLYCHOICE                0x01
#define CELLF_ROW_ONLYCHOICE            ROW_RULE(CELLF_ONLYCHOICE)
#define CELLF_COL_ONLYCHOICE            COL_RULE(CELLF_ONLYCHOICE)
#define CELLF_REGION_ONLYCHOICE         REGION_RULE(CELLF_ONLYCHOICE)

#define CELLF_NOWHEREELSE               0x02
#define CELLF_ROW_NOWHEREELSE           ROW_RULE(CELLF_NOWHEREELSE)
#define CELLF_COL_NOWHEREELSE           COL_RULE(CELLF_NOWHEREELSE)
#define CELLF_REGION_NOWHEREELSE        REGION_RULE(CELLF_NOWHEREELSE)

#define CELLF_SUBGROUP                  0x04
#define CELLF_ROW_SUBGROUP              ROW_RULE(CELLF_SUBGROUP)
#define CELLF_COL_SUBGROUP              COL_RULE(CELLF_SUBGROUP)
#define CELLF_REGION_SUBGROUP           REGION_RULE(CELLF_SUBGROUP)

#define CELLF_TWINS                     0x08
#define CELLF_ROW_TWINS                 ROW_RULE(CELLF_TWINS)
#define CELLF_COL_TWINS                 COL_RULE(CELLF_TWINS)
#define CELLF_REGION_TWINS              REGION_RULE(CELLF_TWINS)

#define CELLF_TRIPLETS                  0x10
#define CELLF_ROW_TRIPLETS              ROW_RULE(CELLF_TRIPLETS)
#define CELLF_COL_TRIPLETS              COL_RULE(CELLF_TRIPLETS)
#define CELLF_REGION_TRIPLETS           REGION_RULE(CELLF_TRIPLETS)

#define CELLF_SOLVERULES_MASK           (CELLF_ONLYCHOICE | CELLF_NOWHEREELSE)

class Rule
{
protected:
    int m_dimRegion;
    int m_dimBoard;

public:
    Rule(
        __in int dimRegion
        );

    ~Rule(
        void
        );

    virtual
    BOOL
    ProcessCell(
        __in Board *board,
        __in int row,
        __in int col
        ) = 0;

    virtual
    void
    PrintSummary(
        __in Board *board
        ) = 0;
};

