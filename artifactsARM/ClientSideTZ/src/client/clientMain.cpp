#include "chunker.hpp"
#include "configure.hpp"
#include "encoder.hpp"
#include "fingerprinter.hpp"
#include "keyClient.hpp"
#include "powClient.hpp"
#include "recvDecode.hpp"
#include "retriever.hpp"
#include "sender.hpp"
#include "sys/time.h"
#include <bits/stdc++.h>
#include <boost/thread/thread.hpp>
#include <signal.h>

using namespace std;

Configure config("config.json");
Chunker* chunkerObj;
Fingerprinter* fingerprinterObj;
KeyClient* keyClientObj;
Encoder* encoderObj;
powClient* powClientObj;
Sender* senderObj;
RecvDecode* recvDecodeObj;
Retriever* retrieverObj;

struct timeval timestart;
struct timeval timeend;
struct timeval timestartBreakDown;
struct timeval timeendBreakDown;

void PRINT_BYTE_ARRAY_CLIENT_MAIN(
    FILE* file, void* mem, uint32_t len)
{
    if (!mem || !len) {
        fprintf(file, "\n( null )\n");
        return;
    }
    uint8_t* array = (uint8_t*)mem;
    fprintf(file, "%u bytes:\n{\n", len);
    uint32_t i = 0;
    for (i = 0; i < len - 1; i++) {
        fprintf(file, "0x%x, ", array[i]);
        if (i % 8 == 7)
            fprintf(file, "\n");
    }
    fprintf(file, "0x%x ", array[i]);
    fprintf(file, "\n}\n");
}

void CTRLC(int s)
{
    cerr << "Client exit with keyboard interrupt" << endl;
    exit(0);
    if (chunkerObj != nullptr) {
        delete chunkerObj;
    }
    if (fingerprinterObj != nullptr) {
        delete fingerprinterObj;
    }
    if (keyClientObj != nullptr) {
        delete keyClientObj;
    }
    if (encoderObj != nullptr) {
        delete encoderObj;
    }
    if (powClientObj != nullptr) {
        delete powClientObj;
    }
    if (senderObj != nullptr) {
        delete senderObj;
    }
    if (recvDecodeObj != nullptr) {
        delete recvDecodeObj;
    }
    if (retrieverObj != nullptr) {
        delete retrieverObj;
    }
    exit(0);
}

void usage()
{
    cout << "[client -s filename] for send file" << endl;
    cout << "[client -r filename] for receive file" << endl;
}

int main(int argv, char* argc[])
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);
    sa.sa_handler = CTRLC;
    sigaction(SIGKILL, &sa, 0);
    sigaction(SIGINT, &sa, 0);

    long diff;
    double second;

    if (argv != 2 && argv != 3 && argv != 4 && argv != 5) {
        usage();
        return 0;
    }
    if (strcmp("-r", argc[1]) == 0) {
        vector<boost::thread*> thList;
        boost::thread* th;
        boost::thread::attributes attrs;
        attrs.set_stack_size(1000 * 1024 * 1024);
        gettimeofday(&timestart, NULL);
        string fileName(argc[2]);
        recvDecodeObj = new RecvDecode(fileName);
        retrieverObj = new Retriever(fileName, recvDecodeObj);
        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;
        cout << "System : Init download time is " << setprecision(8) << second << " s" << endl;

        gettimeofday(&timestart, NULL);
        // start recv data & decrypt thread
        th = new boost::thread(attrs, boost::bind(&RecvDecode::run, recvDecodeObj));
        thList.push_back(th);
        // start write file thread
        th = new boost::thread(attrs, boost::bind(&Retriever::run, retrieverObj));
        thList.push_back(th);

        for (auto it : thList) {
            it->join();
        }

        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;

        delete recvDecodeObj;
        delete retrieverObj;

        cout << "System : total work time is " << setprecision(8) << second << " s" << endl;
#if MULTI_CLIENT_UPLOAD_TEST == 1
        cout << "System : start work time is " << timestart.tv_sec << " s, " << timestart.tv_usec << " us" << endl;
        cout << "System : finish work time is " << timeend.tv_sec << " s, " << timeend.tv_usec << " us" << endl;
#endif
        cout << endl;
        return 0;

    } else if (strcmp("-s", argc[1]) == 0) {
        vector<boost::thread*> thList;
        boost::thread* th;
        boost::thread::attributes attrs;
        attrs.set_stack_size(200 * 1024 * 1024);

        gettimeofday(&timestart, NULL);

        senderObj = new Sender();
        powClientObj = new powClient(senderObj);
#if SYSTEM_BREAK_DOWN == 1
        gettimeofday(&timestartBreakDown, NULL);
#endif
        u_char sessionKey[SYSTEM_CIPHER_SIZE];
        if (!senderObj->getKeyServerSK(sessionKey)) {
            cerr << "Client : get key server session key failed" << endl;
            delete senderObj;
            delete powClientObj;
            return 0;
        }
#if SYSTEM_BREAK_DOWN == 1
        gettimeofday(&timeendBreakDown, NULL);
        diff = 1000000 * (timeendBreakDown.tv_sec - timestartBreakDown.tv_sec) + timeendBreakDown.tv_usec - timestartBreakDown.tv_usec;
        second = diff / 1000000.0;
        cout << "System : download key exchange session key time is " << setprecision(8) << second << " s" << endl;
#endif
#if SYSTEM_DEBUG_FLAG == 1
        cerr << "System : recved key enclave session key = " << endl;
        PRINT_BYTE_ARRAY_CLIENT_MAIN(stderr, sessionKey, 32);
#endif
        encoderObj = new Encoder(powClientObj);
        keyClientObj = new KeyClient(encoderObj, sessionKey);
#if SUPER_FEATURE_GEN_METHOD != NAN_FEATURE
        fingerprinterObj = new Fingerprinter(keyClientObj);
        string inputFile(argc[2]);
        chunkerObj = new Chunker(inputFile, fingerprinterObj);
#else
        string inputFile(argc[2]);
        chunkerObj = new Chunker(inputFile, keyClientObj);
#endif
        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;
        cout << "System : Init upload time is " << setprecision(8) << second << " s" << endl;

#if MULTI_CLIENT_UPLOAD_TEST == 1
        cerr << "System : input sync number for multi client test" << endl;
        int inputNumberUsedForSyncInMultiClientTest = 0;
        cin >> inputNumberUsedForSyncInMultiClientTest;
#endif

        gettimeofday(&timestart, NULL);
        //start chunking thread
        th = new boost::thread(attrs, boost::bind(&Chunker::chunking, chunkerObj));
        thList.push_back(th);
#if SUPER_FEATURE_GEN_METHOD != NAN_FEATURE
        //start fingerprinting thread
        for (int i = 0; i < config.getKeySeedGenFeatureThreadNumber(); i++) {
            th = new boost::thread(attrs, boost::bind(&Fingerprinter::runFingerprintGen, fingerprinterObj, i));
            thList.push_back(th);
        }
#endif

        //start key client thread
        th = new boost::thread(attrs, boost::bind(&KeyClient::run, keyClientObj));
        thList.push_back(th);

        //start encoder thread
        th = new boost::thread(attrs, boost::bind(&Encoder::run, encoderObj));
        thList.push_back(th);

        //start pow thread
        th = new boost::thread(attrs, boost::bind(&powClient::run, powClientObj));
        thList.push_back(th);

        //start sender thread
        th = new boost::thread(attrs, boost::bind(&Sender::run, senderObj));
        thList.push_back(th);

        for (auto it : thList) {
            it->join();
        }
        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;
        delete senderObj;
        delete powClientObj;
        delete keyClientObj;
#if SUPER_FEATURE_GEN_METHOD != NAN_FEATURE
        delete fingerprinterObj;
#endif
        delete chunkerObj;

        cout << "System : upload total work time is " << setprecision(8) << second << " s" << endl;
#if MULTI_CLIENT_UPLOAD_TEST == 1
        cout << "System : start work time is " << timestart.tv_sec << " s, " << timestart.tv_usec << " us" << endl;
        cout << "System : finish work time is " << timeend.tv_sec << " s, " << timeend.tv_usec << " us" << endl;
#endif
        cout << endl;
        return 0;

    } else {
        usage();
        return 0;
    }
}
