#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="pch.h" />
///
/// <summary>
///     This module contains all the common definitions.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 02-July-2010
///     Environment: User mode
/// </remarks>
#endif

#pragma once

#if DBG
  #ifndef DEBUG
    #define DEBUG
  #endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include "avlnode.h"
#include "avltree.h"
#include "dict.h"
#include "dictls.h"
#include "dictbs.h"
#include "dictavl.h"
#include "resource.h"

#define WPP_CTRL_GUID (aafe324e,4ed1,4590,833d,31f96b63abfd)
#if DBG
  #define WPP_CUSTOM_FLAGS      WPP_TRACE_FLAGS         \
                                WPP_DEFINE_BIT(MAIN)    \
                                WPP_DEFINE_BIT(DICT)    \
                                WPP_DEFINE_BIT(AVLTREE) \
                                WPP_DEFINE_BIT(AVLNODE) \
                                WPP_DEFINE_BIT(UTIL)
  #include "wtraced.h"
  #include "wtracehelper.h"
#else
  #include "wtrace.h"
#endif
