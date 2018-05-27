#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="unscramble.cpp" />
///
/// <summary>
///     This program unscrambles a given word.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 02-July-2010
///     Environment: User mode
/// </remarks>
#endif

#ifdef _USE_WPP_TRACING
#define WPP_HELPER_FUNC
#endif
#include "pch.h"
#ifdef _USE_WPP_TRACING
#include "unscramble.tmh"
#endif

//
// Constants.
//
#define USF_TRACE_ON            0x80000000
#define USF_LIST_COMBOS         0x00000001
#define USF_USE_BINSEARCH       0x00000002
#define USF_USE_AVLSEARCH       0x00000004
#define USF_PRINT_TREE          0x00000008

//
// Global data.
//
LPSTR       g_pszProgName = NULL;
DWORD       g_dwfUnscramble = 0;
ULONGLONG   g_cntTotalCombos = 0;
ULONGLONG   g_cntTotalWords = 0;
int        *g_piWordLens = NULL;
int         g_numWords = 0;
Dictionary *g_Dict = NULL;
LPSTR       g_pszLastCombo = NULL;
int         g_lenLastCombo = 0;

//
// Local function prototypes.
//
void
PrintWordCombo(
    __inout LPSTR pszPrefix,
    __in    size_t lenPrefix,
    __inout LPSTR pszWord,
    __in    size_t lenWord
    );

DWORD
SplitWords(
    __inout LPSTR pszComboWord
    );

DWORD
CheckWords(
    __in LPSTR pszComboWord
    );

void
PrintUsage(
    void
    );

void
PrintBanner(
    void
    );

void
PrintErrMsg(
    __in DWORD dwErr
    );

int
__cdecl
CompareChar(
    __in const void *arg1,
    __in const void *arg2
    );

/**
 *  Program main entry point.
 *
 *  @param icArgs Specifies the number of command line arguments.
 *  @param apszArgs Points to the array of argument strings.
 *
 *  @return Success: Returns 0.
 *  @return Failure: Returns -1.
 */
int __cdecl
main(
    __in                int   icArgs,
    __in_ecount(icArgs) LPSTR apszArgs[]
    )
{
    int rc = 0;
    LPSTR pszScrambledWord = NULL;

    WPP_INIT_TRACING(L"Unscramble");
    TEnter(MAIN, WPPIndent, "icArgs=%d,apszArgs=%p", icArgs, apszArgs);
    //
    // Isolate the program name.
    //
    g_pszProgName = strrchr(apszArgs[0], '\\');
    if (g_pszProgName != NULL)
    {
        LPSTR psz;

        g_pszProgName[0] = '\0';
        g_pszProgName++;
        psz = strrchr(g_pszProgName, '.');
        if (psz != NULL)
        {
            *psz = '\0';
        }
    }
    else
    {
        g_pszProgName = MODNAME;
    }
    icArgs--;
    apszArgs++;

    PrintBanner();
    //
    // Parse the command line options.
    //
    while ((icArgs > 1) &&
           ((apszArgs[0][0] == '/') || (apszArgs[0][0] == '-')))
    {
        if (_stricmp(&apszArgs[0][1], "l") == 0)
        {
            g_dwfUnscramble |= USF_LIST_COMBOS;
        }
        else if (_stricmp(&apszArgs[0][1], "t") == 0)
        {
            g_dwfUnscramble |= USF_TRACE_ON;
        }
        else if (_stricmp(&apszArgs[0][1], "pt") == 0)
        {
            g_dwfUnscramble |= USF_PRINT_TREE;
        }
        else if (_stricmp(&apszArgs[0][1], "bs") == 0)
        {
            g_dwfUnscramble |= USF_USE_BINSEARCH;
        }
        else if (_stricmp(&apszArgs[0][1], "avl") == 0)
        {
            g_dwfUnscramble |= USF_USE_AVLSEARCH;
        }
        else
        {
            printf("%s: Invalid option <%s>.\n", g_pszProgName, apszArgs[0]);
            PrintUsage();
            rc = -1;
        }
        icArgs--;
        apszArgs++;
    }

    if ((rc == 0) && (icArgs < 1))
    {
        printf("%s: Invalid command syntax.\n", g_pszProgName);
        PrintUsage();
        rc = -1;
    }

    if (rc == 0)
    {
        pszScrambledWord = apszArgs[0];
        icArgs--;
        apszArgs++;
        if (icArgs > 0)
        {
            g_piWordLens = new int[icArgs];
            if (g_piWordLens == NULL)
            {
                printf("%s: Failed to allocate word length array (len=%d).\n",
                       g_pszProgName, icArgs);
                rc = -1;
            }
            else
            {
                g_numWords = icArgs;
                for (int i = 0; i < g_numWords; i++)
                {
                    g_piWordLens[i] = atoi(apszArgs[i]);
                }
                //
                // If we have split word scenario, we better use AVL tree
                // search since the words are no longer sorted so the linear
                // search will perform horribly.
                //
                g_dwfUnscramble |= USF_USE_AVLSEARCH;
            }
        }
    }

    if (rc == 0)
    {
        size_t lenWord;
        LPSTR pszPrefix = NULL;

        lenWord = strlen(pszScrambledWord);
        pszPrefix = new CHAR[lenWord + 1];
        g_lenLastCombo = (int)(lenWord + g_numWords);
        g_pszLastCombo = new CHAR[g_lenLastCombo];
        if (pszPrefix == NULL)
        {
            printf("%s: Failed to allocate prefix string (len=%Id).\n",
                   g_pszProgName, lenWord + 1);
            rc = -1;
        }
        else if (g_pszLastCombo == NULL)
        {
            printf("%s: Failed to allocate last combo string (len=%Id).\n",
                   g_pszProgName, g_lenLastCombo);
            rc = -1;
        }
        else
        {
            LARGE_INTEGER freqPerf;
            LARGE_INTEGER timeOverhead;
            LARGE_INTEGER timeStart;
            LARGE_INTEGER timeStop;
            //
            // Initialize the performance time stamp.
            //
            QueryPerformanceFrequency(&freqPerf);
            QueryPerformanceCounter(&timeStart);
            QueryPerformanceCounter(&timeStop);
            timeOverhead.QuadPart = timeStop.QuadPart - timeStart.QuadPart;

            if (!(g_dwfUnscramble & USF_LIST_COMBOS))
            {
                CHAR szDictFile[MAX_PATH];
                LPSTR psz;
                DWORD dwErr;
                //
                // Instantiate the dictionary object.
                //
                GetModuleFileNameA(NULL, szDictFile, ARRAYSIZE(szDictFile));
                dwErr = GetLastError();
                if (dwErr != ERROR_SUCCESS)
                {
                    printf("%s: Failed to get dictionary file path.\n",
                           g_pszProgName);
                    PrintErrMsg(dwErr);
                    rc = -1;
                }
                else if ((psz = strrchr(szDictFile, '\\')) == NULL)
                {
                    printf("%s: Invalid dictionary file path <%s>.\n",
                           g_pszProgName, szDictFile);
                    rc = -1;
                }
                else
                {
                    psz++;
                    *psz = '\0';
                    strcat_s(szDictFile, ARRAYSIZE(szDictFile), "en-us.dic");
                    try
                    {
                        if (g_dwfUnscramble & USF_USE_AVLSEARCH)
                        {
                            QueryPerformanceCounter(&timeStart);
                            g_Dict = new AVLDictionary(szDictFile);
                            QueryPerformanceCounter(&timeStop);
                            printf("AVL dictionary loaded in %I64d msec.\n\n",
                                   (timeStop.QuadPart - timeStart.QuadPart -
                                    timeOverhead.QuadPart)*
                                   1000/freqPerf.QuadPart);
                        }
                        else if (g_dwfUnscramble & USF_USE_BINSEARCH)
                        {
                            g_Dict = new BSDictionary(szDictFile);
                        }
                        else
                        {
                            g_Dict = new LSDictionary(szDictFile);
                        }
                    }
                    catch(DWORD dwErr)
                    {
                        printf(
                            "%s: Failed to create dictionary object <%s>.\n",
                            g_pszProgName, szDictFile);
                        PrintErrMsg(dwErr);
                        rc = -1;
                    }
                }
            }

            if (rc == 0)
            {
                //
                // Clear the prefix and sort the incoming word so that the
                // resulting combinations will be in sorted order.
                //
                ZeroMemory(pszPrefix, sizeof(CHAR)*(lenWord + 1));
                qsort(pszScrambledWord,
                      lenWord,
                      sizeof(pszScrambledWord[0]),
                      CompareChar);
                //
                // Start the performance time stamp. Print all possible
                // unique combintions of the word.
                //
                QueryPerformanceCounter(&timeStart);
                PrintWordCombo(pszPrefix, 0, pszScrambledWord, lenWord);
                QueryPerformanceCounter(&timeStop);
                //
                // Prints the statistics of the operation.
                //
                if (g_dwfUnscramble & USF_LIST_COMBOS)
                {
                    printf("\nNumber of word combinations = %I64u\n",
                           g_cntTotalCombos);
                }
                else
                {
                    printf("\nFound %I64u solution%s out of %I64u combination%s.\n",
                           g_cntTotalWords, (g_cntTotalWords > 1)? "s": "",
                           g_cntTotalCombos, (g_cntTotalCombos > 1)? "s": "");
                }
                printf("Executed in %I64d msec.\n",
                       (timeStop.QuadPart -
                        timeStart.QuadPart -
                        timeOverhead.QuadPart)*1000/freqPerf.QuadPart);

                if ((g_dwfUnscramble & USF_USE_AVLSEARCH) &&
                    (g_dwfUnscramble & USF_PRINT_TREE))
                {
                    ((AVLDictionary *)g_Dict)->PrintDictTree();
                }
            }

            if (g_Dict != NULL)
            {
                delete g_Dict;
                g_Dict = NULL;
            }
        }

        if (g_pszLastCombo != NULL)
        {
            delete [] g_pszLastCombo;
            g_pszLastCombo = NULL;
        }

        if (pszPrefix != NULL)
        {
            delete [] pszPrefix;
            pszPrefix = NULL;
        }

        if (g_piWordLens != NULL)
        {
            delete [] g_piWordLens;
            g_piWordLens = NULL;
            g_numWords = 0;
        }
    }


    TExitDec(MAIN, WPPIndent, rc);
    WPP_CLEANUP();
    return rc;
}       //main

/**
 *  This function prints out all the combinations of a word. It does this by
 *  recursion. When this function is first called, Prefix is empty. This
 *  function then removes the first letter of the word and appends it to the
 *  end of Prefix. It then calls itself to print all combinations of the
 *  remaining word (Suffix). After all combinations of the Suffix are printed,
 *  it swaps each subsequent letters in the Suffix with the last letter of
 *  the Prefix and loops back to print all combinations of the new Suffix.
 *  An optimization compares each subsequent letter in the Suffix with the
 *  last letter of the Prefix. If they are the same, we can skip that
 *  particular set of combinations because they would be duplicates of the
 *  combination set before. In addition, if the incoming word is sorted, this
 *  algorithm will produce the combinations in sorted order.
 *
 *  @param pszPrefix Points to the prefix string.
 *  @param lenPrefix Specifies the length of the prefix in number of characters.
 *  @param pszWord Points to the scrambled word string.
 *  @param lenWord Specifies the length of the word in number of characters.
 *
 *  @return None.
 */
void
PrintWordCombo(
    __inout LPSTR pszPrefix,
    __in    size_t lenPrefix,
    __inout LPSTR pszWord,
    __in    size_t lenWord
    )
{
    TEnter(MAIN, WPPIndent, "Prefix=%s,lenPrefix=%Id,Word=%s,lenWord=%Id",
           pszPrefix, lenPrefix, pszWord, lenWord);

    if (lenWord <= 1)
    {
        //
        // Suffix word has only one letter left, there is no other
        // combination. So print it with the Prefix.
        //
        sprintf_s(g_pszLastCombo, lenPrefix + 2, "%s%s", pszPrefix, pszWord);
        if (g_numWords > 0)
        {
            SplitWords(g_pszLastCombo);
        }

        if ((g_Dict == NULL) || (CheckWords(g_pszLastCombo) == ERROR_SUCCESS))
        {
            printf("%s\n", g_pszLastCombo);
            g_cntTotalWords++;
        }
        g_cntTotalCombos++;
    }
    else
    {
        size_t i;
        CHAR ch;
        //
        // Move the first letter of the Suffix word to the end of the Prefix.
        //
        pszPrefix[lenPrefix] = pszWord[0];
        for (i = 1; i < lenWord + 1; i++)
        {
            pszWord[i - 1] = pszWord[i];
        }

        for (i = 0; i < lenWord; i++)
        {
            if ((i == 0) || (pszPrefix[lenPrefix] != pszWord[i - 1]))
            {
                //
                // We only process the combo set if the char we swapped was
                // different from the last combo. This trick will eliminate
                // the duplicates.
                //
                PrintWordCombo(pszPrefix, lenPrefix + 1,
                               pszWord, lenWord - 1);
            }
            //
            // Switching the next letter of the Suffix with the last letter
            // of the Prefix.
            //
            ch = pszPrefix[lenPrefix];
            pszPrefix[lenPrefix] = pszWord[i];
            pszWord[i] = ch;
        }
    }

    TExitVoid(MAIN, WPPIndent);
    return;
}   //PrintWordCombo

/**
 *  This function splits the word combo into multiple words with spaces in
 *  between according to the given word length array.
 *
 *  @param pszComboWord Points to the combo word.
 *
 *  @return Success: Returns ERROR_SUCCESS.
 *  @return Failure: Returns Win32 error code.
 */
DWORD
SplitWords(
    __inout LPSTR pszComboWord
    )
{
    DWORD rc = ERROR_SUCCESS;
    LPSTR pszDup;

    TEnter(DICT, WPPIndent, "ComboWord=%s", pszComboWord);

    pszDup = _strdup(pszComboWord);
    if (pszDup == NULL)
    {
        printf("%s: Failed to duplicate combo word <%s>.\n",
               g_pszProgName, pszComboWord);
        rc = ERROR_OUTOFMEMORY;
    }
    else
    {
        int i, len;
        LPSTR psz1, psz2;

        for (i = 0, len = g_lenLastCombo, psz1 = pszComboWord, psz2 = pszDup;
             i < g_numWords;
             i++)
        {
            strncpy_s(psz1, len, psz2, g_piWordLens[i]);
            psz1[g_piWordLens[i]] = ' ';
            psz1 += g_piWordLens[i] + 1;
            len -= g_piWordLens[i] + 1;
            psz2 += g_piWordLens[i];
        }
        psz1--;
        psz1[0] = '\0';
        free(pszDup);
    }

    TExitErrMsg(DICT, WPPIndent, rc, "(Word=%s)", pszComboWord);
    return rc;
}   //SplitWords

/**
 *  This function breaks up the given word into multiple words according to
 *  the word length array and call the dictionary to look up each word. It
 *  will return ERROR_NOT_FOUND unless all the words are found in the
 *  dictionary.
 *
 *  @param pszComboWord Points to the combo words to look up.
 *
 *  @return Success: Returns ERROR_SUCCESS.
 *  @return Failure: Returns Win32 error code.
 */
DWORD
CheckWords(
    __in LPSTR pszComboWord
    )
{
    DWORD rc = ERROR_SUCCESS;
    int i;
    LPSTR psz;

    TEnter(DICT, WPPIndent, "ComboWord=%s", pszComboWord);

    if (g_numWords == 0)
    {
        rc = g_Dict->LookupWord(pszComboWord,
                                (g_dwfUnscramble & USF_TRACE_ON) != 0);
    }
    else
    {
        for (i = 0, psz = pszComboWord;
             (rc == ERROR_SUCCESS) && (i < g_numWords);
             i++)
        {
            psz[g_piWordLens[i]] = '\0';
            rc = g_Dict->LookupWord(psz,
                                    (g_dwfUnscramble & USF_TRACE_ON) != 0);
            if (i + 1 < g_numWords)
            {
                psz[g_piWordLens[i]] = ' ';
                psz += g_piWordLens[i] + 1;
            }
        }
    }

    TExitErr(DICT, WPPIndent, rc);
    return rc;
}   //CheckWords

/**
 *  This function prints the usage help message.
 *
 *  @return None.
 */
void
PrintUsage(
    void
    )
{
    TEnterVoid(Util, WPPIndent);

    printf("\nUsage:\t%s [/l] [/t] [/bs] [/avl] <Word> {<WordLen>}\n",
           g_pszProgName);
    printf("\tl         - List all unique combinations.\n"
           "\tt         - Turn on debug trace.\n"
           "\tpt        - Print the AVL tree (only valid with avl option).\n"
           "\tbs        - Do binary search on a linear dictionary file.\n"
           "\tavl       - Do binary search on an AVL tree.\n"
           "\t<Word>    - Specifies the word to unscramble.\n"
           "\t<WordLen> - If unscramble to multiple words, specify word\n"
           "\t            length of each word.\n");

    TExitVoid(Util, WPPIndent);
    return;
}   //PrintUsage

/**
 *  This function prints the program banner.
 *
 *  @return None.
 */
void
PrintBanner(
    void
    )
{
    TEnterVoid(Util, WPPIndent);

    printf("\n%s v%02d.%02d.%02d\n%s\n\n",
           STR_PROGTITLE, VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD,
           STR_COPYRIGHT);

    TExitVoid(Util, WPPIndent);
    return;
}   //PrintBanner

/**
 *  This function prints the message associated with the given error code.
 *
 *  @param dwErr Specifies the Win32 error code.
 *
 *  @return None.
 */
void
PrintErrMsg(
    __in DWORD dwErr
    )
{
    static CHAR szMsg[256];

    TEnter(Util, WPPIndent, "Err=%d", dwErr);

    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,
                   dwErr,
                   0,
                   szMsg, ARRAYSIZE(szMsg),
                   NULL);
    printf("Error: %s\n", szMsg);

    TExitVoid(Util, WPPIndent);
    return;
}   //PrintErrMsg

/**
 *  This function is called by the qsort C-runtime library to compare the
 *  character element in two strings.
 *
 *  @param arg1 Points to the character of the first string.
 *  @param arg2 Points to the character of the second string.
 *
 *  @returns Returns -1 if arg1 is less than arg2.
 *           Returns 0 if arg1 is equal to arg2.
 *           Returns 1 if arg1 is greater than arg2.
 */
int
__cdecl
CompareChar(
    __in const void *arg1,
    __in const void *arg2
    )
{
    int rc;

    TEnter(UTIL, WPPIndent, "arg1=%x,arg2=%x",
           *(CHAR *)arg1, *(CHAR *)arg2);

    rc = *(CHAR *)arg1 - *(CHAR *)arg2;

    TExitDec(UTIL, WPPIndent, rc);
    return rc;
}   //CompareChar

