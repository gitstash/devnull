#include <errno.h>
#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#define TOO_BIG 1000000

Client::Client(string hostname, uint16_t port, string email):
    mEmail(email), mAuthCode("")
{
    struct hostent *server;
    stringstream errObj;
    mSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(mSockfd == -1) {
        errObj << "Error opening socket (errno " << errno << " )" << endl;
        throw runtime_error("Error opening socket");
    }
    server = gethostbyname(hostname.c_str());
    if (server == NULL) {
        errObj << "Couldnt resolve " << hostname << " (h_errno " << h_errno<<  ")" << endl;
        throw runtime_error(errObj.str());
    }

    memset(&mServer,0, sizeof mServer);
    mServer.sin_family = AF_INET;
    mServer.sin_addr = *((struct in_addr*)server->h_addr_list[0]);
    char ss[MAX_BUF]="";

    inet_ntop(AF_INET, (void *)&mServer.sin_addr, ss, sizeof ss);
    cout<<"DEBUG: server is "<< ss<<endl;
    mServer.sin_port = htons(port);
}


Client::~Client()
{
    close(mSockfd);
}

void Client::startConnection()
{
    stringstream errObj;
    if(connect(mSockfd, (sockaddr *)&mServer,sizeof(mServer)) < 0) 
    {
        errObj << "Error connecting .. (errno: " << errno << " )" << endl;
        throw runtime_error(errObj.str());
    }
}

uint32_t Client::send(string msgBuf)
{
    stringstream errObj;
    ssize_t n = write(mSockfd, msgBuf.c_str(), msgBuf.size());
    if( n < 0 )
    {
        errObj << "Error writing to the socket.. (errno: " << errno << " )" << endl;
        throw runtime_error(errObj.str());
    }
    return n;
}

uint32_t Client::receive(char msgBuf[], uint32_t numByte)
{
    stringstream errObj;
    ssize_t n = read(mSockfd, msgBuf, numByte);
    if( n < 0 ){
        errObj << "Error reading ...(errno: " << errno << " )" << endl;
        throw runtime_error(errObj.str());
    }

    return n;
}

string Client::parse(const string &msg)
{
    unsigned first = msg.find(":") + 1;
    unsigned last = msg.find("\n");
    string challengeNum = msg.substr (first, last-first);
    return challengeNum;
}
bool Client::handshake()
{
    char msgBuf[MAX_BUF];
    stringstream ss;
    if(!receive(msgBuf, MAX_BUF)) { 
        return false;
    }

    //cout << "-------Received-------" << endl;
    //cout << msgBuf <<  endl;
    string token = parse(msgBuf);
    ss << "IAM:" << token << ":" << mEmail << ":at\n"; 
    //cout << "-----Sending----" << endl;
    //cout << ss.str() << endl;
    send(ss.str());
    if(!receive(msgBuf, MAX_BUF))
        return false;
    //cout << "-------Received-------" << endl;
    //cout << msgBuf <<  endl;

    //check if success
    if(strstr(msgBuf, "SUCCESS") == NULL)
        return false;
    //store auth code for later use, if any
    mAuthCode = msgBuf;
    return true;
}

// Each packet has the following structure: 
// 0   4   8   12 
// +---+---+---+=============================+ 
// |SEQ|CHK|LEN|…LEN bytes of raw Linear PCM | 
// +---+---+---+=============================+ 
// 
// CHK (Checksum) 
//     The 32-bit checksum is calculated by XOR'ing each 32-bit chunk of 
//     the 4 byte big-endian sequence number(SEQ) and data.  If LEN is 
//     not a multiple of 4, the byte 0xAB shall be used in place of the 
//     missing bytes only for the purpose of calculating the checksum. 

int Client::getChunk(uint32_t *pseq, char **chunk)
{
    uint32_t seq, csum, datalen;
    char hdrbuf[4];

    char *dataBuf;
    uint32_t n = 0;
    uint32_t readlen = 0;

    if(!pseq || !chunk)
        return -1;

    //check if authenticated
    if(mAuthCode.empty())
        return -1;

    //read sequence num
    if(receive(hdrbuf, 4) < 4)
        return -1;
    memcpy(&seq, hdrbuf, 4);
    *pseq = ntohl(seq);
    //cout << "-------Received SeqNum-------" << endl;
    //cout << htonl(seq) <<  endl;

    //read checksum
    if(receive(hdrbuf, 4) < 4)
        return -1;
    memcpy(&csum, hdrbuf, 4);
    //cout << "-------Received csum-------" << endl;
    //cout << csum <<  endl;

    //data len
    if(receive(hdrbuf, 4) < 4)
        return -1;
    memcpy(&datalen, hdrbuf, 4);
    datalen = ntohl(datalen);
    cout << "-------Received len-------" << endl;
    cout << datalen <<  endl;

    if(datalen > TOO_BIG){
        cout << "data too big ..Error.. " << endl;
        return -1;
    }


#if 0
    try{
        dataBuf = new char[datalen];
    }catch(bad_alloc &ba){
        cerr << "Bad alloc. "<< endl;
        return -1;
    }
#endif

    dataBuf = (char *)malloc (datalen);
    if(dataBuf == NULL){
        cout << "Mem alloc failed " << endl;
        return -1;
    }

    //data
    readlen = 0;
    while(readlen < datalen) {
        if( !(n = receive(dataBuf + readlen, datalen)) )
            break;
        readlen += n;
        //cout << readlen << " of " << datalen << " bytes received" << endl;
    }

    //cout << "Total received bytes: " << readlen << endl;
    if(readlen < datalen) {
        cout << "Pkt seq#:" << ntohl(seq) << " Warning: pkt receipt truncated" << endl;
    }

    //validate checksum
    if(isValidChecksum(csum, seq, dataBuf, readlen)) {
        cout << "Pkt seq#:" << ntohl(seq) << " Valid checksum " << endl;
        *chunk = dataBuf;
    } else {
        cout << "Pkt seq#:" << ntohl(seq) << " Checksum invalid" << endl;
        *chunk = dataBuf;
    }
    return readlen;
}

bool Client::saveChunk(const char *buf, uint32_t len)
{
    FILE *fp = NULL;
    if( !(fp = fopen("/tmp/data.pcm", "a")) ) {
        cout << "File open failed" << endl;
        return false;
    }

    if(fwrite(buf, 1, len, fp) != len) {
        cout << "Truncated output file: /tmp/data.pcm" << endl;
    }
#if 0
    perror("fopen");
    if( fp == NULL ) {
        cout << "Error opening file for writing " << endl;
        return;
    }
    uint32_t n = 0;
    uint32_t readLen = 0;
    while(readLen<=len){
      n = fwrite(buf, 1, len, fp);
      perror("fwrite");
      readLen+=n;
    }
#endif
    fclose(fp);
    return true;
}

#if 0
bool Client::isValidChecksum(uint32_t csum, uint32_t seq,
        const char *dataBuf, uint32_t len)
{
    uint32_t checksum = 0;
    uint32_t newnum = 0;
    int i =0;
    for(i =0 ; i < len/4 ; i = i+4)
    {
        checksum^=*((uint32_t *)&dataBuf[i]);
    }
    cout << "checksum  of data alone: " << checksum << endl;
    checksum^=seq;
    cout << "checksum after XORing with seq: " << checksum << endl;
    checksum^=htonl(len);
    cout << "checksum after XORing with Len: " << checksum << endl;
    //xor with (4 - remainder) times with 0xab 
    uint32_t remainder = len%4;
    if(remainder == 1)
        newnum = (*(uint8_t *)&dataBuf[i] << 24) | (0xab << 16) | (0xab << 8) | 0xab;
    else if(remainder == 2)
        newnum = (*(uint16_t *)&dataBuf[i] << 16) | (0xab << 8) | 0xab;
    else if(remainder == 3)
        newnum = (*(uint32_t *)&dataBuf[i] << 8)|0xab;

    checksum^=newnum;
    cout << "Calculated checksum " << checksum << endl;
    cout << "Packet checksum " << csum << endl;
    return ((checksum^csum) == 0) ? true : false;
}
#endif
#if 1
//Calculate checksum in network-byte order
bool Client::isValidChecksum(uint32_t csum, uint32_t seq,
        const char *dataBuf, uint32_t len)
{
    uint32_t *pnum = (uint32_t *) dataBuf;
    uint32_t num = len / sizeof(uint32_t);
    uint32_t checksum = seq; //initialize checksum to seq
    uint32_t leftover = 0;

    for(uint32_t i=0; i < num ; i++) {
        checksum ^= *pnum++;
        dataBuf += sizeof *pnum;
    }

    //cout << "After doing data checksum with itself cksum = " << checksum << endl;
    if( (leftover = len % sizeof(uint32_t)) ) {
        uint32_t lastnum = 0, appendlen = sizeof(uint32_t) - leftover;
        char *pp = (char *)&lastnum;

        cout << "Leftover bytes = " << leftover << endl;
        cout <<  "Not a multiple of "<<sizeof(uint32_t)<< ", need to append " << appendlen << " bytes " << endl;
        memcpy(&lastnum, dataBuf, leftover);
        pp = (char *)&lastnum +leftover;
        memset(pp, 0xab, appendlen);
        checksum ^= lastnum;
    }

    cout << "csum  = " << csum << endl;
   cout << "cksum = " << checksum << endl;
    return csum==checksum ? true : false ;
}
#endif
