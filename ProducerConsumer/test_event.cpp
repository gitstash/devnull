#include "event.h"
int main(void)
{
    Queue q1(10);
    int data = 0;
    cout << "Dequeueing data from empty queue "<< endl;
    if(q1.dequeueData(&data)){
        cout << "dequeued " <<  data << endl;;
    }

    //Testing queue by inserting 10 elements and deleting the very same elements
    q1.enqueueData(23);
    q1.enqueueData(3);
    q1.enqueueData(2);
    q1.enqueueData(13);
    q1.enqueueData(20);
    q1.enqueueData(4);
    q1.enqueueData(5);
    q1.enqueueData(10);
    q1.enqueueData(24);
    q1.enqueueData(0);
    q1.printQueue();
    cout << "Adding element to the full queue "<< endl;
    q1.enqueueData(33);

    cout << "Dequeueing data "<< endl;
    if(q1.dequeueData(&data)){
        cout << "dequeued " << data << endl;;
    }
    q1.dequeueData(&data);
    q1.dequeueData(&data);
    q1.dequeueData(&data);
    q1.dequeueData(&data);
    q1.dequeueData(&data);
    q1.dequeueData(&data);
    q1.dequeueData(&data);
    q1.dequeueData(&data);
    q1.dequeueData(&data);

    cout << "Dequeueing data from empty queue "<< endl;
    if(q1.dequeueData(&data)){
        cout << "dequeued " <<  data << endl;
    }

    // create consumer and producer threads using base class pointer operate on same queue
    pthread_t th1, th2;
    Event *p = new Producer();
    pthread_create(&th1, NULL, p->work, &q1);

    Event *c = new Consumer();
    pthread_create(&th2, NULL, c->work, &q1);

    pthread_join(th1, NULL); 
    pthread_join(th2, NULL); 
}
