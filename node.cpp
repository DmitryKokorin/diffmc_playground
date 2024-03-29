#include "node.h"

#if !defined NULL
#define NULL 0
#endif


///////////////////////////////////////////////////////////////////////////////


Node::Node(const GreedRect& rect_) :
    rect(rect_),
    pParent(NULL),
    pChild1(NULL),
    pChild2(NULL)
{
}

Node::~Node()
{
    delete pChild1; pChild1 = NULL;
    delete pChild2; pChild2 = NULL;
}

bool Node::isLeaf() const
{
    return (NULL == pChild1) && (NULL == pChild2); 
}

bool Node::splitX() 
{
    if (!rect.canSplitX())
        return false;

    pChild1 = new Node(rect.leftHalf());
    pChild2 = new Node(rect.rightHalf());
    pChild1->pParent = this;
    pChild2->pParent = this;

    return true;
}

bool Node::splitY() 
{
    if (!rect.canSplitY())
        return false;

    pChild1 = new Node(rect.topHalf());
    pChild2 = new Node(rect.bottomHalf());
    pChild1->pParent = this;
    pChild2->pParent = this;

    return true;
}

