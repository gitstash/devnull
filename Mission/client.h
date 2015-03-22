#include <netinet/in.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept> 
#include <string.h>
#include <unistd.h>
#define MAX_BUF 256

using namespace std;

class Client
{
    private:
        //has ip add port and family
        struct sockaddr_in mServer;
        int  mSockfd;
        const string mEmail;
        string mAuthCode;

        string parse(const string &msg);
        uint32_t send(string msgBuf);
        uint32_t receive(char msgBuf[], uint32_t numByte);
        bool isValidChecksum(uint32_t csum, uint32_t nwByteSeq,
                        const char *dataBuf, uint32_t datalen);
    public:
        Client(string hostname, uint16_t port, string email);
        ~Client();
        void startConnection();
        bool handshake();
        int getChunk(uint32_t *pseq, char **pChunk);
        bool saveChunk(const char *buf, uint32_t len);
};
