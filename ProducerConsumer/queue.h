#include <iostream>
#include <pthread.h>
using namespace std;

class Queue
{
    private:
        int mRear;
        int mFront;
        int mSize;
        int *mArr;
        pthread_mutex_t mEventmutex;
        pthread_cond_t mEventCond;

    public:
        Queue(int size);
        Queue() ;//defaultone
        ~Queue();
        bool enqueueData(int data);
        bool dequeueData(int *data);
        void printQueue();
};
