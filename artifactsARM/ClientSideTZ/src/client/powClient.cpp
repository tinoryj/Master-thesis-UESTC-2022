#include "powClient.hpp"
// #include <sys/time.h>

using namespace std;

extern Configure config;

#if SYSTEM_BREAK_DOWN == 1
struct timeval timestartPowClient;
struct timeval timeendPowClient;
#endif

void PRINT_BYTE_ARRAY_POW_CLIENT(
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

powClient::powClient(Sender* senderObjTemp)
{
    inputMQ_ = new messageQueue<Data_t>;
    senderObj_ = senderObjTemp;
    powTZObj_ = new powTZ();
    cryptoObj_ = new CryptoPrimitive();
#if SYSTEM_BREAK_DOWN == 1
    long diff;
    double second;
#endif
#if SYSTEM_BREAK_DOWN == 1
    gettimeofday(&timestartPowClient, NULL);
#endif
    bool loginToServerStatus = senderObj_->sendLogInMessage(CLIENT_SET_LOGIN);
    if (loginToServerStatus) {
#if SYSTEM_DEBUG_FLAG == 1
        cerr << "PowClient : login to storage service provider success" << endl;
#endif
    } else {
        cerr << "PowClient : login to storage service provider error" << endl;
    }
    u_char powKeySig[32];
    // TODO:
    u_char* rsaEncPoWKey = (u_char*)malloc(256 * sizeof(u_char));
    size_t rsaEncPoWKeySize = 256, powKeyHMACSize = 32;
    powTZObj_->getPoWKey(config.getClientID(), rsaEncPoWKey, rsaEncPoWKeySize, powKeySig, powKeyHMACSize);
#if SYSTEM_DEBUG_FLAG == 1
    cerr << "PoWClient : get pow key done, encrypted key size = " << rsaEncPoWKeySize << endl;
    PRINT_BYTE_ARRAY_POW_CLIENT(stderr, rsaEncPoWKey, rsaEncPoWKeySize);
    PRINT_BYTE_ARRAY_POW_CLIENT(stderr, powKeySig, powKeyHMACSize);
#endif
    bool keyExchangeToServerStatus = senderObj_->sendPoWKeyGenMessage(rsaEncPoWKey, powKeySig);
    if (keyExchangeToServerStatus) {
#if SYSTEM_DEBUG_FLAG == 1
        cerr << "PowClient : pow key exchange to storage service provider success" << endl;
#endif
    } else {
        cerr << "PowClient : pow key exchange to storage service provider error" << endl;
    }
    free(rsaEncPoWKey);
#if SUPER_FEATURE_GEN_METHOD != NAN_FEATURE
    powTZObj_->prepareDetection(PREFIX_FREQUENCY_THRESHOLD, COUNT_WINDOW_SIZE);
#endif

#if SYSTEM_BREAK_DOWN == 1
    gettimeofday(&timeendPowClient, NULL);
    diff = 1000000 * (timeendPowClient.tv_sec - timestartPowClient.tv_sec) + timeendPowClient.tv_usec - timestartPowClient.tv_usec;
    second = diff / 1000000.0;
    cout << "PowClient : enclave init time = " << setprecision(8) << second << " s" << endl;
#endif
}

powClient::~powClient()
{
    powTZObj_->~powTZ();
    cerr << "delete pow TZ done" << endl;
    // senderObj_->sendLogOutMessage();
    inputMQ_->~messageQueue();
    delete inputMQ_;
    delete cryptoObj_;
}

bool powClient::editJobDoneFlag()
{
    inputMQ_->done_ = true;
    if (inputMQ_->done_) {
        return true;
    } else {
        return false;
    }
}

void powClient::run()
{
#if SYSTEM_BREAK_DOWN == 1
    double powEnclaveCaluationTime = 0;
    double powExchangeInofrmationTime = 0;
    double powBuildHashListTime = 0;
    long diff;
    double second;
#endif
    vector<Data_t> batchChunk;
    int powBatchSize = config.getPOWBatchSize();
    u_char* batchChunkLogicDataCharBuffer = (u_char*)malloc(sizeof(u_char) * (MAX_CHUNK_SIZE + sizeof(int)) * powBatchSize);
    memset(batchChunkLogicDataCharBuffer, 0, sizeof(u_char) * (MAX_CHUNK_SIZE + sizeof(int)) * powBatchSize);
    Data_t tempChunk;
    int netstatus;
    int currentBatchChunkNumber = 0;
    bool jobDoneFlag = false;
    uint32_t currentBatchSize = 0;
    batchChunk.clear();
    bool powRequestStatus = false;

    while (true) {

        if (inputMQ_->done_ && inputMQ_->isEmpty()) {
            jobDoneFlag = true;
        }
        if (extractMQ(tempChunk)) {
            if (tempChunk.dataType == DATA_TYPE_RECIPE) {
                senderObj_->insertMQ(tempChunk);
                continue;
            } else {
                batchChunk.push_back(tempChunk);
                memcpy(batchChunkLogicDataCharBuffer + currentBatchSize, &tempChunk.chunk.logicDataSize, sizeof(int));
                currentBatchSize += sizeof(int);
                memcpy(batchChunkLogicDataCharBuffer + currentBatchSize, tempChunk.chunk.logicData, tempChunk.chunk.logicDataSize);
                currentBatchSize += tempChunk.chunk.logicDataSize;
                currentBatchChunkNumber++;
            }
        }
        if (currentBatchChunkNumber == powBatchSize || jobDoneFlag) {
#if SYSTEM_BREAK_DOWN == 1
            gettimeofday(&timestartPowClient, NULL);
#endif
            u_char clientMac[32];
            u_char chunkHashList[currentBatchChunkNumber * SYSTEM_CIPHER_SIZE];
            memset(chunkHashList, 0, currentBatchChunkNumber * SYSTEM_CIPHER_SIZE);
            powRequestStatus = this->request(batchChunkLogicDataCharBuffer, currentBatchSize, clientMac, chunkHashList, currentBatchChunkNumber);
#if SYSTEM_BREAK_DOWN == 1
            gettimeofday(&timeendPowClient, NULL);
            diff = 1000000 * (timeendPowClient.tv_sec - timestartPowClient.tv_sec) + timeendPowClient.tv_usec - timestartPowClient.tv_usec;
            second = diff / 1000000.0;
            powEnclaveCaluationTime += second;
#endif
            if (!powRequestStatus) {
                cerr << "PowClient : trustzone request failed" << endl;
                break;
            } else {
#if SYSTEM_BREAK_DOWN == 1
                gettimeofday(&timestartPowClient, NULL);
#endif
                for (int i = 0; i < currentBatchChunkNumber; i++) {
                    memcpy(batchChunk[i].chunk.chunkHash, chunkHashList + i * SYSTEM_CIPHER_SIZE, SYSTEM_CIPHER_SIZE);
                }
#if SYSTEM_BREAK_DOWN == 1
                gettimeofday(&timeendPowClient, NULL);
                diff = 1000000 * (timeendPowClient.tv_sec - timestartPowClient.tv_sec) + timeendPowClient.tv_usec - timestartPowClient.tv_usec;
                second = diff / 1000000.0;
                powBuildHashListTime += second;
#endif
            }
#if SYSTEM_BREAK_DOWN == 1
            gettimeofday(&timestartPowClient, NULL);
#endif
            u_char serverResponse[sizeof(int) + sizeof(bool) * currentBatchChunkNumber];
            senderObj_->sendEnclaveSignedHash(clientMac, chunkHashList, currentBatchChunkNumber, serverResponse, netstatus);
#if SYSTEM_DEBUG_FLAG == 1
            cerr << "PowClient : send signed hash list data = " << endl;
            PRINT_BYTE_ARRAY_POW_CLIENT(stderr, chunkHashList, SYSTEM_CIPHER_SIZE * currentBatchChunkNumber);
            // PRINT_BYTE_ARRAY_POW_CLIENT(stderr, chunkHashList, currentBatchChunkNumber * SYSTEM_CIPHER_SIZE);
#endif
#if SYSTEM_BREAK_DOWN == 1
            gettimeofday(&timeendPowClient, NULL);
            diff = 1000000 * (timeendPowClient.tv_sec - timestartPowClient.tv_sec) + timeendPowClient.tv_usec - timestartPowClient.tv_usec;
            second = diff / 1000000.0;
            powExchangeInofrmationTime += second;
#endif
            if (netstatus != SUCCESS) {
                cerr << "PowClient : server pow signed hash verify error, client mac = " << endl;
                PRINT_BYTE_ARRAY_POW_CLIENT(stderr, clientMac, 32);
                cerr << "PowClient : server pow signed hash verify error, ta client hashlist = " << endl;
                PRINT_BYTE_ARRAY_POW_CLIENT(stderr, chunkHashList, SYSTEM_CIPHER_SIZE);
                break;
            } else {
                int totalNeedChunkNumber;
                memcpy(&totalNeedChunkNumber, serverResponse, sizeof(int));
                bool requiredChunksList[currentBatchChunkNumber];
                memcpy(requiredChunksList, serverResponse + sizeof(int), sizeof(bool) * currentBatchChunkNumber);
#if SYSTEM_DEBUG_FLAG == 1
                cerr << "PowClient : send pow signed hash for " << currentBatchChunkNumber << " chunks success, Server need " << totalNeedChunkNumber << " over all " << batchChunk.size() << endl;
#endif
                for (int i = 0; i < totalNeedChunkNumber; i++) {
                    if (requiredChunksList[i] == true) {
                        batchChunk[i].chunk.type = CHUNK_TYPE_NEED_UPLOAD;
                    }
                }
                int batchChunkSize = batchChunk.size();
                for (int i = 0; i < batchChunkSize; i++) {
                    senderObj_->insertMQ(batchChunk[i]);
                }
            }
            currentBatchChunkNumber = 0;
            currentBatchSize = 0;
            batchChunk.clear();
        }
        if (jobDoneFlag) {
            break;
        }
    }
    if (!senderObj_->editJobDoneFlag()) {
        cerr << "PowClient : error to set job done flag for sender" << endl;
    }
#if SYSTEM_BREAK_DOWN == 1
    cout << "PowClient : enclave compute work time = " << powEnclaveCaluationTime << " s" << endl;
    cout << "PowClient : build hash list and insert hash to chunk time = " << powBuildHashListTime << " s" << endl;
    cout << "PowClient : exchange status to storage service provider time = " << powExchangeInofrmationTime << " s" << endl;
    cout << "PowClient : Total work time = " << powExchangeInofrmationTime + powEnclaveCaluationTime + powBuildHashListTime << " s" << endl;
#endif
    free(batchChunkLogicDataCharBuffer);
    return;
}

bool powClient::request(u_char* logicDataBatchBuffer, uint32_t bufferSize, u_char* hmac, u_char* chunkHashList, uint32_t chunkNumber)
{
    // PRINT_BYTE_ARRAY_POW_CLIENT(stderr, logicDataBatchBuffer, bufferSize);
    powTZObj_->getSignedHashList(logicDataBatchBuffer, bufferSize, chunkHashList, hmac, chunkNumber);
    return true;
}

bool powClient::insertMQ(Data_t& newChunk)
{
    return inputMQ_->push(newChunk);
}

bool powClient::extractMQ(Data_t& newChunk)
{
    return inputMQ_->pop(newChunk);
}