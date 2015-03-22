#include "client.h"
#include <cstdlib>

string parse(const string &msg)
{
    uint32_t first = msg.find(":");
    uint32_t last = msg.find("\n");
    string challengeNum = msg.substr (first,last-first);
    cout << "Challenge num " << challengeNum << endl;
    return challengeNum;
}
int main(void)
{
    char *pChunk = NULL;
    int bytes = 0;
    uint32_t seq = 0;
    string msgBuf;
    stringstream ss;
    try {
        Client c("Challenge2.airtime.com", 2323, "manishamails@gmail.com");
        c.startConnection();
        if(!c.handshake()) {
            cout << "Handshake failed " << endl;
            return EXIT_FAILURE;
        }
        cout << "Connection established. " << endl;
        while( (bytes = c.getChunk(&seq, &pChunk)) >= 0) {
            if(pChunk) {
                //delete[] pChunk;
                free (pChunk);
            cout << "Pkt #:" << seq << ", " << bytes << " bytes" << endl;
                c.saveChunk(pChunk+44, bytes-44);
                pChunk = NULL;
            }
        }
    }
    catch(runtime_error &re){
        cout << "Exception: " << re.what() << endl;
    }

    return EXIT_SUCCESS;
}

