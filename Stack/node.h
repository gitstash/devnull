#include "data.h"
class Node
{
    private:
        friend class Stack;
        int _num;
        Data _data;
        Node* next;
    public:
        Node(int num, Data data): _num(num), _data(data)
        {
        }
        void printNode()
        {
            cout << _num << ":" << _data << endl;
        }
};

