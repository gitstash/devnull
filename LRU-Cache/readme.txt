Design doubly Linked list

typedef struct Page_s
{
    int id;
    int age;
    int pid;
} Page_t;


1. insert - should take page struct, should return pointer to the node inserted.
    ( the node pointer will be used by insertHash api to add the data into Hash)
2.  delete - should be overload.
    a) doesnt take argument and delete the oldest page maintained as first node in LL
    b) takes Node pointer as argument. 
3. print  - prints ll


Hash design;
