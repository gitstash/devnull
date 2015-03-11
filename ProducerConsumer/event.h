#include <unistd.h>
#include "queue.h"

class Event
{
    private:
          pthread_t mThreadId; //event will be running in a dedicated thread, so threadId
    protected:
          Queue mEventQ;
    public:
          Event(); 
          ~Event();
          void *work();
          virtual void *work(void *eventQ) = 0;
};

class Producer:public Event
{
    public:
        Producer();
        ~Producer();
        void *work(void *eventQ) ;
};

class Consumer:public Event
{
    public:
        Consumer(); 
        ~Consumer();
        void *work(void *eventQ);
};
