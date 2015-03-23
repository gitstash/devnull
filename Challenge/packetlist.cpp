#include <string>
#include <iostream>
#include <fstream>

#include "packetlist.h"
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

    //memset(nnode, 0, sizeof nnode);
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
        cout << "Seq: " << tmp->seq << " len: " << tmp->len << endl;
        tmp = tmp->next;
    }
    cout << endl;
}

bool Packetlist::serializeData(string path)
{
    ofstream of(path.c_str(), ios::out | ios::binary);

    if(!of.is_open())
        return false;
    node *iter = head;

    while(iter) {
        of << iter->data;
        iter = iter->next;
    }

    of.close();

    return true;
}
