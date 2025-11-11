#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
static bool checkDepth(Node* node,int depth,int& firstDepth)
{
    if(node==NULL)
    {
        return true;
    }
    if(node->left==NULL &&node->right==NULL)
    {
        if(firstDepth==-1)
        {
            firstDepth=depth;
            return true;
        }
        return depth==firstDepth;
    }
    return checkDepth(node->left,depth+1,firstDepth)&&checkDepth(node->right,depth+1,firstDepth);
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root==NULL)
    {
        return true;
    }
    int firstDepth=-1;
    return checkDepth(root,0,firstDepth);

}

