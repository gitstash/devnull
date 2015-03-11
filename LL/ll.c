#include "ll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node* newNode(int data)
{
    node *nnode = (node *) malloc (sizeof (node));
    if( !nnode )
    {
        printf("memory allocation failed");
        return NULL;
    }
    memset(nnode, 0, sizeof nnode);
    nnode->data = data;
    nnode->next = NULL;
    return nnode;
}
    
node *insertNode(node *mylist, int data)
{
    if(!mylist){
        return newNode(data);
    }else{
        node *tmp = mylist;
        while(tmp->next){
            tmp =  tmp->next;
        }
        tmp->next = newNode(data);
    }
    return mylist;
}

void printList(node *mylist)
{
    while( mylist )
    {
        printf("%d->", mylist->data);
        mylist = mylist->next;
    }
    printf("\n");
}
        
node* reverseListIter(node* mylist)
{
    /*! check if the list is NULL */
    if(!mylist)
        return NULL;

    node* prev =  mylist;
    node* cur = mylist->next;
    node* save = NULL;
    while(cur)
    {
        save = cur->next;
        cur->next = prev;
        prev = cur;
        cur = save;
    }

    mylist->next = NULL;
    mylist = prev;
    return mylist;
}

node* reverseListRec(node* mylist)
{
    /*! empty list */
    if( !mylist )
        return mylist;

    /*! end condition for recusion - or one node list */
    if(!mylist->next ){
        return mylist;
    }

    node* newhead;
    newhead = reverseListRec(mylist->next);
    mylist->next->next = mylist;
    mylist->next = NULL;

    return newhead;
}

int main(void)
{
    node* mylist1 = NULL;
    mylist1 = insertNode(mylist1, 4);
    mylist1 = insertNode(mylist1, 2);
    mylist1 = insertNode(mylist1, 12);
    mylist1 = insertNode(mylist1, 0);
    mylist1 = insertNode(mylist1, 8);
    mylist1 = insertNode(mylist1, 5);
    printList(mylist1);

    node* mylist2 = NULL;
    mylist2 = insertNode(mylist2, 10);
    mylist2 = insertNode(mylist2, 1);
    mylist2 = insertNode(mylist2, 12);
    mylist2 = insertNode(mylist2, 13);
    mylist2 = insertNode(mylist2, 2);
    printList(mylist2);

    /*! reverse a linkedlist  - iterative */
    mylist1 = reverseListIter(mylist1);
    printList(mylist1);

    /*! reverse a linkedlist - recursive */
    mylist1 = reverseListRec(mylist1);
    printList(mylist1);

}
