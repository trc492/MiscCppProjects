#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="Dict.h" />
///
/// <summary>
///     This module contains the definitions of the Dictionary class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 07-July-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class Dictionary
{
public:
    virtual
    DWORD
    LookupWord(
        __in LPCSTR pszWord,
        __in BOOL fTrace
        ) = 0;
};

