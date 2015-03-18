#include <iostream>
#include "ll.h"
using namespace std;

list::list(int size): maxSize(size), counter(0), head(NULL), tail(NULL)
{}
list::~list(){}

node* list::enqueue(page p)
{
    node *newnode;
    try
    {
        newnode = new node();
    }
    catch(bad_alloc &ba)
    {
        cout << "Memory allocation for node failed"<< endl;
        return NULL;
    }
    //maintain sorted ll based in page age
    //traverse the list till we get an elemtn greater than age part of the node
    newnode->pg = p;
    //handle first node 
    if(tail == NULL)
    {
        head = tail = newnode;
    }
    else
    // enqueue in the rear always
    {
        tail->next = newnode;
        newnode->prev = tail;
        newnode->next = NULL;
        tail = newnode;
    }
    counter++;
    return newnode;
}

node *list::dequeue()
{
    //always delete node from head
    node *tmp = head;
    head = head->next;
    counter--;
    return tmp;
}

void list::printList()
{
    node *tmp = head;
    cout << "printing start to end" << endl;
    while(tmp)
    {
        cout << tmp->pg.id << "->" ;
        tmp = tmp->next;
    }
    cout << endl;
    tmp = tail;
    cout << "printing end to start" << endl;
    while(tmp)
    {
        cout << tmp->pg.id << "->" ;
        tmp = tmp->prev;
    }
    cout << endl;
}
node* list::deleteNode(node *p )
{
    if( p == head)
    {
        head = p->next;
        p->next->prev = NULL;
    }
    //if its end node
    else if( p == tail )
    {
        tail->prev->next = NULL;
        tail = tail->prev;
    }
    else
    {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    }
    counter--;
    return p;
}

bool list::isFull()
{
    return counter >= maxSize ? true : false;
}
void list::printQueue()
{
    node *tmp = head;
    while(tmp)
    {
        cout << "{ " << tmp->pg.id << " " << tmp->pg.pid << " }->" ;
        tmp = tmp->next;
    }
    cout << endl;
}
#if 0
int main(void)
{
    list l1(50);
    page p1 = {2, 1010};
    node *n1, *n2, *n3, *n4, *n5;
    n1 = l1.enqueue(p1);
    l1.printList();
    p1 = {4, 115};
    n2 = l1.enqueue(p1);
    p1 = {20, 1000};
    n3 = l1.enqueue(p1);
    p1 = {3, 2030};
    n4 = l1.enqueue(p1);
    p1 = {1, 3333};
    n5 = l1.enqueue(p1);

    l1.printList();
    l1.deleteNode(n5);
    l1.printList();
}
#endif
