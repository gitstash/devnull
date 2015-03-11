#include <iostream>
#include "btree_recur.h"
using namespace std;

Node *Btree::insertNode(Node *root, int data)
{
    if(!root){
        return root;
    }

    if(data <= root->mData){
        root->mLeft = insertNode(root->mLeft, data);
    }else {
        root->mRight = insertNode(root->mRight, data);
    }
    return mRoot;
}
void Btree::insertNode(int data)
{
    insertNode(mRoot, data);
}

int main(void)
{

    Btree t;
    t.insertNode(12);
    cout <<"Inserted 12" << endl;
    t.insertNode(7);
    cout << "Inserted 7"<<endl;
    t.insertNode(9);
    cout << "Inserted 9"<<endl;
    t.insertNode(4);
    cout << "Inserted 4"<<endl;
    t.insertNode(2);
    cout << "Inserted 2"<<endl;
    t.insertNode(10);
    cout << "Inserted 10"<<endl;
    t.insertNode(15);
    cout << "Inserted 15"<<endl;
    t.insertNode(17);
    cout << "Inserted 17"<<endl;
    t.insertNode(13);
    cout << "Inserted 13"<<endl;
    t.insertNode(6);
    cout << "Inserted 6"<<endl;
    t.insertNode(19);
    cout << "Inserted 19"<<endl;
    t.insertNode(11);
    cout << "Inserted 11"<<endl;
}
