#ifndef TEEDEDUP_ENCODER_HPP
#define TEEDEDUP_ENCODER_HPP

#include "configure.hpp"
#include "cryptoPrimitive.hpp"
#include "dataStructure.hpp"
#include "messageQueue.hpp"
#include "powClient.hpp"
#include "ssl.hpp"

class Encoder {
private:
    messageQueue<Data_t>* inputMQ_;
    powClient* powObj_;
    CryptoPrimitive* cryptoObj_;

public:
    Encoder(powClient* powObjTemp);
    ~Encoder();
    void run();
    bool encodeChunk(Data_t& newChunk);
    bool insertMQ(Data_t& newChunk);
    bool extractMQ(Data_t& newChunk);
    bool editJobDoneFlag();
};

#endif //TEEDEDUP_ENCODER_HPP
