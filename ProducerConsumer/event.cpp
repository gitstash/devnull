#include "event.h"

void *Producer::work(void *eventQ)
{
    while(true){
        int data = 0;
        if(((Queue *)eventQ)->dequeueData(&data)){
                cout << "Dequeued data " <<  data << endl;
        }
        sleep(2);
    }
    return NULL;
}

void *Consumer::work(void *eventQ)
{
    while(true){
        ((Queue *)eventQ)->enqueueData(time(0));
        sleep(2);
    }
    return NULL;
}
