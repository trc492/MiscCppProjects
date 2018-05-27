#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="AVLNode.cpp" />
///
/// <summary>
///     This module contains the implementation of the AVLNode class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 17-July-2010
///     Environment: User mode
/// </remarks>
#endif

#include "pch.h"
#ifdef _USE_WPP_TRACING
#include "avlnode.tmh"
#endif

/**
 *  Constructor to create an instance of the object.
 */
AVLNode::AVLNode(
    void
    ): m_childLeft(NULL),
       m_childRight(NULL),
       m_height(0),
       m_buffData(NULL),
       m_lenData(0)
{
    TEnterVoid(AVLNODE, WPPIndent);
    TExitVoid(AVLNODE, WPPIndent);
    return;
}   //AVLNode

/**
 *  Constructor to create an instance of the object.
 *
 *  @param buffData Points to the data buffer for the tree node.
 *  @param lenData Specifies the length of the data buffer.
 */
AVLNode::AVLNode(
    __in_bcount(lenData) PVOID buffData,
    __in                 size_t lenData
    ): m_childLeft(NULL),
       m_childRight(NULL),
       m_height(0),
       m_buffData(buffData),
       m_lenData(lenData)
{
    TEnter(AVLNODE, WPPIndent, "buff=%p,len=%Id", buffData, lenData);
    TExitVoid(AVLNODE, WPPIndent);
    return;
}   //AVLNode

/**
 *  Destructor to destroy an instance of the object.
 *  Note: It is the caller's responsibility to free the node data before
 *  destroying the node object.
 */
AVLNode::~AVLNode(
    void
    )
{
    TEnterVoid(AVLNODE, WPPIndent);
    TExitVoid(AVLNODE, WPPIndent);
    return;
}   //~AVLNode

/**
 *  This function returns the left child node.
 *
 *  @return Returns the left child node.
 */
AVLNode *
AVLNode::GetLeftChild(
    void
    )
{
    TEnterVoid(AVLNODE, WPPIndent);
    TExitPtr(AVLNODE, WPPIndent, m_childLeft);
    return m_childLeft;
}   //GetLeftChild

/**
 *  This function sets the left child of the tree node and returns the
 *  previous child if any.
 *
 *  @param child Points to the node to become the new left child.
 *
 *  @return Returns the previous child if any.
 */
AVLNode *
AVLNode::SetLeftChild(
    __in_opt AVLNode *child
    )
{
    AVLNode *childPrev = m_childLeft;

    TEnter(AVLNODE, WPPIndent, "child=%p", child);

    m_childLeft = child;

    TExitPtr(AVLNODE, WPPIndent, childPrev);
    return childPrev;
}   //SetLeftChild

/**
 *  This function returns the right child node.
 *
 *  @return Returns the right child node.
 */
AVLNode *
AVLNode::GetRightChild(
    void
    )
{
    TEnterVoid(AVLNODE, WPPIndent);
    TExitPtr(AVLNODE, WPPIndent, m_childRight);
    return m_childRight;
}   //GetRightChild

/**
 *  This function sets the right child of the tree node and returns the
 *  previous child if any.
 *
 *  @param child Points to the node to become the new right child.
 *
 *  @return Returns the previous child if any.
 */
AVLNode *
AVLNode::SetRightChild(
    __in_opt AVLNode *child
    )
{
    AVLNode *childPrev = m_childRight;

    TEnter(AVLNODE, WPPIndent, "child=%p", child);

    m_childRight = child;

    TExitPtr(AVLNODE, WPPIndent, childPrev);
    return childPrev;
}   //SetRightChild

/**
 *  This function returns the height of the  node in the tree.
 *
 *  @return Returns the height of the tree.
 */
int
AVLNode::GetHeight(
    void
    )
{
    TEnterVoid(AVLNODE, WPPIndent);
    TExitDec(AVLNODE, WPPIndent, m_height);
    return m_height;
}   //GetHeight

/**
 *  This function sets the height of the node in the tree and returns the
 *  previous height.
 *
 *  @param height Specifies the new height.
 *
 *  @return Returns the previous height.
 */
int
AVLNode::SetHeight(
    __in int height
    )
{
    int heightPrev = m_height;

    TEnter(AVLNODE, WPPIndent, "height=%d", height);

    m_height = height;

    TExitDec(AVLNODE, WPPIndent, heightPrev);
    return heightPrev;
}   //SetHeight

/**
 *  This function returns the data of the tree node.
 *
 *  @param plenData Optionally points to the variable to hold the data length.
 *
 *  @return Returns the pointer to the data buffer.
 */
PVOID
AVLNode::GetData(
    __out_opt size_t *plenData
    )
{
    TEnter(AVLNODE, WPPIndent, "plenData=%p", plenData);

    if (plenData != NULL)
    {
        *plenData = m_lenData;
    }

    TExitPtrMsg(AVLNODE, WPPIndent, m_buffData, "(len=%Id)", m_lenData);
    return m_buffData;
}   //GetData

/**
 *  This function sets the data of the tree node and returns the previous
 *  data if any.
 *  Note: It is the responsibility of the caller to free the previous data
 *        if necessary.
 *
 *  @param buffData Points to the data buffer.
 *  @param lenData Specifies the length of the data.
 *
 *  @return Returns the previous data if any.
 */
PVOID
AVLNode::SetData(
    __in_bcount_opt(lenData) PVOID buffData,
    __in                     size_t lenData
    )
{
    PVOID dataPrev = m_buffData;

    TEnter(AVLNODE, WPPIndent, "buffData=%p,lenData=%Id", buffData, lenData);

    m_buffData = buffData;
    m_lenData = lenData;

    TExitPtr(AVLNODE, WPPIndent, dataPrev);
    return dataPrev;
}   //SetData


