/* Assumptions: Its a binary search tree */

#include <iostream>
#include <pthread.h>
#include "btree.h"
#include <queue>
using namespace std;


bool Btree::insertNode(int data)
{
    if(pthread_mutex_lock(&mLock))
        return false;

    if(!mRoot){
        //cout << "1. Incoming number " << data << endl;
        try {
            mRoot = new Node(data);
        }
        catch (std::bad_alloc& ba) {
            //cerr << "bad_alloc caught: " << ba.what() << endl;
            pthread_mutex_unlock(&mLock);
            return false;
        }
    }else{
        try{
            Node *newnode = new Node(data);
            Node *tmp = mRoot;
            Node *prev = tmp;
            bool leftFlag = false;
            bool rightFlag = false;
            while(tmp)
            {
                leftFlag = rightFlag = false;
                prev = tmp;
                if(data <= tmp->mData){
                    tmp =  tmp->mLeft;
                    leftFlag = true;
                }else if(data > tmp->mData){
                    tmp = tmp->mRight;
                    rightFlag = true;
                }
            }
            if(leftFlag){
                //cout << "2. Incoming number " << data << endl;
                prev->mLeft = newnode;
            }else if(rightFlag){
                //cout << "3. Incoming number " << data << endl;
                prev->mRight = newnode;
            }
        }catch(bad_alloc *ba){
            //cerr << "Node allocation failed " << endl;
            pthread_mutex_unlock(&mLock);
            return false;
        }
    }

    pthread_mutex_unlock(&mLock);
    return true;
}

void Btree::printTree(Node *node)
{
    if(!node)
        return;
    printTree(node->mLeft);
    cout << node->mData << "->" ;
    printTree(node->mRight);
}

void Btree::printTree()
{
    /* terminating condition */
    /* Based on assumption that this is a binary tree in-order traversal 
       would give the sorted result */
    if(!mRoot){
        //cout << "Tree is empty " << endl;
        return ;
    }
    printTree(mRoot);
}

void Btree::updateNode(int oldData, int data)
{
    /* traverse and find the oldData and replace with new data */
    Node *tmp = mRoot;
    bool found = false;
    Node *prev = NULL;
    bool leftFlag = false;
    bool rightFlag = false;
    int minVal = 0;
    int maxVal = 0;
    //cout << "Request for replacing " << oldData << " with " << data << endl;
    while(tmp)
    {
        if(oldData == tmp->mData) {
            //cout << "Found data , Validating .." << endl;
            /*! Check if it was left or right child of its parent */
            if(leftFlag && prev->mData < data ){
                //cerr << "1. Invalid input data, exiting.... " << endl;
                return;
            /*! Check if it was left or right child of its parent */
            }else if(rightFlag && prev->mData > data ){
                //cerr << "2. Invalid input data, exiting..." << endl;
                return;
            // check if the max of left sub-tree is bigger and
            // min of right sub-tree is smaller, if either is true,
            // log & exit without updating 
            }else if ((getMax(tmp->mLeft, maxVal) && data < maxVal)  || (getMin(tmp->mRight, minVal) && data > minVal)){
                //cerr << "3. Invalid input data, exiting..." << endl;
                return;
            }
            tmp->mData = data;
            found = true;
            break;
        }else if(oldData < tmp->mData){
            prev = tmp;
            tmp = tmp->mLeft;
            leftFlag = true;
            rightFlag = false;
        }else if(oldData > tmp->mData){
            prev = tmp;
            tmp = tmp->mRight;
            rightFlag = true;
            leftFlag = false;
        }
    }
    if(!found) {
        //cout << "Data to be replace dont exist " << endl;
    }
    return;
}

bool Btree::getMin(Node *root, int& minVal)
{
    Node *tmp = root;
    if(!root){
        return false;
    }
    while(tmp->mLeft) {
        tmp = tmp->mLeft;
    }
    minVal = tmp->mData;
    return true;
}

bool Btree::getMax(Node *root, int& maxVal)
{
    Node *tmp = root;
    if(!root){
        return false;
    }
    while(tmp->mRight) {
        tmp = tmp->mRight;
    }
    maxVal = tmp->mData;
    return true;
}

void Btree::deleteNode(int data)
{
    /*! deleting data requires  deleting/free the node and
      re-adjusting the complete tree to maintain it to be BST 
      doing it iteratively - at any point of time need to 
      maintain parent of the data to be deleted */

    Node *tmp = mRoot;
    Node *prev = tmp;
    bool found = false;
    bool leftFlag = false;
    bool rightFlag = false;

    //cout << "Request for deleting " << data << endl;

    if(!mRoot){
        //cerr << "Empty tree, exiting.."<< endl;
        return;
    }
    while(tmp) {
        if(data == tmp->mData){
            found = true;
            break;
        }else if(data < tmp->mData){ 
            prev = tmp;
            tmp = tmp->mLeft;
            leftFlag = true;
            rightFlag = false;
        }else if(data > tmp->mData){
            prev = tmp;
            tmp = tmp->mRight;
            rightFlag = true;
            leftFlag = false;
        }
    }
    if(found){
        /* case 1 -  delete a node with no child takes care of root being only node */
        if(tmp->mLeft ==  NULL && tmp->mRight == NULL){
            delete tmp;
            if(leftFlag){
                prev->mLeft = NULL;
            }else if(rightFlag){
                prev->mRight = NULL;
            }else{
                // handle root node
                mRoot = NULL;
            }
        }else if(tmp->mLeft == NULL && tmp->mRight != NULL){
            if(leftFlag){
                prev->mLeft = tmp->mRight;
                delete tmp;
            }else if(rightFlag){
                prev->mRight = tmp->mRight;
                delete tmp;
            }else{
                //to handle root node deletion
                mRoot = tmp->mRight;
                delete tmp;
            }
        }else if(tmp->mRight == NULL &&  tmp->mLeft != NULL){
            if(leftFlag){
                prev->mLeft = tmp->mLeft ;
                delete tmp;
            }else if(rightFlag){
                prev->mRight = tmp->mLeft;
                delete tmp;
            }else{
                //to handle root node deletion
                mRoot = tmp->mLeft;
                delete tmp;
            }
        }else if(tmp->mLeft && tmp->mRight){ // when neither of the childer are NULL
            //replace tmp->mData with min or right subtree or max of left subtree*/
            //cout << "The node to be deleted has two children "<< endl;
            int minVal = 0;
            if(!getMin(tmp->mRight, minVal)){
                //cerr << "Right sub-tree is empty " << endl;
                return ;
            }
            tmp->mData = minVal;
            /* recurse down to min of right subtree and delete the node */
            prev = tmp;
            tmp = tmp->mRight;
            rightFlag = true;
            leftFlag = false;
            while(tmp->mLeft){
                prev = tmp;
                tmp = tmp->mLeft;
                leftFlag = true;
                rightFlag = false;
            }
            if(leftFlag) {
                if(tmp->mRight != NULL){
                    prev->mLeft = tmp->mRight;
                }else{
                    prev->mLeft = NULL;
                }
            }else if(rightFlag){
                if(tmp->mRight != NULL){
                    prev->mRight = tmp->mRight;
                }else{
                    prev->mRight = NULL;
                }
            }
            delete tmp;
        }
    }else{
        //cerr << "Data " << data << " to be delete not found.." << endl;
    }
    return;
}
void Btree::printBFS()
{
    deque<node *> q;
    q.push_back(mRoot);
    node *tmp;
    while(!q.empty())
    {
        if(tmp->left)
        {
            tmp = tmp->left;
            q.push_back(tmp);
        }
        if(tmp->right)
        {
            tmp = tmp->right;
            q.push_back(tmp);
        }
        cout << "Visited node -> " << q.pop() ;
    }
}
