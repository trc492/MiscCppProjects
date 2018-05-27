#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="DictBS.h" />
///
/// <summary>
///     This module contains the definitions of the BSDictionary class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 07-July-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class BSDictionary: public Dictionary
{
private:
    #define MAX_WORDBUFF_LEN    32
    typedef struct _DICTWORD
    {
        CHAR    szWord[MAX_WORDBUFF_LEN];
        INT64   posWord;
    } DICTWORD, *PDICTWORD;

    LPCSTR          m_pszSep;
    FILE           *m_pfileDict;
    INT64           m_FileSize;
    DICTWORD        m_wordFirst;
    DICTWORD        m_wordLast;
    DICTWORD        m_wordCurr;
    LARGE_INTEGER   m_freqPerf;
    LARGE_INTEGER   m_timeOverhead;

    DWORD
    GetWord(
        __out PDICTWORD Word,
        __in  INT64 Pos
        );

public:
    BSDictionary(
        __in LPCSTR pszFile
        );

    ~BSDictionary(
        void
        );

    virtual
    DWORD
    LookupWord(
        __in LPCSTR pszWord,
        __in BOOL fTrace
        );
};

