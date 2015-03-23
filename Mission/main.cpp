#include <cstdlib>
#include "packetlist.h"
#include "client.h"

int main(void)
{
    string chunk;
    int bytes = 0;
    uint32_t seq = 0;
    uint32_t total = 0, skip = 0, save = 0;
    string msgBuf;
    stringstream ss;
    try {
        Client c("Challenge2.airtime.com", 2323, "manishamails@gmail.com", "/tmp/data.pcm");
        Packetlist pktList;

        c.startConnection();
        if(!c.handshake()) {
            cout << "Handshake failed " << endl;
            return EXIT_FAILURE;
        }
        cout << "Connection established. " << endl;
        while( (bytes = c.getChunk(&seq, chunk)) >= 0) {
            if(!chunk.empty()) {
                cout << "Pkt #:" << seq << ", " << bytes << " bytes" << endl;
                pktList.insertPacket(seq, bytes, chunk);
                //c.saveChunk(chunk, bytes);
                chunk.clear();
                total+=bytes;
                save++;
            }else skip++;
        }

        pktList.printList();
        pktList.serializeData("/tmp/data.pcm");
    }
    catch(runtime_error &re){
        cout << "Exception: " << re.what() << endl;
    }
    cout << "Total bytes " <<  total << endl;
    cout << "Saved: " << save << ", Skipped: " << skip << " packets " <<  endl;
    if(save+skip)
        cout << "Save packet " << (save * 100)/(save+skip) << "%" << endl;

    cout<<"Writing to file \'/tmp/data.pcm\' ..." << endl;

    return EXIT_SUCCESS;
}
