/* Assuptions: tree is BST */
#include <stdio.h>
#include <stdlib.h>
typedef struct _node{
    int data;
    struct _node *left;
    struct _node *right;
}node;

node *newNode(int data)
{
    node *newnode = (node *)malloc(sizeof(node));
    if(!newnode){
        printf("Memory allocation failed\n");
        return NULL;
    }
    newnode->data = data;
    newnode->left = (newnode->right = NULL);
    return newnode;
}

node *insertNode(int data, node *bt)
{
    node *tmp = bt;
    node *prev = tmp;
    int left = 0;
    int right = 0;
    if(!tmp){
        return newNode(data);
    }
    node *newnode = newNode(data);
    while(tmp){
        prev = tmp;
        left = right = 0;
        if(data <= tmp->data){
            tmp = tmp->left;
            left = 1;
        }else if(data > tmp->data){
            tmp = tmp->right;
            right = 1;
        }
    }
    if(left)
        prev->left = newnode;
    }else if(right) {
        prev->right = newnode;
    }
    return bt;
}

/*! in-order traversal */
node *printTree(node *btree)
{
    printTree(btree->left);
    printf("%d->", btree->data);
    printTree(btree->right);
}

/*           10
           /    \
          9      18
        /  \     / \
       5    12  15  21 
      / \   / \      \
     2  7  11  13     25
                      /
                     24 
*/

int main(void)
{
    node *btree1;
    btree1 = insertNode(10, btree1);
    btree1 = insertNode(18, btree1);
    btree1 = insertNode(9, btree1);
    btree1 = insertNode(5, btree1);
    btree1 = insertNode(7, btree1);
    btree1 = insertNode(2, btree1);
    btree1 = insertNode(12, btree1);
    btree1 = insertNode(13, btree1);
    btree1 = insertNode(11, btree1);
    btree1 = insertNode(21, btree1);
    btree1 = insertNode(15, btree1);
    btree1 = insertNode(25, btree1);
    btree1 = insertNode(24, btree1);
    printTree(btree1);
}
