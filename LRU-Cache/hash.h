#include <iostream>
#include "ll.h"
using namespace std;

typedef struct {
    node *n;
    bool isFull;
}value;

class hashtbl
{
    int nbuckets;
    int nfilled;
    value *tbl;

    int hash(int pageid);
    bool resize(int newsz);

    public:
        hashtbl(int bucketsize);
        node *get(int pageId);
        bool put(int pageId, node *n);
        bool del(int pageId);
        void printHash();
        void setEmpty(node *);
        bool isFull();
};
