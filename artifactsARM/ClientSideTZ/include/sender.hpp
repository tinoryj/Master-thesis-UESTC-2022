#ifndef TEEDEDUP_SENDER_HPP
#define TEEDEDUP_SENDER_HPP

#include "configure.hpp"
#include "cryptoPrimitive.hpp"
#include "dataStructure.hpp"
#include "messageQueue.hpp"
#include "protocol.hpp"
#include "ssl.hpp"

class Sender {
private:
    std::mutex mutexSocket_;
    ssl* powSecurityChannel_;
    ssl* dataSecurityChannel_;
    SSL* sslConnectionPow_;
    SSL* sslConnectionData_;
    int clientID_;
    messageQueue<Data_t>* inputMQ_;
    CryptoPrimitive* cryptoObj_;

public:
    Sender();

    ~Sender();

    //status define in protocol.hpp
    bool sendRecipe(Recipe_t request, RecipeList_t requestList, int& status);
    bool sendChunkList(ChunkList_t request, int& status);
    bool sendChunkList(char* requestBufferIn, int sendBufferSize, int sendChunkNumber, int& status);
    bool getKeyServerSK(u_char* SK);
    //for pow
    bool sendPoWKeyGenMessage(u_char* powKey, u_char* powKeySig);
    bool sendEnclaveSignedHash(u_char* clientMac, u_char* hashList, int requestNumber, u_char* respond, int& status);
    bool sendLogInMessage(int loginType);
    //send chunk when socket free
    void run();

    //general send data
    bool sendDataPow(char* request, int requestSize, char* respond, int& respondSize);
    bool sendEndFlag();
    bool insertMQ(Data_t& newChunk);
    bool extractMQ(Data_t& newChunk);
    bool editJobDoneFlag();
};

#endif //TEEDEDUP_SENDER_HPP
