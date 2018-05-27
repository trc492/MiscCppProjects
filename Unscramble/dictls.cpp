#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="DictLS.cpp" />
///
/// <summary>
///     This module contains the implementation of the LSDictionary class.
///     The LSDictionary class searches word linearly in a sorted dictionary
///     text file.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 07-July-2010
///     Environment: User mode
/// </remarks>
#endif

#include "pch.h"
#ifdef _USE_WPP_TRACING
#include "dictls.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 *  It opens the dictionary file and throws an error if it failed.
 *
 *  @param pszFile Points to the dictionary file name string.
 */
LSDictionary::LSDictionary(
    __in LPCSTR pszFile
    ): m_pfileDict(NULL),
       m_pszCurrWord(NULL),
       m_pszSep(" \t\n"),
       m_pszContext(NULL)
{
    DWORD dwErr;

    TEnter(DICT, WPPIndent, "File=%s", pszFile);

    m_szLine[0] = '\0';
    m_szLastWord[0] = '\0';
    if (fopen_s(&m_pfileDict, pszFile, "r") != 0)
    {
        dwErr = GetLastError();
        TMsgErr(Err, dwErr, "Failed to open dictionary file <%s> (err=%d).",
                pszFile, dwErr);
        throw dwErr;
    }

    TExitVoid(DICT, WPPIndent);
    return;
}   //LSDictionary

/**
 *  Destructor to destroy an instance of the object.
 *  It closes the dictionary file.
 */
LSDictionary::~LSDictionary(
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
}   //~LSDictionary

/**
 *  This method looks up the given word in the dictionary scanning the
 *  dictionary linearly. It has an optimzation that it remembers the
 *  last word we looked up. If the given word is alphabetically higher
 *  than the last word we looked up, we continue the dictionary scan
 *  from the last word's position. If it is alphabetically lower than
 *  the last word we looked up, we have no choice but to reset the
 *  dictionary file and search from the beginning again. Therefore,
 *  it is beneficial for the caller to sort the word list before
 *  calling this method to look them up one after the other. If the
 *  word list is sorted, we will only do one pass on the dictionary
 *  no matter how many words we are going to look up because the
 *  dictionary is also sorted.
 *
 *  @param pszWord Points to the word to look up.
 *  @param fTrace If TRUE, turn tracing ON, FALSE otherwise.
 *
 *  @return Success: Returns ERROR_SUCCESS.
 *  @return Failure: Returns Win32 error code.
 */
DWORD
LSDictionary::LookupWord(
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
    else
    {
        int result;
        //
        // If we remembered the last word searched, check to make sure the
        // new word is alphabetically higher than the last word. If not, we
        // must reset the dictionary file back to the beginning.
        //
        if ((m_szLastWord[0] != '\0') &&
            (_stricmp(pszWord, m_szLastWord) < 0))
        {
            m_szLastWord[0] = '\0';
            m_szLine[0] = '\0';
            m_pszCurrWord = NULL;
            fseek(m_pfileDict, 0, SEEK_SET);
        }

        if (m_pszCurrWord == NULL)
        {
            m_pszCurrWord = GetNextWord();
        }

        while (m_pszCurrWord != NULL)
        {
            result = _stricmp(pszWord, m_pszCurrWord);

            if (fTrace)
            {
                printf("Comparing <%s> to <%s> = %d\n",
                       pszWord, m_pszCurrWord, result);
            }

            if (result > 0)
            {
                //
                // The word is after the current point, go forward.
                //
                m_pszCurrWord = GetNextWord();
            }
            else if (result < 0)
            {
                //
                // We don't have a match.
                //
                break;
            }
            else
            {
                //
                // We have a match, we are done.
                //
                fFound = TRUE;
                break;
            }
        }

        strcpy_s(m_szLastWord, ARRAYSIZE(m_szLastWord), pszWord);
    }

    if ((rc == ERROR_SUCCESS) && !fFound)
    {
        rc = ERROR_NOT_FOUND;
    }

    TExitErr(DICT, WPPIndent, rc);
    return rc;
}   //LookupWord

/**
 *  This method get the next word from the dictionary file.
 *
 *  @return Success: Returns a pointer to the next word.
 *  @return Failure: Returns NULL.
 */
LPSTR
LSDictionary::GetNextWord(
    void
    )
{
    LPSTR pszWord = NULL;

    TEnterVoid(DICT, WPPIndent);

    if (m_szLine[0] != '\0')
    {
        //
        // We have a line, get the next token from the line.
        //
        pszWord = strtok_s(NULL, m_pszSep, &m_pszContext);
        if (pszWord == NULL)
        {
            //
            // There is no more token in the line, get rid of the line.
            //
            m_szLine[0] = '\0';
        }
    }

    if (pszWord == NULL)
    {
        //
        // While we don't have a valid line, get the next line.
        //
        while (m_szLine[0] == '\0')
        {
            if (fgets(m_szLine, ARRAYSIZE(m_szLine), m_pfileDict) == NULL)
            {
                //
                // There is no more line, we quit.
                //
                break;
            }
            else
            {
                //
                // Got a new line. Get the first token from the line.
                //
                pszWord = strtok_s(m_szLine, m_pszSep, &m_pszContext);
                if (pszWord == NULL)
                {
                    //
                    // The line has no token, get the next line.
                    //
                    m_szLine[0] = '\0';
                }
            }
        }
    }

    TExitStr(DICT, WPPIndent, pszWord);
    return pszWord;
}   //GetNextWord

