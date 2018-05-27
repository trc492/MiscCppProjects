#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="DictLS.h" />
///
/// <summary>
///     This module contains the definitions of the LSDictionary class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 07-July-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class LSDictionary: public Dictionary
{
private:
    #define MAX_WORDBUFF_LEN    32

    FILE       *m_pfileDict;
    LPSTR       m_pszCurrWord;
    LPSTR       m_pszSep;
    LPSTR       m_pszContext;
    CHAR        m_szLine[256];
    CHAR        m_szLastWord[MAX_WORDBUFF_LEN];
    
    LPSTR
    GetNextWord(
        void
        );

public:
    LSDictionary(
        __in LPCSTR pszFile
        );

    ~LSDictionary(
        void
        );

    virtual
    DWORD
    LookupWord(
        __in LPCSTR pszWord,
        __in BOOL fTrace
        );
};

