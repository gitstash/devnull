#include "linkedlist.h"
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

Packetlist::Packetlist():head(NULL)
{}
Packetlist::~Packetlist()
{
    node *iter = NULL;
    while(head){
        iter = head->next;
        delete head;
        head = iter;
    }
}
node* Packetlist::newPacket(int seq, int len, string data)
{
    node *nnode = NULL;
    try{
        nnode = new node;
    }catch(std::bad_alloc &ba){
        cout << "Mem alloc failed " << endl;
        throw ba;
    }

    memset(nnode, 0, sizeof nnode);
    nnode->seq = seq;
    nnode->len = len;
    nnode->data = data;
    nnode->next = NULL;
    return nnode;
}
    
bool Packetlist::insertPacket(int seq, int len, string data)
{
    node *newnode = newPacket(seq, len, data);
    if(!head){
        head = newnode;
        return true;
    }

    node *tmp = head, *prev = NULL;
    
    while(tmp && (seq > tmp->seq)){
        prev = tmp;
        tmp = tmp->next;
    }

    if(tmp == head){
        newnode->next = tmp;
        head = newnode;
    }else if(tmp == NULL){
        prev->next = newnode;
    }else{
        prev->next = newnode;
        newnode->next= tmp;
    }
    return true;
}

void Packetlist::printList()
{
    node *tmp = head;
    while( tmp )
    {
        cout << "Seq: " << tmp->seq << " len: " << tmp->len << " data: " << tmp->data << endl;
        tmp = tmp->next;
    }
    cout << endl;
}

bool Packetlist::serializeData(ofstream &of)
{
    if(!of.isopen())
        return false;
    node *iter = head;

    while(iter) {
        of.write(iter->data.c_str(), iter->len);
        iter = iter->next;
    }

    return true;
}
#if 0
int main(void)
{
    Packetlist p;
    p.insertPacket(4, 5, "nitin");
    p.insertPacket(2, 3, "pug");
    p.insertPacket(12, 1, "a");
    p.insertPacket(0, 10, "abcdefghij");
    p.insertPacket(8, 2, "hi");
    p.insertPacket(5, 3, "mum");
    p.printList();
#endif

}
