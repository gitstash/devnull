#include <iostream>
#include <stdlib.h>
#include "hash.h"

hashtbl::hashtbl(int bucketsize):
    nbuckets(bucketsize), nfilled(0)
{
    // using malloc for memory allocation
    // later when we need to expand the memory
    // we can use re-alloc function
    tbl = (value *)malloc( nbuckets * sizeof(value)); 
    if(tbl == NULL) {
        cout << "Memory allocation failed"<< endl;
    }
    for(int i = 0 ; i < nbuckets ; i++) {
        tbl[i].isFull = false;
    }
}

int hashtbl::hash(int pageId)
{
    return pageId;
}

bool hashtbl::resize(int newsz)
{
    cout<<"Resizing hash....please wait!\n"<<endl;
    if(newsz < nbuckets) {
        cout<<"Cant shrink the hash, sorrry!"<<endl;
        return false;
    }

    tbl = (value *)realloc(tbl, newsz * sizeof(value));
    if(tbl == NULL)
    {
        cout << "Bucket re-size failed " << endl;
        return false;
    }
    return true;
}
node *hashtbl::get(int pageId)
{
    int k = hash(pageId);

    return tbl[k].isFull ? tbl[k].n : NULL;
}

void hashtbl::setEmpty(node *pageNode)
{
    tbl[hash(pageNode->pg.id)].isFull = false;
}
bool hashtbl::put(int pageId, node *n)
{
    //if it 80% full, resize the tbl
    if( nfilled/nbuckets* 100 > 80) {
        resize(2 * nbuckets);
    }
    int key = hash(pageId);

    if( tbl[key].isFull ) { //collision
        cout << "Hash collision, handle it.." << endl;
        return false;
    } else {
        tbl[key].n = n;
        tbl[key].isFull = true;
    }
    return true;
}

bool hashtbl::del(int pageId)
{
    int k = hash(pageId);
    if(tbl[k].isFull)
        tbl[k].isFull = false;
    return true;
}

void hashtbl::printHash()
{
    cout << "Printing hash...." << endl;
    for ( int i = 0;  i < nbuckets ; i++)
    {
        if(tbl[i].n != NULL)
            cout << "@" << tbl[i].n << ":" << hash(tbl[i].n->pg.id) << "->" << tbl[i].n->pg.id << " " << tbl[i].n->pg.pid << " " << tbl[i].isFull << " | ";
    }
    cout << endl;
}
