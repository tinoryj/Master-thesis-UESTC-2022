
#include <dataSR.hpp>
#include <sys/times.h>

extern Configure config;

void PRINT_BYTE_ARRAY_DATA_SR(
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

DataSR::DataSR(StorageCore* storageObj, DedupCore* dedupCoreObj, powServer* powServerObj, kmServer* kmServerObj, ssl* powSecurityChannelTemp, ssl* dataSecurityChannelTemp)
{
    restoreChunkBatchNumber_ = config.getSendChunkBatchSize();
    storageObj_ = storageObj;
    dedupCoreObj_ = dedupCoreObj;
    powServerObj_ = powServerObj;
    kmServerObj_ = kmServerObj;
    cryptoObj_ = new CryptoPrimitive();
    keyExchangeKeySetFlag_ = false;
    powSecurityChannel_ = powSecurityChannelTemp;
    dataSecurityChannel_ = dataSecurityChannelTemp;
    keyRegressionCurrentTimes_ = config.getKeyRegressionMaxTimes();
#if SYSTEM_DEBUG_FLAG == 1
    cerr << " DataSR : key regression current count = " << keyRegressionCurrentTimes_ << endl;
#endif
    // memcpy(keyExchangeKey_, keyExchangeKey, 16);
}

DataSR::~DataSR()
{
    delete cryptoObj_;
}

void DataSR::runData(SSL* sslConnection)
{
    int recvSize = 0;
    int sendSize = 0;
    char recvBuffer[NETWORK_MESSAGE_DATA_SIZE];
    char sendBuffer[NETWORK_MESSAGE_DATA_SIZE];
    // double totalSaveChunkTime = 0;
    uint32_t startID_ = 0;
    uint32_t endID_ = 0;
    Recipe_t restoredFileRecipe_;
    uint32_t totalRestoredChunkNumber_ = 0;
    char* restoredRecipeList;
    uint64_t recipeSize = 0;
#if SYSTEM_BREAK_DOWN == 1
    bool uploadFlag = false;
    struct timeval timestartDataSR;
    struct timeval timeendDataSR;
    double saveChunkTime = 0;
    double saveRecipeTime = 0;
    double restoreChunkTime = 0;
    long diff;
    double second;
#endif
    while (true) {
        if (!dataSecurityChannel_->recv(sslConnection, recvBuffer, recvSize)) {
            cout << "DataSR : client closed socket connect, thread exit now" << endl;
#if SYSTEM_BREAK_DOWN == 1
            if (uploadFlag == true) {
                cout << "DataSR : total save chunk time = " << saveChunkTime << " s" << endl;
                cout << "DataSR : total save recipe time = " << saveRecipeTime << " s" << endl;
            } else {
                cout << "DataSR : total restore chunk time = " << restoreChunkTime << " s" << endl;
            }
#endif
            cerr << "DataSR : data thread exit now due to client connection lost" << endl;
            if (restoredRecipeList != nullptr) {
                free(restoredRecipeList);
            }
            return;
        } else {
            NetworkHeadStruct_t netBody;
            memcpy(&netBody, recvBuffer, sizeof(NetworkHeadStruct_t));
#if SYSTEM_DEBUG_FLAG == 1
            cerr << "DataSR : data service recv message type " << netBody.messageType << ", message size = " << netBody.dataSize << endl;
#endif
            switch (netBody.messageType) {
            case CLIENT_EXIT: {
                netBody.messageType = SERVER_JOB_DONE_EXIT_PERMIT;
                netBody.dataSize = 0;
                sendSize = sizeof(NetworkHeadStruct_t);
                memset(sendBuffer, 0, NETWORK_MESSAGE_DATA_SIZE);
                memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                dataSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
#if SYSTEM_BREAK_DOWN == 1
                if (uploadFlag == true) {
                    cout << "DataSR : total save chunk time = " << saveChunkTime << " s" << endl;
                    cout << "DataSR : total save recipe time = " << saveRecipeTime << " s" << endl;
                } else {
                    cout << "DataSR : total restore chunk time = " << restoreChunkTime << " s" << endl;
                }
                storageObj_->clientExitSystemStatusOutput(uploadFlag);
#endif
#if SYSTEM_LOG_FLAG == 1
                cerr << "DataSR : data thread recv exit flag, thread exit now" << endl;
#endif
                if (restoredRecipeList != nullptr) {
                    free(restoredRecipeList);
                }
                return;
            }
            case CLIENT_UPLOAD_CHUNK: {
#if SYSTEM_BREAK_DOWN == 1
                uploadFlag = true;
                gettimeofday(&timestartDataSR, NULL);
#endif
                bool storeChunkStatus = storageObj_->storeChunks(netBody, (char*)recvBuffer + sizeof(NetworkHeadStruct_t));
#if SYSTEM_BREAK_DOWN == 1
                gettimeofday(&timeendDataSR, NULL);
                diff = 1000000 * (timeendDataSR.tv_sec - timestartDataSR.tv_sec) + timeendDataSR.tv_usec - timestartDataSR.tv_usec;
                second = diff / 1000000.0;
                saveChunkTime += second;
#endif
                if (!storeChunkStatus) {
                    cerr << "DedupCore : store chunks report error, server may incur internal error, thread exit" << endl;
#if SYSTEM_BREAK_DOWN == 1
                    if (uploadFlag == true) {
                        cout << "DataSR : total save chunk time = " << saveChunkTime << " s" << endl;
                        cout << "DataSR : total save recipe time = " << saveRecipeTime << " s" << endl;
                    } else {
                        cout << "DataSR : total restore chunk time = " << restoreChunkTime << " s" << endl;
                    }
                    storageObj_->clientExitSystemStatusOutput(uploadFlag);
#endif
                    cerr << "DataSR : data thread exit now due to client connection lost" << endl;
                    if (restoredRecipeList != nullptr) {
                        free(restoredRecipeList);
                    }
                    return;
                }
                break;
            }
            case CLIENT_UPLOAD_ENCRYPTED_RECIPE: {
#if SYSTEM_BREAK_DOWN == 1
                uploadFlag = true;
#endif
                int recipeListSize = netBody.dataSize;
#if SYSTEM_LOG_FLAG == 1
                cout << "DataSR : recv file recipe size = " << recipeListSize << endl;
#endif
                char* recipeListBuffer = (char*)malloc(sizeof(char) * recipeListSize + sizeof(NetworkHeadStruct_t));
                if (!dataSecurityChannel_->recv(sslConnection, recipeListBuffer, recvSize)) {
                    cout << "DataSR : client closed socket connect, recipe store failed, Thread exit now" << endl;
#if SYSTEM_BREAK_DOWN == 1
                    if (uploadFlag == true) {
                        cout << "DataSR : total save chunk time = " << saveChunkTime << " s" << endl;
                        cout << "DataSR : total save recipe time = " << saveRecipeTime << " s" << endl;
                    } else {
                        cout << "DataSR : total restore chunk time = " << restoreChunkTime << " s" << endl;
                    }
                    storageObj_->clientExitSystemStatusOutput(uploadFlag);
#endif
                    cerr << "DataSR : data thread exit now due to client connection lost" << endl;
                    if (restoredRecipeList != nullptr) {
                        free(restoredRecipeList);
                    }
                    return;
                }
                Recipe_t newFileRecipe;
                memcpy(&newFileRecipe, recipeListBuffer + sizeof(NetworkHeadStruct_t), sizeof(Recipe_t));
#if SYSTEM_BREAK_DOWN == 1
                gettimeofday(&timestartDataSR, NULL);
#endif
                storageObj_->storeRecipes((char*)newFileRecipe.fileRecipeHead.fileNameHash, (u_char*)recipeListBuffer + sizeof(NetworkHeadStruct_t), recipeListSize);
#if SYSTEM_BREAK_DOWN == 1
                gettimeofday(&timeendDataSR, NULL);
                diff = 1000000 * (timeendDataSR.tv_sec - timestartDataSR.tv_sec) + timeendDataSR.tv_usec - timestartDataSR.tv_usec;
                second = diff / 1000000.0;
                saveRecipeTime += second;
#endif
                free(recipeListBuffer);
                break;
            }
            case CLIENT_UPLOAD_DECRYPTED_RECIPE: {
                // cout << "DataSR : current recipe size = " << recipeSize << ", toatl chunk number = " << restoredFileRecipe_.fileRecipeHead.totalChunkNumber << endl;
                uint64_t decryptedRecipeListSize = 0;
                memcpy(&decryptedRecipeListSize, recvBuffer + sizeof(NetworkHeadStruct_t), sizeof(uint64_t));
                // cout << "DataSR : process recipe list size = " << decryptedRecipeListSize << endl;
                restoredRecipeList = (char*)malloc(sizeof(char) * decryptedRecipeListSize + sizeof(NetworkHeadStruct_t));
                if (dataSecurityChannel_->recv(sslConnection, restoredRecipeList, recvSize)) {
                    NetworkHeadStruct_t tempHeader;
                    memcpy(&tempHeader, restoredRecipeList, sizeof(NetworkHeadStruct_t));
                    // cout << "DataSR : CLIENT_UPLOAD_DECRYPTED_RECIPE, recv message type " << tempHeader.messageType << ", message size = " << tempHeader.dataSize << endl;
                } else {
                    cerr << "DataSR : recv decrypted file recipe error " << endl;
                }
                cerr << "DataSR : process recipe list done" << endl;
                break;
            }
            case CLIENT_DOWNLOAD_ENCRYPTED_RECIPE: {
#if MULTI_CLIENT_UPLOAD_TEST == 1
                mutexRestore_.lock();
#endif
                bool restoreRecipeSizeStatus = storageObj_->restoreRecipesSize((char*)recvBuffer + sizeof(NetworkHeadStruct_t), recipeSize);
#if MULTI_CLIENT_UPLOAD_TEST == 1
                mutexRestore_.unlock();
#endif
                if (restoreRecipeSizeStatus) {
                    netBody.messageType = SUCCESS;
                    netBody.dataSize = recipeSize;
                    sendSize = sizeof(NetworkHeadStruct_t);
                    memset(sendBuffer, 0, NETWORK_MESSAGE_DATA_SIZE);
                    memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    dataSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
                    u_char* recipeBuffer = (u_char*)malloc(sizeof(u_char) * recipeSize);
#if MULTI_CLIENT_UPLOAD_TEST == 1
                    mutexRestore_.lock();
#endif
                    storageObj_->restoreRecipes((char*)recvBuffer + sizeof(NetworkHeadStruct_t), recipeBuffer, recipeSize);
#if MULTI_CLIENT_UPLOAD_TEST == 1
                    mutexRestore_.unlock();
#endif
                    char* sendRecipeBuffer = (char*)malloc(sizeof(char) * recipeSize + sizeof(NetworkHeadStruct_t));
                    memcpy(sendRecipeBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    memcpy(sendRecipeBuffer + sizeof(NetworkHeadStruct_t), recipeBuffer, recipeSize);
                    sendSize = sizeof(NetworkHeadStruct_t) + recipeSize;
                    dataSecurityChannel_->send(sslConnection, sendRecipeBuffer, sendSize);
                    memcpy(&restoredFileRecipe_, recipeBuffer, sizeof(Recipe_t));
#if SYSTEM_DEBUG_FLAG == 1
                    cerr << "StorageCore : send encrypted recipe list done, file size = " << restoredFileRecipe_.fileRecipeHead.fileSize << ", total chunk number = " << restoredFileRecipe_.fileRecipeHead.totalChunkNumber << endl;
#endif
                    free(sendRecipeBuffer);
                    free(recipeBuffer);
                } else {
                    netBody.messageType = ERROR_FILE_NOT_EXIST;
                    netBody.dataSize = 0;
                    memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    sendSize = sizeof(NetworkHeadStruct_t);
                    dataSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
                }
                break;
            }
            case CLIENT_DOWNLOAD_CHUNK_WITH_RECIPE: {
                cerr << "DataSR : start retrive chunks, chunk number = " << restoredFileRecipe_.fileRecipeHead.totalChunkNumber << endl;
                if (restoredFileRecipe_.fileRecipeHead.totalChunkNumber < restoreChunkBatchNumber_) {
                    endID_ = restoredFileRecipe_.fileRecipeHead.totalChunkNumber - 1;
                }
                while (totalRestoredChunkNumber_ != restoredFileRecipe_.fileRecipeHead.totalChunkNumber) {
                    memset(sendBuffer, 0, NETWORK_MESSAGE_DATA_SIZE);
                    int restoredChunkNumber = 0, restoredChunkSize = 0;
#if SYSTEM_BREAK_DOWN == 1
                    gettimeofday(&timestartDataSR, NULL);
#endif
#if MULTI_CLIENT_UPLOAD_TEST == 1
                    mutexRestore_.lock();
#endif
                    bool restoreChunkStatus = storageObj_->restoreRecipeAndChunk(restoredRecipeList + sizeof(NetworkHeadStruct_t) + startID_ * (SYSTEM_CIPHER_SIZE + sizeof(int)), startID_, endID_, sendBuffer + sizeof(NetworkHeadStruct_t) + sizeof(int), restoredChunkNumber, restoredChunkSize);
#if MULTI_CLIENT_UPLOAD_TEST == 1
                    mutexRestore_.unlock();
#endif
                    if (restoreChunkStatus) {
                        netBody.messageType = SUCCESS;
                        memcpy(sendBuffer + sizeof(NetworkHeadStruct_t), &restoredChunkNumber, sizeof(int));
                        netBody.dataSize = sizeof(int) + restoredChunkSize;
                        memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                        sendSize = sizeof(NetworkHeadStruct_t) + sizeof(int) + restoredChunkSize;
                        totalRestoredChunkNumber_ += restoredChunkNumber;
                        startID_ = endID_;
                        uint32_t remainChunkNumber = restoredFileRecipe_.fileRecipeHead.totalChunkNumber - totalRestoredChunkNumber_;
                        // cout << "DataSR : wait for restore chunk number = " << remainChunkNumber << ", current restored chunk number = " << restoredChunkNumber << endl;
                        if (remainChunkNumber < restoreChunkBatchNumber_) {
                            endID_ += restoredFileRecipe_.fileRecipeHead.totalChunkNumber - totalRestoredChunkNumber_;
                        } else {
                            endID_ += restoreChunkBatchNumber_;
                        }
                    } else {
                        netBody.dataSize = 0;
                        netBody.messageType = ERROR_CHUNK_NOT_EXIST;
                        memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                        sendSize = sizeof(NetworkHeadStruct_t);
#if SYSTEM_BREAK_DOWN == 1
                        if (uploadFlag == true) {
                            cout << "DataSR : total save chunk time = " << saveChunkTime << " s" << endl;
                            cout << "DataSR : total save recipe time = " << saveRecipeTime << " s" << endl;
                        } else {
                            cout << "DataSR : total restore chunk time = " << restoreChunkTime << " s" << endl;
                        }
                        storageObj_->clientExitSystemStatusOutput(uploadFlag);
#endif
                        cerr << "DataSR : data thread exit now due to client connection lost" << endl;
                        if (restoredRecipeList != nullptr) {
                            free(restoredRecipeList);
                        }
                        return;
                    }
#if SYSTEM_BREAK_DOWN == 1
                    gettimeofday(&timeendDataSR, NULL);
                    diff = 1000000 * (timeendDataSR.tv_sec - timestartDataSR.tv_sec) + timeendDataSR.tv_usec - timestartDataSR.tv_usec;
                    second = diff / 1000000.0;
                    restoreChunkTime += second;
#endif
                    dataSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
                    cerr << "DataSR : send back chunks last ID = " << startID_ << endl;
                    // cerr << "DataSR : new start ID = " << startID_ << ", end ID = " << endID_ << endl;
                }
                break;
            }
            default:
                continue;
            }
        }
    }
    cerr << "DataSR : data thread exit now due to client connection lost" << endl;
    if (restoredRecipeList != nullptr) {
        free(restoredRecipeList);
    }
    return;
}

void DataSR::runPow(SSL* sslConnection)
{
    int recvSize = 0;
    int sendSize = 0;
    char recvBuffer[NETWORK_MESSAGE_DATA_SIZE];
    char sendBuffer[NETWORK_MESSAGE_DATA_SIZE];
    int clientID = -1;
    TrustZoneSession* currentSession = new TrustZoneSession;
#if SYSTEM_BREAK_DOWN == 1
    struct timeval timestartDataSR;
    struct timeval timeendDataSR;
    double verifyTime = 0;
    double dedupTime = 0;
    long diff;
    double second;
#endif
    while (true) {

        if (!powSecurityChannel_->recv(sslConnection, recvBuffer, recvSize)) {
            cout << "DataSR : client closed socket connect, Client ID = " << clientID << endl;
#if SYSTEM_BREAK_DOWN == 1
            cout << "DataSR : total pow Verify time = " << verifyTime << " s" << endl;
            cout << "DataSR : total deduplication query time = " << dedupTime << " s" << endl;
#endif
            return;
        } else {
            NetworkHeadStruct_t netBody;
            memcpy(&netBody, recvBuffer, sizeof(NetworkHeadStruct_t));
#if SYSTEM_DEBUG_FLAG == 1
            cerr << "DataSR : pow service recv message type " << netBody.messageType << ", message size = " << netBody.dataSize << endl;
#endif
            switch (netBody.messageType) {
            case CLIENT_EXIT: {
                delete currentSession;
                netBody.messageType = SERVER_JOB_DONE_EXIT_PERMIT;
                netBody.dataSize = 0;
                sendSize = sizeof(NetworkHeadStruct_t);
                memset(sendBuffer, 0, NETWORK_MESSAGE_DATA_SIZE);
                memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                powSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
#if SYSTEM_BREAK_DOWN == 1
                cout << "DataSR : total pow Verify time = " << verifyTime << " s" << endl;
                cout << "DataSR : total deduplication query time = " << dedupTime << " s" << endl;
#endif
#if SYSTEM_LOG_FLAG == 1
                cerr << "DataSR : pow thread recv exit flag, exit now" << endl;
#endif
                return;
            }
            case POW_THREAD_DOWNLOAD: {
#if SYSTEM_LOG_FLAG == 1
                cerr << "DataSR : client download data, pow thread exit now" << endl;
#endif
                return;
            }
            case CLIENT_SET_LOGIN: {
                clientID = netBody.clientID;
#if SYSTEM_LOG_FLAG == 1
                cout << "DataSR : client send login message, loading session" << endl;
#endif
                cout << "DataSR : connected client ID = " << clientID << endl;
                netBody.messageType = SUCCESS;
                netBody.dataSize = 0;
                memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                sendSize = sizeof(NetworkHeadStruct_t);
                powSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
                continue;
            }
            case CLIENT_GET_KEY_SERVER_SK: {
                if (keyExchangeKeySetFlag_ == true) {
                    netBody.messageType = SUCCESS;
                    netBody.dataSize = SYSTEM_CIPHER_SIZE;
                    memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    memcpy(sendBuffer + sizeof(NetworkHeadStruct_t), keyExchangeKey_, SYSTEM_CIPHER_SIZE);
                    sendSize = sizeof(NetworkHeadStruct_t) + SYSTEM_CIPHER_SIZE;
                } else {
                    netBody.messageType = ERROR_CLOSE;
                    netBody.dataSize = 0;
                    memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    sendSize = sizeof(NetworkHeadStruct_t);
                }
                powSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
                break;
            }
            case POW_KEY_GEN: {
                cerr << "PowServer : recv pow key exchange message" << endl;
                u_char powKeyTemp[256];
                u_char powKeySigTemp[32];
                memcpy(powKeyTemp, recvBuffer + sizeof(NetworkHeadStruct_t), 256);
                memcpy(powKeySigTemp, recvBuffer + sizeof(NetworkHeadStruct_t) + 256, 32);
#if MULTI_CLIENT_UPLOAD_TEST == 1
                mutexSessions_.lock();
#endif
                if (!powServerObj_->powkeyGen(currentSession, clientID, powKeyTemp, powKeySigTemp)) {
                    cerr << "PowServer : error pow key generate" << endl;
                    netBody.messageType = ERROR_RESEND;
                    netBody.dataSize = 0;
                    memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    sendSize = sizeof(NetworkHeadStruct_t);
                } else {
                    netBody.messageType = SUCCESS;
                    netBody.dataSize = 0;
                    memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    sendSize = sizeof(NetworkHeadStruct_t);
                }
#if MULTI_CLIENT_UPLOAD_TEST == 1
                mutexSessions_.unlock();
#endif
                powSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
                break;
            }
            case SGX_SIGNED_HASH: {
                u_char clientMac[32];
                memcpy(clientMac, recvBuffer + sizeof(NetworkHeadStruct_t), sizeof(uint8_t) * 32);
                int signedHashSize = netBody.dataSize - sizeof(uint8_t) * 32;
                int signedHashNumber = signedHashSize / SYSTEM_CIPHER_SIZE;
                u_char hashList[signedHashSize];
                memcpy(hashList, recvBuffer + sizeof(NetworkHeadStruct_t) + sizeof(uint8_t) * 32, signedHashSize);
                if (currentSession == nullptr || !currentSession->enclaveTrusted) {
                    cerr << "PowServer : client not trusted yet" << endl;
                    netBody.messageType = ERROR_CLOSE;
                    netBody.dataSize = 0;
                    memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                    sendSize = sizeof(NetworkHeadStruct_t);
                } else {
#if SYSTEM_BREAK_DOWN == 1
                    gettimeofday(&timestartDataSR, NULL);
#endif
#if MULTI_CLIENT_UPLOAD_TEST == 1
                    mutexCrypto_.lock();
#endif
                    bool powVerifyStatus = powServerObj_->process_signedHash(currentSession, clientMac, hashList, signedHashNumber);
#if MULTI_CLIENT_UPLOAD_TEST == 1
                    mutexCrypto_.unlock();
#endif
#if SYSTEM_BREAK_DOWN == 1
                    gettimeofday(&timeendDataSR, NULL);
                    diff = 1000000 * (timeendDataSR.tv_sec - timestartDataSR.tv_sec) + timeendDataSR.tv_usec - timestartDataSR.tv_usec;
                    second = diff / 1000000.0;
                    verifyTime += second;
#endif
                    if (powVerifyStatus) {
                        bool requiredChunkTemp[signedHashNumber];
                        int requiredChunkNumber = 0;
#if SYSTEM_BREAK_DOWN == 1
                        gettimeofday(&timestartDataSR, NULL);
#endif
                        bool dedupQueryStatus = dedupCoreObj_->dedupByHash(hashList, signedHashNumber, requiredChunkTemp, requiredChunkNumber);
#if SYSTEM_BREAK_DOWN == 1
                        gettimeofday(&timeendDataSR, NULL);
                        diff = 1000000 * (timeendDataSR.tv_sec - timestartDataSR.tv_sec) + timeendDataSR.tv_usec - timestartDataSR.tv_usec;
                        second = diff / 1000000.0;
                        dedupTime += second;
#endif
                        if (dedupQueryStatus) {
                            netBody.messageType = SUCCESS;
                            netBody.dataSize = sizeof(int) + sizeof(bool) * signedHashNumber;
                            memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                            memcpy(sendBuffer + sizeof(NetworkHeadStruct_t), &requiredChunkNumber, sizeof(int));
                            memcpy(sendBuffer + sizeof(NetworkHeadStruct_t) + sizeof(int), requiredChunkTemp, signedHashNumber * sizeof(bool));
                            sendSize = sizeof(NetworkHeadStruct_t) + netBody.dataSize;
                        } else {
                            cerr << "DedupCore : recv sgx signed hash success, dedup stage report error" << endl;
                            netBody.messageType = ERROR_RESEND;
                            netBody.dataSize = 0;
                            memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                            sendSize = sizeof(NetworkHeadStruct_t);
                        }
                    } else {
                        netBody.messageType = ERROR_RESEND;
                        netBody.dataSize = 0;
                        memcpy(sendBuffer, &netBody, sizeof(NetworkHeadStruct_t));
                        sendSize = sizeof(NetworkHeadStruct_t);
                    }
                    powSecurityChannel_->send(sslConnection, sendBuffer, sendSize);
                    break;
                }
            }
            default:
                continue;
            }
        }
    }
    return;
}

void DataSR::runKeyServerSessionKeyUpdate()
{
#if SYSTEM_BREAK_DOWN == 1
    struct timeval timestart;
    struct timeval timeend;
    long diff;
    double second;
#endif
    while (true) {
        if (!keyExchangeKeySetFlag_) {
            cerr << "";
            continue;
        }
        if (keyServerSession_ != nullptr) {
            time_t timep;
            time(&timep);
#if SYSTEM_LOG_FLAG == 1
            cout << "DataSR : start key server session key update, current time = " << endl;
            cout << asctime(gmtime(&timep));
#endif
            keyExchangeKeySetFlag_ = false;
#if SYSTEM_BREAK_DOWN == 1
            gettimeofday(&timestart, 0);
#endif
            u_char hashDataTemp[32];
            u_char hashResultTemp[SYSTEM_CIPHER_SIZE];
            memcpy(hashDataTemp, keyServerSession_->sk, 16);
            memcpy(hashDataTemp + 16, keyServerSession_->mk, 16);
            for (auto i = 0; i < keyRegressionCurrentTimes_; i++) {
                cryptoObj_->generateHash(hashDataTemp, 32, hashResultTemp);
                memcpy(hashDataTemp, hashResultTemp, SYSTEM_CIPHER_SIZE);
            }
            u_char finalHashBuffer[8 + SYSTEM_CIPHER_SIZE];
            memset(finalHashBuffer, 0, 8 + SYSTEM_CIPHER_SIZE);
            memcpy(finalHashBuffer + 8, hashResultTemp, SYSTEM_CIPHER_SIZE);
            cryptoObj_->generateHash(finalHashBuffer, 8 + SYSTEM_CIPHER_SIZE, hashResultTemp);
            memcpy(keyExchangeKey_, hashResultTemp, SYSTEM_CIPHER_SIZE);
#if SYSTEM_BREAK_DOWN == 1
            gettimeofday(&timeend, 0);
            diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
            second = diff / 1000000.0;
#endif
#if SYSTEM_DEBUG_FLAG == 1
            cerr << "DataSR : key server current session key = " << endl;
            PRINT_BYTE_ARRAY_DATA_SR(stderr, keyExchangeKey_, SYSTEM_CIPHER_SIZE);
            cerr << "DataSR : key server original session key = " << endl;
            PRINT_BYTE_ARRAY_DATA_SR(stderr, keyServerSession_->sk, 16);
            cerr << "DataSR : key server original mac key = " << endl;
            PRINT_BYTE_ARRAY_DATA_SR(stderr, keyServerSession_->mk, 16);
#endif
            keyExchangeKeySetFlag_ = true;
#if SYSTEM_LOG_FLAG == 1
            cerr << "DataSR : keyServer session key update done, current regression counter = " << keyRegressionCurrentTimes_ << endl;
#endif
#if SYSTEM_BREAK_DOWN == 1
            cout << "DataSR : session key update time = " << second << " s, current regression counter = " << keyRegressionCurrentTimes_ << endl;
#endif
            keyRegressionCurrentTimes_--;
            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC_);
            xt.sec += config.getKeyRegressionIntervals();
            boost::thread::sleep(xt);
        }
    }
    return;
}

void DataSR::runKeyServerRemoteAttestation()
{
    ssl* sslRAListen = new ssl(config.getStorageServerIP(), config.getKMServerPort(), SERVERSIDE);
#if SYSTEM_DEBUG_FLAG == 1
    cerr << "DataSR : key server ra request channel setup" << endl;
#endif
    int sendSize = sizeof(NetworkHeadStruct_t);
    char sendBuffer[sendSize];
    NetworkHeadStruct_t netHead, recvHead;
    netHead.messageType = RA_REQUEST;
    netHead.dataSize = 0;
    memcpy(sendBuffer, &netHead, sizeof(NetworkHeadStruct_t));
    while (true) {
        SSL* sslRAListenConnection = sslRAListen->sslListen().second;
#if SYSTEM_LOG_FLAG == 1
        cout << "DataSR : key server connected" << endl;
#endif
        char recvBuffer[sizeof(NetworkHeadStruct_t)];
        int recvSize = -1;
    reTry:
        bool recvStatus = sslRAListen->recv(sslRAListenConnection, recvBuffer, recvSize);
        if (recvStatus == false) {
            cerr << "DataSR : key enclave ra recv error, retrying..." << endl;
            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC_);
            xt.sec += 1;
            boost::thread::sleep(xt);
            goto reTry;
        }
        memcpy(&recvHead, recvBuffer, sizeof(NetworkHeadStruct_t));
#if SYSTEM_DEBUG_FLAG == 1
        cerr << "Recv size = " << recvSize << ", content = " << endl;
        cerr << "DataSR : key server message type = " << recvHead.messageType << endl;
        PRINT_BYTE_ARRAY_DATA_SR(stderr, recvBuffer, recvSize);
#endif
        if (recvHead.messageType == KEY_SERVER_RA_REQUES) {

            time_t timep;
            time(&timep);
#if SYSTEM_LOG_FLAG == 1
            cout << "DataSR : key server start remote attestation now, current time = " << endl;
            cout << asctime(gmtime(&timep));
#endif
            keyServerSession_ = kmServerObj_->authkm(sslRAListen, sslRAListenConnection);
            if (keyServerSession_ != nullptr) {
#if SYSTEM_LOG_FLAG == 1
                cout << "DataSR : keyServer enclave trusted" << endl;
#endif
                keyExchangeKeySetFlag_ = true;
                // delete sslRAListenConnection
                free(sslRAListenConnection);
                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC_);
                xt.sec += config.getRASessionKeylifeSpan();
                boost::thread::sleep(xt);
                keyExchangeKeySetFlag_ = false;
                memset(keyExchangeKey_, 0, SYSTEM_CIPHER_SIZE);
                ssl* sslRARequest = new ssl(config.getKeyServerIP(), config.getkeyServerRArequestPort(), CLIENTSIDE);
                SSL* sslRARequestConnection = sslRARequest->sslConnect().second;
                sslRARequest->send(sslRARequestConnection, sendBuffer, sendSize);
                // delete sslRARequest;
                free(sslRARequestConnection);
            } else {
                cerr << "DataSR : keyServer send wrong message, storage try again now" << endl;
                continue;
            }
        } else {
            cerr << "DataSR : keyServer enclave not trusted, storage try again now, request type = " << recvHead.messageType << endl;
            continue;
        }
    }
    return;
}
