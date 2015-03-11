#include "event.h"
Queue::Queue() {}
Queue::Queue(int size): mRear(-1), mFront(-1), mSize(size)
{ 
    mEventmutex = PTHREAD_MUTEX_INITIALIZER;
    mEventCond = PTHREAD_COND_INITIALIZER;
    try{
        mArr = new int[mSize];
    }catch(std::bad_alloc &bd){
        cout << "Mem allocation failed for Queue " << endl;
    }
}

Queue::~Queue()
{
    delete mArr;
}

bool Queue::enqueueData(int data)
{

    pthread_mutex_lock(&mEventmutex);
    if((mRear+1) == mSize){
        cout << "Queue is full " << endl;
        return false;
    }

    if(mRear == -1){
        mArr[++mRear] = data;
        mFront++;
    }else{
        mArr[++mRear] = data;
    }
    pthread_cond_signal(&mEventCond);
    pthread_mutex_unlock(&mEventmutex);
    return true;
}

bool Queue::dequeueData(int *data)
{
    pthread_mutex_lock(&mEventmutex);
    pthread_cond_wait(&mEventCond, &mEventmutex);
    if(mFront > mRear || mRear == -1 ){
        cout << "Queue is empty " << endl;
        return false;
    }else{
        *data =  mArr[mFront++];
        return true;
    }
    pthread_mutex_unlock(&mEventmutex);
}


void Queue::printQueue()
{
    int count = mFront;
    while(count <= mRear){
        cout << mArr[count++]  << "->" ;
    }
    cout << endl;
}


