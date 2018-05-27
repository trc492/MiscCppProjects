#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="AVLNode.h" />
///
/// <summary>
///     This module contains the definitions of the AVLNode class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 17-July-2010
///     Environment: User mode.
/// </remarks>
#endif

#pragma once

class AVLNode
{
private:
    AVLNode    *m_childLeft;
    AVLNode    *m_childRight;
    int         m_height;
    PVOID       m_buffData;
    size_t      m_lenData;

public:
    AVLNode(
        void
        );

    AVLNode(
        __in_bcount(lenData) PVOID buffData,
        __in                 size_t lenData
        );

    ~AVLNode(
        void
        );

    AVLNode *
    GetLeftChild(
        void
        );

    AVLNode *
    SetLeftChild(
        __in_opt AVLNode *child
        );

    AVLNode *
    GetRightChild(
        void
        );

    AVLNode *
    SetRightChild(
        __in_opt AVLNode *child
        );

    int
    GetHeight(
        void
        );

    int
    SetHeight(
        __in int height
        );

    PVOID
    GetData(
        __out_opt size_t *plenData
        );

    PVOID
    SetData(
        __in_bcount_opt(lenData) PVOID buffData,
        __in                     size_t lenData
        );
};

