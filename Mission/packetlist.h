#ifndef __PACKETLIST_H__
#define __PACKETLIST_H__
#include <fstream>
#include <string>

using namespace std;
typedef struct node_s
{
    int seq;
    int len;
    string data;
    struct node_s *next;
}node;


class Packetlist
{
    private:
        node *head;

        node *newPacket(int seq, int  len, string data);

    public:
        Packetlist();
        ~Packetlist();
        bool insertPacket(int seq, int len, string data);
        bool serializeData(string path);
        void printList();

};
#endif //__PACKETLIST_H__
