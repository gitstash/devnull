#include "stack.h"
#include <iostream>
Stack::Stack():  _top(NULL), _size(0)
{
}

bool Stack::isStackFull()
{
    if (_size == MAX_SIZE){
        return true;
    }
    return false;
}

bool Stack::isStackEmpty()
{
    if(_top == NULL){
        return true;
    }
    return false;
}

//Used for printing only -  doesnt pop elements
void Stack::printStack()
{
    Node* tmp = _top;
    while(tmp)
    {
        tmp->printNode();
        tmp = tmp->next;
    }
}

Node* Stack::push(int num, Data data)
{
    if(isStackFull()){
        cout << "Stack is full "<<endl;
    } else {
        Node* node = new Node(num, data) ;
        node->next = _top;
        _top = node;
        _size++;
    }
    return _top;
}
Node* Stack::pop()
{
    Node* node = NULL;
    if(isStackEmpty()){
        cout <<"Stack is empty " << endl;
    }else{
        node = _top;
        _top = _top->next;
    }
    return  node;
}
