#include <iostream>
#include <pthread.h>
#include "btree.h"
#include <unistd.h>
using namespace std;

/*

      12
     /   \
    7     15
   / \    /  \
  4   9   13   17
 / \   \         \
2   6   10       19
        /
       11
*/
void *threadOne(void *arg)
{
    Btree *t = (Btree *)arg;
    t->insertNode(12);
    cout <<"Inserted 12" << endl;
    t->insertNode(7);
    cout << "Inserted 7"<<endl;
    t->insertNode(9);
    cout << "Inserted 9"<<endl;
    t->insertNode(4);
    cout << "Inserted 4"<<endl;
    t->insertNode(2);
    cout << "Inserted 2"<<endl;
    t->insertNode(10);
    cout << "Inserted 10"<<endl;
    t->insertNode(15);
    cout << "Inserted 15"<<endl;
    t->insertNode(17);
    cout << "Inserted 17"<<endl;
    t->insertNode(13);
    cout << "Inserted 13"<<endl;
    t->insertNode(6);
    cout << "Inserted 6"<<endl;
    t->insertNode(19);
    cout << "Inserted 19"<<endl;
    t->insertNode(11);
    cout << "Inserted 11"<<endl;
    t->printTree();
    cout << endl;
}

void *threadTwo(void *arg)
{
    Btree *t = (Btree *)arg;
    t->updateNode(4, 5); 
    cout << "Updated node 4"<< endl;
    sleep(5);
    t->updateNode(15, 14);
    cout << "Updated node 15" << endl;
    t->printTree();
    cout << endl;
}

void *threadFour(void *arg)
{
    Btree *t = (Btree *)arg;
    t->printBFS();
}

void *threadThree(void *arg)
{
    Btree *t= (Btree *)arg;
    t->deleteNode(10);
    cout << "Deleted node 10"<< endl;
    t->deleteNode(17);
    cout << "Deleted node 17"<< endl;
    t->deleteNode(9);
    cout << "Deleted node 9"<< endl;
    sleep(5);
    t->deleteNode(7);
    cout << "Deleted node 7"<< endl;
    t->printTree();
    cout << endl;
}

int main(void)
{
    pthread_t thr1, thr2, thr3, thr4;
    Btree t;
    //Spawn two threads and wait

    //pthread_create(&thr1, NULL, threadOne, &t);
    threadOne(&t);
    pthread_create(&thr2, NULL, threadTwo, &t);
    pthread_create(&thr3, NULL, threadThree, &t);
    pthread_create(&thr4, NULL, threadFour, &t);
    //pthread_join(thr1, NULL); 
    pthread_join(thr2, NULL); 
    pthread_join(thr3, NULL); 
    pthread_join(thr4, NULL); 
}

