#ifndef _LL_H_
#define _LL_H_
typedef struct _page
{
    int id;
    int pid;
}page;

typedef struct  _node
{
    page pg;
    struct _node *prev;
    struct _node *next;
    _node(): prev(NULL), next(NULL) {}
#if 0
    {
        prev = NULL;
        next = NULL;
    }
#endif
}node;

class list
{
    int maxSize;
    int counter;
    node *head;
    node *tail;
    public:
    list(int size); 
    ~list();
    node *enqueue(page p);
    node *deleteNode(node *p);
    node *dequeue();
    void printList();
    bool isFull();
    void printQueue();
};

#endif //_LL_H_
