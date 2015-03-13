#include "stack.h"

int main()
{
    Stack st;
    Node* node;
    // push few elements and print 
    Data data1("manisha");
    st.push(2, data1);
    Data data2("nitin");
    st.push(10, data2);
    Data data3("kanchu");
    st.push(1, data3);
    st.printStack();

    // pop an element and print
    // caller takes the responsibility to use the node 
    //and then free the memory
    cout << "pop and print last element inserted " << endl;
    node = st.pop();
    if(node){
        node->printNode();
        delete node;
    }

    //print rest of the stack
    cout << "Remaining element after popping first one " << endl;
    st.printStack();

    // create empty stack
    Stack st_empty;
    cout << "printing empty stack " << endl;
    st_empty.printStack();

    //try to pop from empty stack
    cout << "popping from empty stack " << endl;
    node = st_empty.pop();
    if(node){
        node->printNode();
        delete node;
    }
}
