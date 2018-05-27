#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Sudoku.h" />
///
/// <summary>
///     This module contains global definitions of the sudoku solver program.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

#if DBG
  #ifndef DEBUG
    #define DEBUG
  #endif
#endif

class Board;

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>
#include "util.h"
#include "cell.h"
#include "rule.h"
#include "onlychoice.h"
#include "nowhereelse.h"
#include "subgroup.h"
#include "twins.h"
#include "triplets.h"
#include "board.h"
#include "resource.h"

#define WPP_CTRL_GUID (a0522675,1564,42ee,be69,adf26adcf588)
#if DBG
  #define WPP_CUSTOM_FLAGS      WPP_TRACE_FLAGS         \
                                WPP_DEFINE_BIT(MAIN)    \
                                WPP_DEFINE_BIT(CELL)    \
                                WPP_DEFINE_BIT(BOARD)   \
                                WPP_DEFINE_BIT(RULE)
  #include "wtraced.h"
  #include "wtracehelper.h"
#else
  #include "wtrace.h"
#endif

//
// Constants.
//
#define DEF_REGION_DIM          3
#define MAX_REGION_DIM          8
#define MAX_SUPPORTED_VALUE     (MAX_REGION_DIM*MAX_REGION_DIM)
#define SF_TRACE                0x00000001
#define SF_VERBOSE              0x00000002

//
// Macors
//
#define SAFE_DELETE(p)          if (p)                  \
                                {                       \
                                    delete p;           \
                                    p = NULL;           \
                                }

//
// Global data.
//
extern DWORD g_dwfSudoku;

