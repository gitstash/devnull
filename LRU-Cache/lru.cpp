#include <iostream>
#include <vector>
#include "hash.h"

class lrucache
{
private:
     list queue;
     hashtbl hash;
public:
    lrucache(int maxSize, int maxPages) : queue(maxSize), hash(maxPages)
    {}
    ~lrucache(){}
    void refer(int pageId, int pid)
    {
        //check if the page exist
        node *pageNode = hash.get(pageId);

        //page doesnt exist, add it
        if(pageNode == NULL)
        {
            cout << "Page { " << pageId << " " <<  pid << " }" << " doesnt exist " ;
            page p = { pageId, pid }; 
            //check if the queue is full
            if(queue.isFull())
            {
                cout << "Queue is full, kicking out oldest page " << endl;
                //kick out the oldest page(from front)
                pageNode = queue.dequeue();
                //mark the dirty bit on for the page in hash table
                hash.setEmpty(pageNode);
                delete pageNode;
            }
            //enqueue in the end
            node *n = queue.enqueue(p);
            if(n != NULL)
                hash.put(pageId, n);
        }
        else
        {
            //found the node
            cout << "Found the node " << endl;
            //delete the node from original pos 
            //and add the node to rear of the queue
            pageNode = queue.deleteNode(pageNode);
            queue.enqueue(pageNode->pg);
            //hash.put(pageId, pageNode);
        }
    }
    void fillTestData(vector<pair<int, int>> &vec)
    {
#if 0
        int arr[100] ;
        for(int i =0; i< 100; i++)
        {
            arr[i] = (i + 1) *100;
        }
#endif
        int seed = time(NULL);
        srand(seed);
        for (int i = 0; i < 10; i++) {
            int pageId =  rand()%100;
            int pid = rand() % 10;
            page p = {pageId, pid};
            //For test purpose store these values in a map to be accessed later for delete/refer operation
            vec.push_back(std::make_pair(pageId, pid));
            node *pageNode = queue.enqueue(p);
            if( !hash.put(pageId, pageNode))
                cout<<"Hash put() failed!"<<endl;
        }
        //print the queue
        queue.printQueue();
        //print the hash
        hash.printHash();
    }
    void printQueueData()
    {
            //print the queue
            cout << "New Queue... " << endl;
            queue.printQueue();
    }
    void printHashData()
    {
        cout << "New hash...." << endl;
        hash.printHash();
    }
    void removeEntry(int pageId)
    {
        node *n = hash.get(pageId);
        queue.deleteNode(n);
        hash.setEmpty(n);
    }
};

int main(void)
{
    lrucache lru(10, 100);
    std::vector<pair<int, int>> test_data;
    lru.fillTestData(test_data);
    
    //refer using page id and pid
    int i = 4;
    cout << "Test case: 1: referring an existing page " << endl;
    cout << "Refering { " << test_data[i].first << " " <<  test_data[i].second << " }" << endl;
    lru.refer(test_data[i].first, test_data[i].second);
    lru.printQueueData();
    lru.printHashData();

    cout << "--------------------------------------------" << endl;
    cout << "Test case: 2: referring non-existing page when queue is full " << endl;
    cout << "Refering page {12 2}" << endl;
    lru.refer(12, 2);
    lru.printQueueData();
    lru.printHashData();

    //delete 1 element from queue to make place for new page
    //cout << "Delete one random page to make place and then try to refer a new page " << endl;
    i = 7;
    lru.removeEntry(test_data[i].first);

    cout << "--------------------------------------------" << endl;
    lru.printQueueData();
    lru.printHashData();
    cout << "Test case: 3: referring non-existing page when queue has space " << endl;
    cout << "Refering { " << test_data[i].first << " " <<  test_data[i].second << " }" << endl;
    lru.refer(test_data[i].first, test_data[i].second);
    lru.printQueueData();
    lru.printHashData();
    return EXIT_SUCCESS;
}
