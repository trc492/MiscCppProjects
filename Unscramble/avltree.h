#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="AVLTree.h" />
///
/// <summary>
///     This module contains the definitions of the AVLTree class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 17-July-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class NodeData
{
public:
    virtual
    void
    ProcessData(
        __in int level,
        __in AVLNode *node,
        __in PVOID context
        ) = 0;

    virtual
    void
    DeleteData(
        __in AVLNode *node
        ) = 0;

    virtual
    void
    PrintData(
        __in int level,
        __in AVLNode *node
        ) = 0;

    virtual
    int
    CompareData(
        __in_bcount(lenData1) PVOID buffData,
        __in                  size_t lenData,
        __in                  AVLNode *node
        ) = 0;

    virtual
    LPSTR
    GetDataString(
        __in_bcount(lenData) PVOID buffData,
        __in                 size_t lenData
        ) = 0;

    virtual
    LPSTR
    GetDataString(
        __in AVLNode *node
        ) = 0;
};

class AVLTree
{
public:
    typedef enum _WalkOrder
    {
        InOrder,
        PreOrder,
        PostOrder
    } WalkOrder;

    typedef enum _Operation
    {
        OpProcessNode,
        OpDeleteNode,
        OpPrintNode
    } Operation;

private:
    NodeData   *m_NodeData;
    AVLNode    *m_nodeRoot;
    DWORD       m_numNodes;

    void
    ExecuteOperation(
        __in     Operation op,
        __in_opt PVOID context,
        __in     int level,
        __in     AVLNode *node
        );

    AVLNode *
    SingleRotateWithLeft(
        __in AVLNode *nodeRoot
        );

    AVLNode *
    SingleRotateWithRight(
        __in AVLNode *nodeRoot
        );

    AVLNode *
    DoubleRotateWithLeft(
        __in AVLNode *nodeRoot
        );

    AVLNode *
    DoubleRotateWithRight(
        __in AVLNode *nodeRoot
        );

public:
    AVLTree(
        __in NodeData *nodedata
        );

    ~AVLTree(
        void
        );

    AVLNode *
    GetRoot(
        void
        );

    DWORD
    GetNumNodes(
        void
        );

    int
    GetHeight(
        __in_opt AVLNode *node
        );

    int
    GetHeight(
        void
        );

    void
    WalkTree(
        __in     int level,
        __in     WalkOrder order,
        __in     Operation op,
        __in_opt PVOID context,
        __in_opt AVLNode *nodeRoot
        );

    void
    WalkTree(
        __in     int level,
        __in     WalkOrder order,
        __in     Operation op,
        __in_opt PVOID context
        );

    AVLNode *
    Find(
        __in_bcount(lenData) PVOID buffData,
        __in                 size_t lenData,
        __in                 BOOL fTrace,
        __in_opt             AVLNode *nodeRoot
        );

    AVLNode *
    Find(
        __in_bcount(lenData) PVOID buffData,
        __in                 size_t lenData,
        __in                 BOOL fTrace
        );

    AVLNode *
    Insert(
        __in_bcount(lenData) PVOID buffData,
        __in                 size_t lenData,
        __in_opt             AVLNode *nodeRoot
        );

    AVLNode *
    Insert(
        __in_bcount(lenData) PVOID buffData,
        __in                 size_t lenData
        );

    void
    DeleteTree(
        __in_opt AVLNode *nodeRoot
        );

    void
    DeleteTree(
        void
        );

    void
    PrintTree(
        __in_opt AVLNode *nodeRoot
        );

    void
    PrintTree(
        void
        );
};

