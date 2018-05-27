#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="sudoku.cpp" />
///
/// <summary>
///     This program solves a sudoku puzzle.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 27-April-2010
///     Environment: User mode
/// </remarks>
#endif

#ifdef _USE_WPP_TRACING
#define WPP_HELPER_FUNC
#endif
#include "sudoku.h"
#ifdef _USE_WPP_TRACING
#include "sudoku.tmh"
#endif

//
// Global data.
//
DWORD   g_dwfSudoku = 0;

//
// Local data.
//
LPWSTR  g_pszProgName = NULL;
int     g_dimRegion = DEF_REGION_DIM;
int     g_dimBoard = g_dimRegion*g_dimRegion;
Rule   *g_RuleTable[MAX_NUM_RULES] = {NULL};

//
// Local function prototypes.
//
int
ParseArguments(
    __inout                  int *picArgs,
    __deref_ecount(*picArgs) LPWSTR **papszArgs
    );

int
ProcessFile(
    __in_opt LPWSTR pszFile,
    __in     int dimRegion
    );

int
ReadBoard(
    __in FILE *pfile,
    __in Board *board,
    __in int dimBoard
    );

void
PrintBanner(
    void
    );

void
PrintUsage(
    void
    );

void
PrintGameInfo(
    __in Board *board,
    __in int iteration
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
wmain(
    __in                int    icArgs,
    __in_ecount(icArgs) LPWSTR apszArgs[]
    )
{
    int rc = 0;

    WPP_INIT_TRACING(L"Sudoku");
    TEnter(MAIN, WPPIndent, "icArgs=%d,apszArgs=%p", icArgs, apszArgs);

    rc = ParseArguments(&icArgs, &apszArgs);
    if (rc == 0)
    {
        if ((g_RuleTable[LSBitPosition(CELLF_ONLYCHOICE, MAX_NUM_RULES)] =
             new OnlyChoice(g_dimRegion)) == NULL)
        {
            wprintf(L"%s: failed to instantiate the OnlyChoice rule.\n",
                    g_pszProgName);
            rc = -1;
        }
        else if ((g_RuleTable[LSBitPosition(CELLF_NOWHEREELSE, MAX_NUM_RULES)] =
                  new NowhereElse(g_dimRegion)) == NULL)
        {
            wprintf(L"%s: failed to instantiate the NowhereElse rule.\n",
                    g_pszProgName);
            rc = -1;
        }
        else if ((g_RuleTable[LSBitPosition(CELLF_SUBGROUP, MAX_NUM_RULES)] =
                  new Subgroup(g_dimRegion)) == NULL)
        {
            wprintf(L"%s: failed to instantiate the Subgroup rule.\n",
                    g_pszProgName);
            rc = -1;
        }
        else if ((g_RuleTable[LSBitPosition(CELLF_TWINS, MAX_NUM_RULES)] =
                  new Twins(g_dimRegion)) == NULL)
        {
            wprintf(L"%s: failed to instantiate the Twins rule.\n",
                    g_pszProgName);
            rc = -1;
        }
        else if ((g_RuleTable[LSBitPosition(CELLF_TRIPLETS, MAX_NUM_RULES)] =
                  new Triplets(g_dimRegion)) == NULL)
        {
            wprintf(L"%s: failed to instantiate the Triplets rule.\n",
                    g_pszProgName);
            rc = -1;
        }
    }

    if (rc == 0)
    {
        if (icArgs == 0)
        {
            //
            // No file specified, use stdin.
            //
            rc = ProcessFile(NULL, g_dimRegion);
        }
        else
        {
            WCHAR szFilePath[MAX_PATH];
            WIN32_FIND_DATAW FindFileData;
            HANDLE hFind;

            while ((rc == 0) && (icArgs > 0))
            {
                if (StringCchCopyW(szFilePath,
                                   ARRAYSIZE(szFilePath),
                                   apszArgs[0]) != S_OK)
                {
                    wprintf(L"%s: failed to copy file path <%s>.\n",
                            g_pszProgName, apszArgs[0]);
                    rc = -1;
                }
                else
                {
                    LPWSTR psz = wcsrchr(szFilePath, L'\\');
                    size_t len = ARRAYSIZE(szFilePath);

                    if (psz != NULL)
                    {
                        psz++;
                        len -= psz - &szFilePath[0];
                    }
                    else
                    {
                        psz = &szFilePath[0];
                    }

                    hFind = FindFirstFileW(apszArgs[0], &FindFileData);
                    if (hFind == INVALID_HANDLE_VALUE)
                    {
                        wprintf(L"%s: failed to find file <%s>.\n",
                                g_pszProgName, apszArgs[0]);
                        rc = -1;
                    }
                    else
                    {
                        do
                        {
                            if (StringCchCopyW(psz,
                                               len,
                                               FindFileData.cFileName) != S_OK)
                            {
                                wprintf(L"%s: failed to format file path <%s>.\n",
                                        g_pszProgName, FindFileData.cFileName);
                                rc = -1;
                            }
                            else
                            {
                                rc = ProcessFile(szFilePath, g_dimRegion);
                            }
                        } while ((rc == 0) &&
                                 FindNextFileW(hFind, &FindFileData));

                        FindClose(hFind);
                    }
                }
                icArgs--;
                apszArgs++;
            }
        }
    }

    for (int i = 0; i < ARRAYSIZE(g_RuleTable); i++)
    {
        SAFE_DELETE(g_RuleTable[i]);
    }

    TExitDec(MAIN, WPPIndent, rc);
    WPP_CLEANUP();
    return rc;
}       //wmain

/**
 *  This function parses the command line arguments.
 *
 *  @param picArgs Points to the variable that specifies the number of
 *         arguments.
 *  @param papszArgs Points to the variable that points to the array of
 *         argument string pointers.
 *
 *  @return Success: Returns 0.
 *  @return Failure: Returns -1.
 */
int
ParseArguments(
    __inout                  int *picArgs,
    __deref_ecount(*picArgs) LPWSTR **papszArgs
    )
{
    int rc = 0;

    TEnter(MAIN, WPPIndent, "picArgs=%p,papszArgs=%p", picArgs, papszArgs);

    g_pszProgName = wcsrchr((*papszArgs)[0], L'\\');
    if (g_pszProgName != NULL)
    {
        LPWSTR psz;

        g_pszProgName++;
        psz = wcsrchr(g_pszProgName, L'.');
        if (psz != NULL)
        {
            *psz = L'\0';
        }
    }
    else
    {
        g_pszProgName = MODNAME;
    }
    (*picArgs)--;
    (*papszArgs)++;

    while ((rc == 0) &&
           (*picArgs > 0) &&
           (((*papszArgs)[0][0] == L'/') || ((*papszArgs)[0][0] == L'-')))
    {
        if (_wcsicmp(&(*papszArgs)[0][1], L"?") == 0)
        {
            PrintUsage();
            rc = 1;
        }
        else if (_wcsicmp(&(*papszArgs)[0][1], L"t") == 0)
        {
            g_dwfSudoku |= SF_TRACE;
        }
        else if (_wcsicmp(&(*papszArgs)[0][1], L"v") == 0)
        {
            g_dwfSudoku |= SF_VERBOSE;
        }
        else if (_wcsnicmp(&(*papszArgs)[0][1], L"r=", 2) == 0)
        {
            int dim = _wtoi(&(*papszArgs)[0][3]);

            if ((dim < 2) || (dim > MAX_REGION_DIM))
            {
                wprintf(L"%s: invalid region dimension %d, must be between %d and %d.\n",
                        g_pszProgName, dim, 2, MAX_REGION_DIM);
                rc = -1;
            }
            else
            {
                g_dimRegion = dim;
                g_dimBoard = dim*dim;
            }
        }
        else
        {
            rc = -1;
            wprintf(L"%s: Invalid command option <%s>.\n",
                    g_pszProgName, (*papszArgs)[0]);
            PrintUsage();
        }
        (*picArgs)--;
        (*papszArgs)++;
    }

    TExitDec(MAIN, WPPIndent, rc);
    return rc;
}   //ParseArguments

/**
 *  This function process a game file.
 *
 *  @param pszFile Points to the game file name.
 *  @param dimRegion Specifies the region dimension.
 *
 *  @return Success: Returns 0.
 *  @return Failure: Returns -1.
 */
int
ProcessFile(
    __in_opt LPWSTR pszFile,
    __in     int dimRegion
    )
{
    int rc = 0;
    FILE *pfile = stdin;
    int dimBoard = dimRegion*dimRegion;

    TEnter(MAIN, WPPIndent, "File=%ws", pszFile? pszFile: L"stdin");

    if (pszFile != NULL)
    {
        if (_wfopen_s(&pfile, pszFile, L"r") != 0)
        {
            wprintf(L"%s: failed to open <%s>.\n", g_pszProgName, pszFile);
            rc = -1;
        }
    }

    if (rc == 0)
    {
        Board *board = new Board(dimRegion);

        if (board == NULL)
        {
            wprintf(L"%s: failed to allocate %dx%d board.",
                    g_pszProgName, dimBoard, dimBoard);
            rc = -1;
        }
        else
        {
            wprintf(L"\nFile: %s\n", pszFile? pszFile: L"stdin");
            rc = ReadBoard(pfile, board, dimBoard);
            if (rc == 0)
            {
                int cntInitCells, cntSolvedCells;
                BOOL fProgress;

                cntInitCells = board->GetInitCellCount();
                for (int i = 1; ; i++)
                {
                    board->ClearProgress();
                    board->ProcessBoard(g_RuleTable, ARRAYSIZE(g_RuleTable));
                    cntSolvedCells = cntInitCells + board->GetSolvedCellCount();
                    fProgress = board->GetProgress();

                    if (g_dwfSudoku & SF_TRACE)
                    {
                        PrintGameInfo(board, i);
                    }

                    if (!fProgress || (cntSolvedCells == dimBoard*dimBoard))
                    {
                        if (!(g_dwfSudoku & SF_TRACE))
                        {
                            //
                            // Print final summary for non-verbose mode.
                            //
                            PrintGameInfo(board, i);
                        }

                        if (!fProgress)
                        {
                            wprintf(L"Failed to make any more progress!\n");
                        }
                        break;
                    }
                }
            }
            delete board;
        }
    }

    if ((pfile != NULL) && (pfile != stdin))
    {
        fclose(pfile);
    }

    TExitDec(MAIN, WPPIndent, rc);
    return rc;
}   //ProcessFile

/**
 *  This function reads the initial board data either from a file or stdin.
 *
 *  @param pfile Specifies the file handle to read the data from.
 *  @param board Points to the board object.
 *  @param dimBoard Specifies the board dimension.
 *
 *  @return Success: Returns 0.
 *  @return Failure: Returns -1.
 */
int
ReadBoard(
    __in FILE *pfile,
    __in Board *board,
    __in int dimBoard
    )
{
    int rc = 0;
    char szLine[256];

    TEnter(MAIN, WPPIndent, "pfile=%p,board=%p,dimBoard=%d",
           pfile, board, dimBoard);

    for (int r = 0; (rc == 0) && (r < dimBoard); r++)
    {
        if (fgets(szLine, ARRAYSIZE(szLine), pfile) == NULL)
        {
            wprintf(L"%s: failed to read row %d.\n", g_pszProgName, r + 1);
            rc = -1;
            break;
        }
        else
        {
            int c;
            LPSTR psz;
            LPSTR pszContext = NULL;
            int value;

            for (c = 0, psz = strtok_s(szLine, " \t,", &pszContext);
                 (rc == 0) && (psz != NULL);
                 psz = strtok_s(NULL, " \t,", &pszContext), c++)
            {
                value = atoi(psz);
                if ((value < 0) || (value > dimBoard))
                {
                    wprintf(L"%s: invalid value [%d,%d]=%d.\n",
                            g_pszProgName, r + 1, c + 1, value);
                    rc = -1;
                    break;
                }
                else if (c < dimBoard)
                {
                    board->SetCellValue(r, c, value, 0);
                }
                else
                {
                    wprintf(L"%s: too much data in row %d.\n",
                            g_pszProgName, r + 1);
                    rc = -1;
                    break;
                }
            }

            if (c < dimBoard)
            {
                wprintf(L"%s: insufficient data in row %d.\n",
                        g_pszProgName, r + 1);
                rc = -1;
            }
        }
    }

    if (g_dwfSudoku & SF_TRACE)
    {
        PrintGameInfo(board, 0);
    }

    TExitDec(MAIN, WPPIndent, rc);
    return rc;
}   //ReadBoard

/**
 *  This function prints the title banner.
 */
void
PrintBanner(
    void
    )
{
    TEnterVoid(Util, WPPIndent);

    wprintf(L"\n%s v%02d.%02d.%02d\n%s\n\n",
            STR_PROGTITLE, VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD,
            STR_COPYRIGHT);

    TExitVoid(Util, WPPIndent);
    return;
}   //PrintBanner

/**
 *  This function prints the usage help message.
 */
void
PrintUsage(
    void
    )
{
    TEnterVoid(Util, WPPIndent);

    PrintBanner();
    wprintf(L"Usage:\t%s /?\n"
            L"\t%s [/t] [/v] [/r=<n>] [<GameDataFiles>]\n\n"
            L"\t?               - Print this help message.\n"
            L"\tt               - Trace mode.\n"
            L"\tv               - Verbose mode.\n"
            L"\tr=<n>           - Specifies the region dimension (2 <= n <= 8).\n"
            L"\t                  If absent, default is 3.\n"
            L"\t<GameDataFiles> - Specifies the files containing game data.\n"
            L"\t                  If absent, stdin is used.\n",
            g_pszProgName, g_pszProgName);

    TExitVoid(Util, WPPIndent);
    return;
}   //PrintUsage

/**
 *  This function prints the game info.
 *
 *  @param board Points to the board object.
 *  @param iteration Specifies the game iteration.
 */
void
PrintGameInfo(
    __in Board *board,
    __in int iteration
    )
{
    TEnter(MAIN, WPPIndent, "board=%p,iteration=%d", board, iteration);

    wprintf(L"\n[Iteration: %d]\n", iteration);
    board->PrintBoard();
    board->PrintSummary();
    for (int i = 0; i < ARRAYSIZE(g_RuleTable); i++)
    {
        if (g_RuleTable[i] != NULL)
        {
            g_RuleTable[i]->PrintSummary(board);
        }
    }
    wprintf(L"\n");

    TExitVoid(MAIN, WPPIndent);
    return;
}   //PrintGameInfo

