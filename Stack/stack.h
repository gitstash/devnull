#include "node.h"

#define MAX_SIZE 10
class Stack
{
    private:
        //Same as LL head
        Node* _top;
        int _size;

    bool isStackFull();
    bool isStackEmpty();
    public: 
        Stack();
        // returns updated list( stack )
        Node* push(int num, Data data);
        //retrieves the last node pushed
        Node* pop();
        void printStack();
};


