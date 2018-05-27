#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="DictBS.cpp" />
///
/// <summary>
///     This module contains the implementation of the BSDictionary class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 07-July-2010
///     Environment: User mode
/// </remarks>
#endif

#include "pch.h"
#ifdef _USE_WPP_TRACING
#include "dictbs.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *
 *  @param pszFile Points to the dictionary file name string.
 */
BSDictionary::BSDictionary(
    __in LPCSTR pszFile
    ): m_pszSep(" \t\r\n"),
       m_pfileDict(NULL)
{
    DWORD dwErr = ERROR_SUCCESS;

    TEnter(DICT, WPPIndent, "File=%s", pszFile);

    if (fopen_s(&m_pfileDict, pszFile, "r") != 0)
    {
        dwErr = GetLastError();
        TMsgErr(Err, dwErr, "Failed to open dictionary file <%s>.",
                pszFile);
    }
    else if (_fseeki64(m_pfileDict, 0, SEEK_END))
    {
        dwErr = ERROR_SEEK;
        TMsg(Err, "Failed to seek to end of file.");
    }
    else if ((m_FileSize = _ftelli64(m_pfileDict)) == -1)
    {
        dwErr = ERROR_GEN_FAILURE;
        TMsg(Err, "Failed to determine file size.");
    }
    else if ((dwErr = GetWord(&m_wordFirst, 0)) != ERROR_SUCCESS)
    {
        TMsgErr(Err, dwErr, "Failed to get first word in dictionary.");
    }
    else if ((dwErr = GetWord(&m_wordLast, m_FileSize)) != ERROR_SUCCESS)
    {
        TMsgErr(Err, dwErr, "Failed to get last word in dictionary.");
    }
    else if (!QueryPerformanceFrequency(&m_freqPerf))
    {
        dwErr = GetLastError();
        TMsgErr(Err, dwErr, "Failed to get performance counter frequency.");
    }
    else
    {
        LARGE_INTEGER time1, time2;

        QueryPerformanceCounter(&time1);
        QueryPerformanceCounter(&time2);
        m_timeOverhead.QuadPart = time2.QuadPart - time2.QuadPart;
        m_wordCurr = m_wordFirst;
    }

    if (dwErr != ERROR_SUCCESS)
    {
        throw dwErr;
    }

    TExitVoid(DICT, WPPIndent);
    return;
}   //BSDictionary

/**
 *  Destructor to destroy an instance of the object.
 */
BSDictionary::~BSDictionary(
    void
    )
{
    TEnterVoid(DICT, WPPIndent);

    if (m_pfileDict != NULL)
    {
        fclose(m_pfileDict);
        m_pfileDict = NULL;
    }

    TExitVoid(DICT, WPPIndent);
}   //~BSDictionary

/**
 *  This method looks up the given word in the dictionary.
 *
 *  @param pszWord Points to the word to look up.
 *  @param fTrace If TRUE, turn tracing ON, FALSE otherwise.
 *
 *  @return Success: Returns ERROR_SUCCESS.
 *  @return Failure: Returns Win32 error code.
 */
DWORD
BSDictionary::LookupWord(
    __in LPCSTR pszWord,
    __in BOOL fTrace
    )
{
    DWORD rc = ERROR_SUCCESS;
    BOOL fFound = FALSE;

    TEnter(DICT, WPPIndent, "Word=%s,fTrace=%d", pszWord, fTrace);

    if (m_pfileDict == NULL)
    {
        TMsg(Err, "Dictionary is not initialized.");
        rc = ERROR_APP_INIT_FAILURE;
    }
    else if ((_stricmp(pszWord, m_wordFirst.szWord) == 0) ||
             (_stricmp(pszWord, m_wordLast.szWord) == 0))
    {
        fFound = TRUE;
    }
    else
    {
        DICTWORD lowerBound = m_wordFirst;
        DICTWORD upperBound = m_wordLast;
        int result;
        int cntIterations = 0;
        INT64 pos;
        LARGE_INTEGER timeStart, timeStop;
        
        if (fTrace)
        {
            printf("\n");
        }

        QueryPerformanceCounter(&timeStart);
        while ((rc == ERROR_SUCCESS) && !fFound)
        {
            cntIterations++;
            result = _stricmp(pszWord, m_wordCurr.szWord);

            if (fTrace)
            {
                printf("Comparing <%s> to <%s> = %d\n",
                       pszWord, m_wordCurr.szWord, result);
            }

            if (result == 0)
            {
                fFound = TRUE;
            }
            else if (result < 0)
            {
                upperBound = m_wordCurr;
            }
            else
            {
                lowerBound = m_wordCurr;
            }

            if (!fFound)
            {
                pos = (lowerBound.posWord + upperBound.posWord)/2;
                rc = GetWord(&m_wordCurr, pos);
                if ((rc == ERROR_SUCCESS) &&
                    (m_wordCurr.posWord == lowerBound.posWord))
                {
                    //
                    // There is no more word between lower and upper bounds.
                    //
                    break;
                }
            }
        }
        QueryPerformanceCounter(&timeStop);

        if (fTrace)
        {
            printf("%d iteration%s executed in %I64d msec\n",
                   cntIterations, (cntIterations > 1)? "s": "",
                   (timeStop.QuadPart -
                    timeStart.QuadPart -
                    m_timeOverhead.QuadPart)*1000/m_freqPerf.QuadPart);
        }
    }

    if ((rc == ERROR_SUCCESS) && !fFound)
    {
        rc = ERROR_NOT_FOUND;
    }

    TExitErr(DICT, WPPIndent, rc);
    return rc;
}   //LookupWord

/**
 *  This method get a word from the dictionary file.
 *
 *  @param Word Points to the WORD structure to be filled in.
 *  @param Pos Specifies the approximate dictionary file position to get the
 *         word from.
 *
 *  @return Success: Returns ERROR_SUCCESS.
 *  @return Failure: Returns Win32 error code.
 */
DWORD
BSDictionary::GetWord(
    __out PDICTWORD Word,
    __in  INT64 Pos
    )
{
    DWORD rc = ERROR_SUCCESS;
    CHAR szBuff[MAX_WORDBUFF_LEN*2];
    size_t len;
    INT64 idx;
    BOOL fTopOfFile = FALSE;
    BOOL fEndOfFile = FALSE;

    TEnter(DICT, WPPIndent, "pWord=%p,Pos=%I64d", Word, Pos);
    //
    // We are reading two max words, one max word before and one max word
    // after the given position so that the caller can give us an approximate
    // file postion and we will find the word boundary closest to the
    // given position.
    //
    if (Pos > m_FileSize)
    {
        //
        // Make sure we are not given a file position past the size of
        // the file.
        //
        Pos = m_FileSize;
    }

    if (Pos == 0)
    {
        fTopOfFile = TRUE;
    }
    else if (Pos == m_FileSize)
    {
        fEndOfFile = TRUE;
    }
    //
    // Adjust the position to read from one max word before.
    // Keep idx pointing to the original given position.
    //
    Pos -= MAX_WORDBUFF_LEN;
    idx = MAX_WORDBUFF_LEN;
    if (Pos < 0)
    {
        //
        // Make sure we don't seek past the beginning.
        //
        idx += Pos;
        Pos = 0;
    }

    if (_fseeki64(m_pfileDict, Pos, SEEK_SET) != 0)
    {
        TMsg(Err, "Failed to seek to position %I64d.", Pos);
        rc = ERROR_SEEK;
    }
    else if ((len = fread(szBuff,
                          sizeof(CHAR),
                          ARRAYSIZE(szBuff) - 1,
                          m_pfileDict)) > 0)
    {
        BOOL fFound = FALSE;
        LPSTR psz, pszContext;

        if (idx >= (INT64)len)
        {
            //
            // This should not happen but just in case.
            //
            idx = len - 1;
        }
        //
        // Make sure the string is terminated.
        //
        szBuff[len] = '\0';

        if (fEndOfFile)
        {
            //
            // Skip trailing separators.
            //
            while (idx >= 0)
            {
                if (strchr(m_pszSep, szBuff[idx]) != NULL)
                {
                    idx--;
                }
                else
                {
                    break;
                }
            }
        }

        if (fTopOfFile)
        {
            fFound = TRUE;
        }
        else
        {
            //
            // Scanning backward to look for beginning of the word.
            //
            while (idx >= 0)
            {
                if (strchr(m_pszSep, szBuff[idx]) != NULL)
                {
                    fFound = TRUE;
                    break;
                }
                else
                {
                    idx--;
                }
            }
        }

        if (fFound)
        {
            psz = strtok_s(&szBuff[idx], m_pszSep, &pszContext);
            if ((psz == NULL) || (&psz[strlen(psz)] == &szBuff[len]))
            {
                //
                // We either did not find a word or the word is partial.
                //
                fFound = FALSE;
                rc = ERROR_INVALID_DATA;
                TMsg(Err, "Failed to find a valid word.");
            }
            else
            {
                ZeroMemory(Word->szWord, sizeof(Word->szWord));
                strcpy_s(Word->szWord, ARRAYSIZE(Word->szWord), psz);
                Word->posWord = Pos + (psz - &szBuff[0]);
            }
        }
        else
        {
            rc = ERROR_INVALID_DATA;
            TMsg(Err, "Failed to find the beginning of the word.");
        }
    }
    else
    {
        rc = ERROR_HANDLE_EOF;
    }

    TExitErrMsg(DICT, WPPIndent, rc, "(Word=%s,pos=%I64d)",
                Word->szWord, Word->posWord);
    return rc;
}   //GetWord

