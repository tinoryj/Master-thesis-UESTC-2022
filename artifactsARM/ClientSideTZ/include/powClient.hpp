#ifndef TEEDEDUP_POWCLIENT_HPP
#define TEEDEDUP_POWCLIENT_HPP
#include "configure.hpp"
#include "dataStructure.hpp"
#include "messageQueue.hpp"
#include "powTrustZone.hpp"
#include "protocol.hpp"
#include "sender.hpp"
#include <bits/stdc++.h>

class powClient {
private:
    messageQueue<Data_t>* inputMQ_;
    Sender* senderObj_;
    bool request(u_char* logicDataBatchBuffer, uint32_t bufferSize, u_char* hmac, u_char* chunkHashList, uint32_t chunkNumber);
    CryptoPrimitive* cryptoObj_;
    powTZ* powTZObj_;

public:
    powClient(Sender* senderObjTemp);
    ~powClient();
    void run();
    bool insertMQ(Data_t& newChunk);
    bool extractMQ(Data_t& newChunk);
    bool editJobDoneFlag();
};

#endif //TEEDEDUP_POWCLIENT_HPP
