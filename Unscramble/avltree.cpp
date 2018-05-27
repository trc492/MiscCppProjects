#if 0
/// Copyright (c) WisdomSoft. All rights reserved.
///
/// <module name="AVLTree.cpp" />
///
/// <summary>
///     This module contains the implementation of the AVLTree class.
/// </summary>
///
/// <remarks>
///     Author: Michael Tsang 17-July-2010
///     Environment: User mode
/// </remarks>
#endif

#include "pch.h"
#ifdef _USE_WPP_TRACING
#include "avltree.tmh"
#endif

/**
 *  This function executes the specifies operation on a given tree node.
 *
 *  @param op Specifies the operation to be done to the node visited.
 *  @param context Points to the callback context if any.
 *  @param level Specifies the level of the node in the tree.
 *  @param node Points to the root node of the tree to be walked.
 *
 *  @return None.
 */
void
AVLTree::ExecuteOperation(
    __in     Operation op,
    __in_opt PVOID context,
    __in     int level,
    __in     AVLNode *node
    )
{
    TEnter(AVLTREE, WPPIndent, "op=%d,context=%p,level=%d,node=%p",
           op, context, level, node);

    TAssert(m_NodeData != NULL);
    switch (op)
    {
    case OpProcessNode:
        m_NodeData->ProcessData(level, node, context);
        break;

    case OpDeleteNode:
        m_NodeData->DeleteData(node);
        node->SetLeftChild(NULL);
        node->SetRightChild(NULL);
        delete node;
        break;

    case OpPrintNode:
        m_NodeData->PrintData(level, node);
        break;
    }

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //ExecuteOperation

/**
 *  This function does a single rotation with the left child to balance the
 *  AVL tree.
 *
 *  @param nodeRoot Points to the root node of the tree.
 *
 *  @return Returns the new root node.
 */
AVLNode *
AVLTree::SingleRotateWithLeft(
    __in AVLNode *nodeRoot
    )
{
    AVLNode *nodeNewRoot;

    TEnter(AVLTREE, WPPIndent, "Root=%p", nodeRoot);

    nodeNewRoot = nodeRoot->GetLeftChild();
    nodeRoot->SetLeftChild(nodeNewRoot->GetRightChild());
    nodeNewRoot->SetRightChild(nodeRoot);
    nodeRoot->SetHeight(max(GetHeight(nodeRoot->GetLeftChild()),
                            GetHeight(nodeRoot->GetRightChild())) + 1);
    nodeNewRoot->SetHeight(max(GetHeight(nodeNewRoot->GetLeftChild()),
                               GetHeight(nodeNewRoot->GetRightChild())) +
                           1);

    TExitPtr(AVLTREE, WPPIndent, nodeNewRoot);
    return nodeNewRoot;
}   //SingleRotateWithLeft

/**
 *  This function does a single rotation with the right child to balance the
 *  AVL tree.
 *
 *  @param nodeRoot Points to the root node of the tree.
 *
 *  @return Returns the new root node.
 */
AVLNode *
AVLTree::SingleRotateWithRight(
    __in AVLNode *nodeRoot
    )
{
    AVLNode *nodeNewRoot;

    TEnter(AVLTREE, WPPIndent, "Root=%p", nodeRoot);

    nodeNewRoot = nodeRoot->GetRightChild();
    nodeRoot->SetRightChild(nodeNewRoot->GetLeftChild());
    nodeNewRoot->SetLeftChild(nodeRoot);
    nodeRoot->SetHeight(max(GetHeight(nodeRoot->GetLeftChild()),
                            GetHeight(nodeRoot->GetRightChild())) + 1);
    nodeNewRoot->SetHeight(max(GetHeight(nodeNewRoot->GetLeftChild()),
                               GetHeight(nodeNewRoot->GetRightChild())) +
                           1);

    TExitPtr(AVLTREE, WPPIndent, nodeNewRoot);
    return nodeNewRoot;
}   //SingleRotateWithRight

/**
 *  This function does a double rotation with the left child to balance the
 *  AVL tree.
 *
 *  @param nodeRoot Points to the root node of the tree.
 *
 *  @return Returns the new root node.
 */
AVLNode *
AVLTree::DoubleRotateWithLeft(
    __in AVLNode *nodeRoot
    )
{
    AVLNode *nodeNewRoot;

    TEnter(AVLTREE, WPPIndent, "Root=%p", nodeRoot);

    nodeRoot->SetLeftChild(SingleRotateWithRight(nodeRoot->GetLeftChild()));
    nodeNewRoot = SingleRotateWithLeft(nodeRoot);

    TExitPtr(AVLTREE, WPPIndent, nodeNewRoot);
    return nodeNewRoot;
}   //DoubleRotateWithLeft

/**
 *  This function does a double rotation with the right child to balance the
 *  AVL tree.
 *
 *  @param nodeRoot Points to the root node of the tree.
 *
 *  @return Returns the new root node.
 */
AVLNode *
AVLTree::DoubleRotateWithRight(
    __in AVLNode *nodeRoot
    )
{
    AVLNode *nodeNewRoot;

    TEnter(AVLTREE, WPPIndent, "Root=%p", nodeRoot);

    nodeRoot->SetRightChild(SingleRotateWithLeft(nodeRoot->GetRightChild()));
    nodeNewRoot = SingleRotateWithRight(nodeRoot);

    TExitPtr(AVLTREE, WPPIndent, nodeNewRoot);
    return nodeNewRoot;
}   //DoubleRotateWithRight

/**
 *  Constructor to create an instance of the object.
 *
 *  @param nodedata Points to the NodeData callback object to process the
 *         data.
 */
AVLTree::AVLTree(
    __in NodeData *nodedata
    ): m_NodeData(nodedata),
       m_nodeRoot(NULL),
       m_numNodes(0)
{
    TEnterVoid(AVLTREE, WPPIndent);
    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //AVLTree

/**
 *  Destructor to destroy an instance of the object.
 */
AVLTree::~AVLTree(
    void
    )
{
    TEnterVoid(AVLTREE, WPPIndent);

    DeleteTree(m_nodeRoot);
    m_nodeRoot = NULL;

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //~AVLTree

/**
 *  This function returns the root node of the tree.
 *
 *  @return Success: Returns the root node of the tree.
 *  @return Failure: Returns NULL.
 */
AVLNode *
AVLTree::GetRoot(
    void
    )
{
    TEnterVoid(AVLTREE, WPPIndent);
    TExitPtr(AVLTREE, WPPIndent, m_nodeRoot);
    return m_nodeRoot;
}   //GetRoot

/**
 *  This function returns the total number of nodes in the tree.
 *
 *  @return Success: Returns the total number of nodes in the tree.
 */
DWORD
AVLTree::GetNumNodes(
    void
    )
{
    TEnterVoid(AVLTREE, WPPIndent);
    TExitDec(AVLTREE, WPPIndent, m_numNodes);
    return m_numNodes;
}   //GetNumNodes

/**
 *  This function returns the height of the tree node.
 *
 *  @param node Points to the tree node.
 *
 *  @return Success: Returns the height of the tree node.
 *  @return Failure: Returns -1.
 */
int
AVLTree::GetHeight(
    __in_opt AVLNode *node
    )
{
    int height = -1;

    TEnter(AVLTREE, WPPIndent, "Node=%p", node);

    if (node != NULL)
    {
        height = node->GetHeight();
    }

    TExitDec(AVLTREE, WPPIndent, height);
    return height;
}   //GetHeight

/**
 *  This function returns the height of the tree.
 *
 *  @return Success: Returns the height of the tree.
 *  @return Failure: Returns -1.
 */
int
AVLTree::GetHeight(
    void
    )
{
    int height;

    TEnterVoid(AVLTREE, WPPIndent);

    height = GetHeight(m_nodeRoot);

    TExitDec(AVLTREE, WPPIndent, height);
    return height;
}   //GetHeight

/**
 *  This function walks the tree recursively.
 *
 *  @param level Specifies the level of the node in the tree.
 *  @param order Specifies the walking order.
 *  @param op Specifies the operation to be done to the node visited.
 *  @param context Points to the callback context if any.
 *  @param nodeRoot Points to the root node of the tree to be walked.
 *
 *  @return None.
 */
void
AVLTree::WalkTree(
    __in     int level,
    __in     WalkOrder order,
    __in     Operation op,
    __in_opt PVOID context,
    __in_opt AVLNode *nodeRoot
    )
{
    TEnter(AVLTREE, WPPIndent, "level=%d,order=%d,op=%d,context=%p,root=%p",
           level, order, op, context, nodeRoot);

    if (nodeRoot != NULL)
    {
        AVLNode *node;

        if (order == PreOrder)
        {
            ExecuteOperation(op, context, level, nodeRoot);
        }

        node = nodeRoot->GetLeftChild();
        if (node != NULL)
        {
            WalkTree(level + 1, order, op, context, node);
        }

        if (order == InOrder)
        {
            ExecuteOperation(op, context, level, nodeRoot);
        }

        node = nodeRoot->GetRightChild();
        if (node != NULL)
        {
            WalkTree(level + 1, order, op, context, node);
        }

        if (order == PostOrder)
        {
            ExecuteOperation(op, context, level, nodeRoot);
        }
    }

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //WalkTree

/**
 *  This function walks the tree recursively.
 *
 *  @param level Specifies the level of the node in the tree.
 *  @param order Specifies the walking order.
 *  @param op Specifies the operation to be done to the node visited.
 *  @param context Points to the callback context if any.
 *
 *  @return None.
 */
void
AVLTree::WalkTree(
    __in     int level,
    __in     WalkOrder order,
    __in     Operation op,
    __in_opt PVOID context
    )
{
    TEnter(AVLTREE, WPPIndent, "level=%d,order=%d,op=%d,context=%p",
           level, order, op, context);

    WalkTree(level, order, op, context, m_nodeRoot);

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //WalkTree

/**
 *  This function finds the given data in the tree.
 *
 *  @param buffData Points to the data buffer.
 *  @param lenData Specifies the length of the data buffer.
 *  @param fTrace If TRUE, turn on tracing.
 *  @param nodeRoot Points to the root node of the tree.
 *
 *  @return Success: Returns the tree node that matches the data.
 *  @return Failure: Returns NULL.
 */
AVLNode *
AVLTree::Find(
    __in_bcount(lenData) PVOID buffData,
    __in                 size_t lenData,
    __in                 BOOL fTrace,
    __in_opt             AVLNode *nodeRoot
    )
{
    AVLNode *node = NULL;

    TEnter(AVLTREE, WPPIndent, "buff=%p,len=%Id,fTrace=%d,root=%p",
           buffData, lenData, fTrace, nodeRoot);

    TAssert(m_NodeData != NULL);
    if (nodeRoot != NULL)
    {
        int result = m_NodeData->CompareData(buffData, lenData, nodeRoot);

        if (fTrace)
        {
            printf("Comparing <%s> to <%s> = %d\n",
                   m_NodeData->GetDataString(buffData, lenData),
                   m_NodeData->GetDataString(nodeRoot),
                   result);
        }

        if (result < 0)
        {
            node = Find(buffData, lenData, fTrace, nodeRoot->GetLeftChild());
        }
        else if (result > 0)
        {
            node = Find(buffData, lenData, fTrace, nodeRoot->GetRightChild());
        }
        else
        {
            node = nodeRoot;
        }
    }

    TExitPtr(AVLTREE, WPPIndent, node);
    return node;
}   //Find

/**
 *  This function finds the given data in the tree.
 *
 *  @param buffData Points to the data buffer.
 *  @param lenData Specifies the length of the data buffer.
 *  @param fTrace If TRUE, turn on tracing.
 *
 *  @return Success: Returns the tree node that matches the data.
 *  @return Failure: Returns NULL.
 */
AVLNode *
AVLTree::Find(
    __in_bcount(lenData) PVOID buffData,
    __in                 size_t lenData,
    __in                 BOOL fTrace = FALSE
    )
{
    AVLNode *node;

    TEnter(AVLTREE, WPPIndent, "buff=%p,len=%Id,fTrace=%d",
           buffData, lenData, fTrace);

    node = Find(buffData, lenData, fTrace, m_nodeRoot);

    TExitPtr(AVLTREE, WPPIndent, node);
    return node;
}   //Find

/**
 *  This function creates a tree node with the given data and insert the node
 *  into the tree.
 *
 *  @param buffData Points to the data buffer.
 *  @param lenData Specifies the length of the data buffer.
 *  @param nodeRoot Points to the root node of the tree.
 *
 *  @return Success: Returns the newly inserted tree node.
 *  @return Failure: Returns NULL.
 */
AVLNode *
AVLTree::Insert(
    __in_bcount(lenData) PVOID buffData,
    __in                 size_t lenData,
    __in_opt             AVLNode *nodeRoot
    )
{
    TEnter(AVLTREE, WPPIndent, "buff=%p,len=%Id,root=%p",
           buffData, lenData, nodeRoot);

    TAssert(m_NodeData != NULL);
    if (nodeRoot == NULL)
    {
        nodeRoot = new AVLNode(buffData, lenData);
        if (nodeRoot == NULL)
        {
            TMsg(Fatal, "Failed to allocate new tree node.");
        }
        else
        {
            m_numNodes++;
        }
    }
    else
    {
        int result = m_NodeData->CompareData(buffData, lenData, nodeRoot);
        AVLNode *childLeft = nodeRoot->GetLeftChild();
        AVLNode *childRight = nodeRoot->GetRightChild();
        AVLNode *node;

        if (result < 0)
        {
            node = Insert(buffData, lenData, childLeft);
            nodeRoot->SetLeftChild(node);
            if (GetHeight(childLeft) - GetHeight(childRight) == 2)
            {
                if (m_NodeData->CompareData(buffData, lenData, childLeft) < 0)
                {
                    nodeRoot = SingleRotateWithLeft(nodeRoot);
                }
                else
                {
                    nodeRoot = DoubleRotateWithLeft(nodeRoot);
                }
            }
        }
        else if (result > 0)
        {
            node = Insert(buffData, lenData, childRight);
            nodeRoot->SetRightChild(node);
            if (GetHeight(childRight) - GetHeight(childLeft) == 2)
            {
                if (m_NodeData->CompareData(buffData, lenData, childRight) > 0)
                {
                    nodeRoot = SingleRotateWithRight(nodeRoot);
                }
                else
                {
                    nodeRoot = DoubleRotateWithRight(nodeRoot);
                }
            }
        }
    }
    nodeRoot->SetHeight(max(GetHeight(nodeRoot->GetLeftChild()),
                            GetHeight(nodeRoot->GetRightChild())) + 1);

    TExitPtr(AVLTREE, WPPIndent, nodeRoot);
    return nodeRoot;
}   //Insert

/**
 *  This function creates a new node the finds the correct place in the tree
 *  to insert the new node.
 *
 *  @param buffData Points to the data buffer.
 *  @param lenData Specifies the length of the data buffer.
 *
 *  @return Success: Returns the newly created tree node.
 *  @return Failure: Returns NULL.
 */
AVLNode *
AVLTree::Insert(
    __in_bcount(lenData) PVOID buffData,
    __in                 size_t lenData
    )
{
    TEnter(AVLTREE, WPPIndent, "buff=%p,len=%Id", buffData, lenData);

    m_nodeRoot = Insert(buffData, lenData, m_nodeRoot);

    TExitPtr(AVLTREE, WPPIndent, m_nodeRoot);
    return m_nodeRoot;
}   //Insert

/**
 *  This function deletes the given tree.
 *
 *  @param nodeRoot Points to the root node of the tree to be deleted.
 *
 *  @return None.
 */
void
AVLTree::DeleteTree(
    __in_opt AVLNode *nodeRoot
    )
{
    TEnter(AVLTREE, WPPIndent, "oot=%p", nodeRoot);

    WalkTree(0, PostOrder, OpDeleteNode, NULL, nodeRoot);

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //DeleteTree

/**
 *  This function deletes the entire tree.
 *
 *  @return None.
 */
void
AVLTree::DeleteTree(
    void
    )
{
    TEnterVoid(AVLTREE, WPPIndent);

    WalkTree(0, PostOrder, OpDeleteNode, NULL, m_nodeRoot);

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //DeleteTree

/**
 *  This function prints the tree to the console.
 *
 *  @param nodeRoot Points to the root node of the tree to be printed.
 *
 *  @return None.
 */
void
AVLTree::PrintTree(
    __in_opt AVLNode *nodeRoot
    )
{
    TEnterVoid(AVLTREE, WPPIndent);

    WalkTree(0, InOrder, OpPrintNode, NULL, nodeRoot);

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //PrintTree

/**
 *  This function prints the tree to the console.
 *
 *  @return None.
 */
void
AVLTree::PrintTree(
    void
    )
{
    TEnterVoid(AVLTREE, WPPIndent);

    WalkTree(0, InOrder, OpPrintNode, NULL, m_nodeRoot);

    TExitVoid(AVLTREE, WPPIndent);
    return;
}   //PrintTree

