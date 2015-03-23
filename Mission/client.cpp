#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>

#include "client.h"

Client::Client(string hostname, uint16_t port, string email, string outfile):
    mEmail(email), mAuthCode(""), mOutFilePath(outfile)
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

    //open file for saving the chunk in case of valid data.
    mOfsream.open(mOutFilePath.c_str(), ios::out | ios::binary); 
    if (!mOfsream.is_open()) { 
        errObj << "Error opening outfile " << endl;
        throw runtime_error(errObj.str());
    }
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

    outBuf.clear();
    char msgBuf[numByte];
    memset(msgBuf, 0,  numByte);

    ssize_t n = recv(mSockfd, msgBuf, numByte, 0);
    if( n < 0 ){
        errObj << "Error reading ...(errno: " << errno << " )" << endl;
        throw runtime_error(errObj.str());
    }else if(n == 0)
        cout << "\n<<<Server terminated connection >>>" << endl;
    else
        outBuf.assign(msgBuf, n);
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
    //char msgBuf[MAX_BUF];
    string msgBuf;
    stringstream ss;
    if(!receiveData(msgBuf, MAX_BUF)) { 
        return false;
    }

    string token = parse(msgBuf);
    ss << "IAM:" << token << ":" << mEmail << ":at\n"; 
    sendData(ss.str());
    if(!receiveData(msgBuf, MAX_BUF))
        return false;

    //check if success
    //if(strstr(msgBuf, "SUCCESS") == NULL)
    //    return false;
    if (msgBuf.compare(0, 7, "SUCCESS") != 0)
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

int Client::getChunk(uint32_t *pseq, string &chunk)
{
    uint32_t seq, csum, dataLen;
    //char hdrBuf[4];
    string hdrBuf;

    //char *dataBuf;
    string dataBuf;
    uint32_t n = 0;
    uint32_t readLen = 0;

    if(!pseq )
        return -1;

    //check if authenticated
    if(mAuthCode.empty())
        return -1;

    //read sequence num
    if(receiveData(hdrBuf, 4) < 4)
        return -1;
    memcpy(&seq, hdrBuf.c_str(), 4);
    *pseq = ntohl(seq);

    //read checksum
    //memset(hdrBuf, 0, sizeof hdrBuf);
    if(receiveData(hdrBuf, 4) < 4)
        return -1;
    memcpy(&csum, hdrBuf.c_str(), 4);

    //data len
    //memset(hdrBuf, 0, sizeof hdrBuf);
    if(receiveData(hdrBuf, 4) < 4)
        return -1;

    memcpy(&dataLen, hdrBuf.c_str(), 4);
    dataLen = ntohl(dataLen);
    //cout << "-------Received len-------" << endl;
    //cout << dataLen <<  endl;

    //data
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

    //validate checksum
    if(isValidChecksum(csum, seq, dataBuf.c_str(), readLen)) {
        //cout << "Pkt seq#:" << ntohl(seq) << " Valid checksum " << endl;
        //chunk.clear();
        //return 0;
    } else {
        //cout << "Pkt seq#:" << ntohl(seq) << " Checksum invalid" << endl;
        chunk.clear();
        return 0;
    }
    chunk = dataBuf;
    return readLen;
}

bool Client::saveChunk(string buf, uint32_t len)
{
#if 0
    FILE *fp = NULL;
    if( !(fp = fopen("/tmp/data.pcm", "w")) ) {
        cout << "File open failed" << endl;
        return false;
    }

    if(fwrite(buf.c_str(), 1, len, fp) != len) {
        cout << "Truncated output file: /tmp/data.pcm" << endl;
    }
    fclose(fp);
    return true;
#endif
    if(!mOfsream.is_open()){
        cout << "File not opened for writing " << endl;
        return false;
    }
    mOfsream << buf;
    return true;
}

//Calculate checksum in network-byte order
bool Client::isValidChecksum(uint32_t csum, uint32_t seq,
        const char *dataBuf, uint32_t len)
{
    uint32_t checksum = 0, leftover = 0, i;
    uint32_t *pn = NULL;
    uint32_t num = len / sizeof(uint32_t);

    checksum = seq;

#ifdef DEBUG
    printf("<<<data-len = %d. No of whole 32-bit chunks: %u>>>\n", len, num);
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
        printf("<<<leftover=%u,append=%u\n", leftover, append);
#endif
        checksum ^= *pn;
    }

#ifdef DEBUG
    printf("Pkt csum(BE)=%#x, calc=%#x\n", csum, checksum);
#endif
    return csum==checksum;
}
