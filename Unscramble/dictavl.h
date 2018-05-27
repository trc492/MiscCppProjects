#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="DictAVL.h" />
///
/// <summary>
///     This module contains the definitions of the AVLDictionary class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 26-July-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class AVLDictionary: public Dictionary, public NodeData
{
private:
    AVLTree    *m_AVLDict;

public:
    virtual
    void
    ProcessData(
        __in int level,
        __in AVLNode *node,
        __in PVOID context
        );

    virtual
    void
    DeleteData(
        __in AVLNode *node
        );

    virtual
    void
    PrintData(
        __in int level,
        __in AVLNode *node
        );

    virtual
    int
    CompareData(
        __in_bcount(lenData1) PVOID buffData,
        __in                  size_t lenData,
        __in                  AVLNode *node
        );

    virtual
    LPSTR
    GetDataString(
        __in_bcount(lenData) PVOID buffData,
        __in                 size_t lenData
        );

    virtual
    LPSTR
    GetDataString(
        __in AVLNode *node
        );

public:
    AVLDictionary(
        __in LPCSTR pszFile
        );

    ~AVLDictionary(
        void
        );

    virtual
    DWORD
    LookupWord(
        __in LPCSTR pszWord,
        __in BOOL fTrace
        );

    void
    PrintDictTree(
        void
        );
};

