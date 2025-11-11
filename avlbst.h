#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
     int subtreeHeight(AVLNode<Key,Value>* x) const
    {
        if (x == NULL) 
        {
            return 0;
        }
        int lefth = subtreeHeight(x->getLeft());
        int righth = subtreeHeight(x->getRight());
        int max;
        if (lefth > righth)
        {
            max = lefth;
        }
        else max = righth;
        return 1 + max;
    }
     void recomputeBalance(AVLNode<Key,Value>* x)
    {
        if (x == NULL) 
        {
            return;
        }
        int lefth = subtreeHeight(x->getLeft());
        int righth = subtreeHeight(x->getRight());
        int diff = righth-lefth;
        x->setBalance(static_cast<int8_t>(diff));
    }
    void rotateLeft(AVLNode<Key,Value>*x)
    {
        if(x==NULL)
        {
            return;
        }
        AVLNode<Key,Value>* y=x->getRight();
        if(y==NULL)
        {
            return;
        }
        AVLNode<Key,Value>* yl=NULL;
        if(y!=NULL)
        {
            yl=y->getLeft();
        }
        AVLNode<Key,Value>* p=x->getParent();
        if(p==NULL)
        {
            this->root_=y;
        }
        else if(x==p->getLeft())
        {
            p->setLeft(y);
        }
        else
        {
            p->setRight(y);
        }
        y->setParent(p);
        y->setLeft(x);
        x->setParent(y);
        x->setRight(yl);
        if(yl!=NULL)
        {
            yl->setParent(x);
        }
        recomputeBalance(x);
        recomputeBalance(y);
    }
    void rotateRight(AVLNode<Key,Value>* x)
    {
        if (x == NULL)
        {
         return;
        }
        AVLNode<Key,Value>* y = x->getLeft();
        if (y == NULL) 
        {
            return;
        }

        AVLNode<Key,Value>* yr = NULL;
        if (y != NULL) 
        {
            yr = y->getRight();
        }
        AVLNode<Key,Value>* p = x->getParent();
        if (p == NULL) {
            this->root_ = y;
        } else if (x == p->getLeft()) {
            p->setLeft(y);
        } else {
            p->setRight(y);
        }
        y->setParent(p);
        y->setRight(x);
        x->setParent(y);
        x->setLeft(yr);
        if (yr != NULL) 
        {
            yr->setParent(x);
        }
        recomputeBalance(x);
        recomputeBalance(y);
    }

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_ == NULL) 
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        static_cast<AVLNode<Key, Value>*>(this->root_)->setBalance(0);
        return;
    }
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;
    while (current != NULL) {
        parent = current;
        if (new_item.first < current->getKey()) {
            current = current->getLeft();
        }
        else if (current->getKey() < new_item.first) {
            current = current->getRight();
        }
        else
        {
            current->setValue(new_item.second);
            return;
        }
    }
    AVLNode<Key, Value>* node = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    node->setBalance(0);
     if (new_item.first < parent->getKey()) {
        parent->setLeft(node);
    }
    else {
        parent->setRight(node);
    }
    AVLNode<Key,Value>* x=parent;
    while(x!=NULL)
    {
        this->recomputeBalance(x);
        int b=x->getBalance();
        if(b==2)
        {
            AVLNode<Key, Value>* R=x->getRight();
            this->recomputeBalance(R);
            if(R->getBalance()>=0)
            {
                this->rotateLeft(x);
            }
            else
            {
                this->rotateRight(R);
                this->rotateLeft(x);
            }
            break;
        }
        else if(b==-2)
        {
            AVLNode<Key,Value>* L=x->getLeft();
            this->recomputeBalance(L);
            if(L->getBalance()<=0)
            {
                this->rotateRight(x);
            }
            else{
                this->rotateLeft(L);
                this->rotateRight(x);
            }
            break;
        }
        x=x->getParent();
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* z = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (z == NULL)
    {
        return;
    }
    if (z->getLeft() != NULL && z->getRight() != NULL) {
        AVLNode<Key, Value>* pred =
            static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(z));
        this->nodeSwap(z, pred);
    }
     AVLNode<Key, Value>* child = NULL;
    if (z->getLeft() != NULL) 
    {
        child = z->getLeft();
    }
    else if (z->getRight() != NULL) child = z->getRight();

    AVLNode<Key, Value>* parent = z->getParent();
    if (child != NULL) 
    {
        child->setParent(parent);
    }
    if (parent == NULL) {
        this->root_ = child;
    }
    else if (z == parent->getLeft()) {
        parent->setLeft(child);
    }
    else {
        parent->setRight(child);
    }

    delete z;
    AVLNode<Key,Value>* x=parent;
    while(x!=NULL)
    {
        this->recomputeBalance(x);
        int b=x->getBalance();
        if(b==2)
        {
            AVLNode<Key,Value>* R=x->getRight();
            this->recomputeBalance(R);
            if(R->getBalance()>=0)
            {
                this->rotateLeft(x);
            }
            else
            {
                this->rotateRight(R);
                this->rotateLeft(x);
            }
            x=x->getParent();
        }
        else if(b==-2)
        {
            AVLNode<Key,Value>*L=x->getLeft();
            this->recomputeBalance(L);
            if(L->getBalance()<=0)
            {
                this->rotateRight(x);
            }
            else
            {
                this->rotateLeft(L);
                this->rotateRight(x);
            }
            x=x->getParent();
        }
        else
        {
            x=x->getParent();
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
