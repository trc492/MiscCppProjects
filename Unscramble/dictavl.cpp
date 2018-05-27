#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="DictAVL.cpp" />
///
/// <summary>
///     This module contains the implementation of the AVLDictionary class.
///     The AVLDictionary class reads a sorted dictionary text file and
///     builds a balanced AVL trees so that we can do binary tree search
///     on the given word.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 26-July-2010
///     Environment: User mode
/// </remarks>
#endif

#include "pch.h"
#ifdef _USE_WPP_TRACING
#include "dictavl.tmh"
#endif

/**
 *  This method is called to process the data of a given tree node.
 *
 *  @param level Specifies the depth of the tree node.
 *  @param node Points to the tree node to be process.
 *  @param context Specifies the callback context.
 *
 *  @return None.
 */
void
AVLDictionary::ProcessData(
    __in int level,
    __in AVLNode *node,
    __in PVOID context
    )
{
    TEnter(DICT, WPPIndent, "level=%d,node=%p,context=%p",
           level, node, context);

    UNREFERENCED_PARAMETER(level);
    UNREFERENCED_PARAMETER(node);
    UNREFERENCED_PARAMETER(context);

    TExitVoid(DICT, WPPIndent);
    return;
}   //ProcessData

/**
 *  This method is called to delete the node data if necessary.
 *
 *  @param node Points to the tree node to be process.
 *
 *  @return None.
 */
void
AVLDictionary::DeleteData(
    __in AVLNode *node
    )
{
    PVOID buffData = node->SetData(NULL, 0);

    TEnter(DICT, WPPIndent, "node=%p", node);

    if (buffData != NULL)
    {
        delete [] (BYTE *)buffData;
    }

    TExitVoid(DICT, WPPIndent);
    return;
}   //DeleteData

/**
 *  This method is called to print the node data.
 *
 *  @param level Specifies the depth of the tree node.
 *  @param node Points to the tree node to be process.
 *
 *  @return None.
 */
void
AVLDictionary::PrintData(
    __in int level,
    __in AVLNode *node
    )
{
    LPSTR pszWord = (LPSTR)node->GetData(NULL);

    TEnter(DICT, WPPIndent, "level=%d,node=%p", level, node);

    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }
    printf("+<%s>\n", pszWord);

    TExitVoid(DICT, WPPIndent);
    return;
}   //PrintData

/**
 *  This method is called to compare the data with the data of the
 *  tree node.
 *
 *  @param buffData Points to the data buffer.
 *  @param lenData Specifies the length of the data buffer.
 *  @param node Points to the tree node for the comparison.
 *
 *  @return Returns negative number if buffData is smaller than node data.
 *          Returns zero if buffData is equal to node data.
 *          Returns positive number if buffData is greater than node data.
 */
int
AVLDictionary::CompareData(
    __in_bcount(lenData1) PVOID buffData,
    __in                  size_t lenData,
    __in                  AVLNode *node
    )
{
    int result;
    LPSTR pszWord = (LPSTR)node->GetData(NULL);

    TEnter(DICT, WPPIndent, "buffData=%p,len=%Id,node=%p",
           buffData, lenData, node);

    UNREFERENCED_PARAMETER(lenData);
    result = _stricmp((LPSTR)buffData, pszWord);

    TExitDec(DICT, WPPIndent, result);
    return result;
}   //CompareData

/**
 *  This method is called to convert the data into string form from a given
 *  data buffer.
 *
 *  @param buffData Points to the data buffer.
 *  @param lenData Specifies the length of the data buffer.
 *
 *  @return Success: Returns the string data.
 *          Failure: Returns NULL.
 */
LPSTR
AVLDictionary::GetDataString(
    __in_bcount(lenData1) PVOID buffData,
    __in                  size_t lenData
    )
{
    LPSTR pszData = (LPSTR)buffData;

    TEnter(DICT, WPPIndent, "buffData=%p,len=%Id", buffData, lenData);
    UNREFERENCED_PARAMETER(lenData);
    TExitStr(DICT, WPPIndent, pszData);
    return pszData;
}   //GetDataString

/**
 *  This method is called to convert the data into string form from a given
 *  tree node.
 *
 *  @param node Points to the tree node.
 *
 *  @return Success: Returns the string data.
 *          Failure: Returns NULL.
 */
LPSTR
AVLDictionary::GetDataString(
    __in AVLNode *node
    )
{
    LPSTR pszData = (LPSTR)node->GetData(NULL);

    TEnter(DICT, WPPIndent, "node=%p", node);
    TExitStr(DICT, WPPIndent, pszData);
    return pszData;
}   //GetDataString

/**
 *  Constructor to create an instance of the object.
 *  It opens the dictionary file and throws an error if it failed.
 *
 *  @param pszFile Points to the dictionary file name string.
 */
AVLDictionary::AVLDictionary(
    __in LPCSTR pszFile
    ): m_AVLDict(NULL)
{
    DWORD dwErr = ERROR_SUCCESS;
    FILE *pfileDict = NULL;

    TEnter(DICT, WPPIndent, "File=%s", pszFile);

    if ((m_AVLDict = new AVLTree(this)) == NULL)
    {
        TMsg(Err, "Failed to create AVLTree object.");
        dwErr = ERROR_OUTOFMEMORY;
    }
    else if (fopen_s(&pfileDict, pszFile, "r") != 0)
    {
        dwErr = GetLastError();
        TMsgErr(Err, dwErr, "Failed to open dictionary file <%s> (err=%d).",
                pszFile, dwErr);
    }
    else
    {
        CHAR szLine[256];
        LPSTR pszSep = " \t\n";
        LPSTR pszToken;
        LPSTR pszContext;
        size_t lenData;
        LPSTR pszWord;

        while ((dwErr == ERROR_SUCCESS) &&
               (fgets(szLine, ARRAYSIZE(szLine), pfileDict) != NULL))
        {
            _strlwr_s(szLine, ARRAYSIZE(szLine));
            pszToken = strtok_s(szLine, pszSep, &pszContext);
            while ((dwErr == ERROR_SUCCESS) && (pszToken != NULL))
            {
                lenData = strlen(pszToken) + 1;
                pszWord = (LPSTR) new CHAR[lenData];
                if (pszWord == NULL)
                {
                    TMsg(Err, "Failed to allocate node data (len=%Id).",
                         lenData);
                    dwErr = ERROR_OUTOFMEMORY;
                }
                else
                {
                    strcpy_s(pszWord, lenData, pszToken);
                    m_AVLDict->Insert((PVOID)pszWord, lenData);
                    pszToken = strtok_s(NULL, pszSep, &pszContext);
                }
            }
        }
        fclose(pfileDict);
    }

    if (dwErr != ERROR_SUCCESS)
    {
        throw dwErr;
    }

    TExitVoid(DICT, WPPIndent);
    return;
}   //AVLDictionary

/**
 *  Destructor to destroy an instance of the object.
 *  It closes the dictionary file.
 */
AVLDictionary::~AVLDictionary(
    void
    )
{
    TEnterVoid(DICT, WPPIndent);

    if (m_AVLDict != NULL)
    {
        delete m_AVLDict;
        m_AVLDict = NULL;
    }

    TExitVoid(DICT, WPPIndent);
}   //~AVLDictionary

/**
 *  This method looks up the given word in the AVL dictionary tree using
 *  binary search.
 *
 *  @param pszWord Points to the word to look up.
 *  @param fTrace If TRUE, turn tracing ON, FALSE otherwise.
 *
 *  @return Success: Returns ERROR_SUCCESS.
 *  @return Failure: Returns Win32 error code.
 */
DWORD
AVLDictionary::LookupWord(
    __in LPCSTR pszWord,
    __in BOOL fTrace
    )
{
    DWORD rc = ERROR_NOT_FOUND;
    size_t lenWord = strlen(pszWord);

    TEnter(DICT, WPPIndent, "Word=%s,fTrace=%d", pszWord, fTrace);

    TAssert(m_AVLDict != NULL);
    if (m_AVLDict->Find((PVOID)pszWord, lenWord + 1, fTrace) != NULL)
    {
        rc = ERROR_SUCCESS;
    }

    TExitErr(DICT, WPPIndent, rc);
    return rc;
}   //LookupWord

/**
 *  This method prints the entire dictionary tree to the console.
 *
 *  @return None.
 */
void
AVLDictionary::PrintDictTree(
    void
    )
{
    TEnterVoid(DICT, WPPIndent);

    TAssert(m_AVLDict != NULL);
    m_AVLDict->PrintTree();

    TExitVoid(DICT, WPPIndent);
    return;
}   //PrintDictTree

