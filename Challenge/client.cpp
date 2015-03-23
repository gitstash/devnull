#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>

#include "client.h"

Client::Client(string hostname, uint16_t port, string email):
    mEmail(email), mAuthCode("")
{
    struct hostent *server;
    stringstream errObj;
    mSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(mSockfd == -1) {
        errObj << "Error opening socket (errno " << errno << " )" << endl;
        throw runtime_error(errObj.str());
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
    mOfsream.close();
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

uint32_t Client::sendData(string msgBuf)
{
    stringstream errObj;
    ssize_t n = send(mSockfd, msgBuf.c_str(), msgBuf.size(), 0);
    if( n < 0 )
    {
        errObj << "Error writing to the socket.. (errno: " << errno << " )" << endl;
        throw runtime_error(errObj.str());
    }
    return n;
}

uint32_t Client::receiveData(string &outBuf, uint32_t numByte)
{
    stringstream errObj;
    char msgBuf[numByte];
    ssize_t n = 0;
    size_t total = 0;

    outBuf.clear();
    memset(msgBuf, 0,  numByte);

    while( (n = recv(mSockfd, msgBuf + total, numByte - total, 0)) > 0 ) {
       total += n;
       if( total == numByte )
           break;
    }

    if( n < 0 ){
        errObj << "Error recv from socket ...(errno: " << errno << " )" << endl;
        throw runtime_error(errObj.str());
    }else if(n == 0)
        cout << "\n<<<Server closed connection >>>" << endl;
    else
        outBuf.assign(msgBuf, total);

    return total;
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
    char authcode[MAX_BUF];
    int n = 0;
    stringstream ss;
    if(!recv(mSockfd, msgBuf, MAX_BUF, 0)) 
        return false;

    string token = parse(msgBuf);
    ss << "IAM:" << token << ":" << mEmail << ":at\n"; 
    sendData(ss.str());

    if(!(n = recv(mSockfd, msgBuf, MAX_BUF, 0))) 
        return false;

    if( sscanf(msgBuf, "SUCCESS:%s\n", authcode) == EOF)
        return false;

    //for future use
    mAuthCode = authcode;

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

int Client::getChunk(uint32_t *pseq, string &chunk)
{
    uint32_t seq, csum, dataLen;
    uint32_t n = 0, readLen = 0;
    string hdrBuf;
    string dataBuf;

    if(!pseq )
        return -1;

    //check if authenticated
    if(mAuthCode.empty())
        return -1;

    //read sequence num
    hdrBuf.clear();
    if(receiveData(hdrBuf, 4) < 4)
        return -1;
    memcpy(&seq, hdrBuf.c_str(), 4);
    *pseq = ntohl(seq);

    //read checksum
    hdrBuf.clear();
    if(receiveData(hdrBuf, 4) < 4)
        return -1;
    memcpy(&csum, hdrBuf.c_str(), 4);

    //data len
    hdrBuf.clear();
    if(receiveData(hdrBuf, 4) < 4)
        return -1;

    memcpy(&dataLen, hdrBuf.c_str(), 4);
    dataLen = ntohl(dataLen);

    //data
    if( (readLen = receiveData(dataBuf, dataLen)) < dataLen)
        return -1;
#if 0
    string tmpBuf;
    while(readLen < dataLen){
        n = receiveData(tmpBuf, dataLen - readLen);
        if(n <= 0)
            break;
        readLen += n;
        dataBuf.append(tmpBuf.c_str(), n);
        tmpBuf.clear();
    }

    if(readLen < dataLen)
        cout << "Pkt seq#:" << ntohl(seq) << " readLen " << readLen << " Warning: pkt receipt truncated" << endl;
#endif

    //validate checksum
    if(!isValidChecksum(csum, seq, dataBuf.c_str(), readLen)) {
#ifdef DEBUG
        cout << "Pkt seq#:" << ntohl(seq) << "*** Checksum invalid ***" << endl;
#endif
        chunk.clear();
        return 0;
    }
    chunk = dataBuf;

    return readLen;
}

#if 0
bool Client::saveChunk(string buf, uint32_t len)
{
    if(!mOfsream.is_open()){
        cout << "File not opened for writing " << endl;
        return false;
    }
    mOfsream << buf;
    return true;
}
#endif

//Calculate checksum in network-byte order
bool Client::isValidChecksum(uint32_t csum,         // incoming csum in network-byte order
                             uint32_t seq,          // pkt seq, network-byte order
                             const char *dataBuf,   // binary data 
                             uint32_t len)          // data-len, host-byte order
{
    uint32_t checksum = 0, leftover = 0, i;
    uint32_t *pn = NULL;
    uint32_t num = len / sizeof(uint32_t);

    checksum = seq;

#ifdef DEBUG
    cout << "Pkt# " << ntohl(seq) << ", data-len = " << len << ", num of while 32-bit chunks = " << num << ">>>" << endl;
#endif

    pn = (uint32_t *)dataBuf;
    for(i = 0; i < num ; i++) {
        checksum ^= *pn++;
        dataBuf += sizeof *pn;
    }

    if( (leftover = len % sizeof(uint32_t)) ) {
        char last[4];
        uint32_t append = sizeof(uint32_t) - leftover;

        memcpy(last, dataBuf, leftover);
        memset(last + leftover, 0xab, append);
        pn = (uint32_t *)last;
#ifdef DEBUG
        cout << "leftover=" << leftover << ",append=" << append << endl;
#endif
        checksum ^= *pn;
    }

#ifdef DEBUG
    cout << "csum(recvd)=" << csum << ", csum(calc)=" << checksum << endl;
#endif

    return csum == checksum;
}
