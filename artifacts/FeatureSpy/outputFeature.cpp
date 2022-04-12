#include "chunker.hpp"
#include "featureExtraction.hpp"
#include <bits/stdc++.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sys/time.h>
using namespace std;

#define PREFIX_UNIT 16 // block unit

bool encryptWithKey(u_char* dataBuffer, const int dataSize, u_char* key, u_char* ciphertext)
{
    EVP_CIPHER_CTX* cipherctx_;
    u_char iv_[16];
    cipherctx_ = EVP_CIPHER_CTX_new();
    if (cipherctx_ == nullptr) {
        cerr << "can not initial cipher ctx" << endl;
    }
    EVP_CIPHER_CTX_init(cipherctx_);
    memset(iv_, 0, 16);

    int cipherlen, len;
    EVP_CIPHER_CTX_set_padding(cipherctx_, 0);
    if (EVP_EncryptInit_ex(cipherctx_, EVP_aes_256_cfb(), nullptr, key, iv_) != 1) {
        cerr << "encrypt error\n";
        EVP_CIPHER_CTX_reset(cipherctx_);
        EVP_CIPHER_CTX_cleanup(cipherctx_);
        return false;
    }
    if (EVP_EncryptUpdate(cipherctx_, ciphertext, &cipherlen, dataBuffer, dataSize) != 1) {
        cerr << "encrypt error\n";
        EVP_CIPHER_CTX_reset(cipherctx_);
        EVP_CIPHER_CTX_cleanup(cipherctx_);
        return false;
    }
    if (EVP_EncryptFinal_ex(cipherctx_, ciphertext + cipherlen, &len) != 1) {
        cerr << "encrypt error\n";
        EVP_CIPHER_CTX_reset(cipherctx_);
        EVP_CIPHER_CTX_cleanup(cipherctx_);
        return false;
    }
    cipherlen += len;
    if (cipherlen != dataSize) {
        cerr << "CryptoPrimitive : encrypt output size not equal to origin size" << endl;
        EVP_CIPHER_CTX_reset(cipherctx_);
        EVP_CIPHER_CTX_cleanup(cipherctx_);
        return false;
    }
    EVP_CIPHER_CTX_reset(cipherctx_);
    EVP_CIPHER_CTX_cleanup(cipherctx_);
    return true;
}

int main(int argv, char* argc[])
{
    struct timeval timestart;
    struct timeval timeend;
    FeatureGen* featureGenObj = new FeatureGen();
    string chunkIDStr(argc[1]);
    int chunkID = stoi(chunkIDStr);
    string prefixLengthStr(argc[2]);
    int prefixLength = stoi(prefixLengthStr);

    u_char chunkBuffer[16385];
    ifstream chunkContentStream;
    chunkContentStream.open("./Content/" + to_string(chunkID) + ".chunk", ios::binary | ios::out);
    if (chunkContentStream.is_open() == false) {
        cerr << "Error open chunk content file for chunk ID = " << chunkID << endl;
        return 0;
    }
    chunkContentStream.read((char*)chunkBuffer, 16385);
    int chunkSize = chunkContentStream.gcount();
    cout << "Target chunk size = " << chunkSize << endl;
    char chunkHashHexBuffer[SYSTEM_CIPHER_SIZE * 2 + 1];
    u_char chunkHashBuffer[32];
    SHA256(chunkBuffer, chunkSize, chunkHashBuffer);
    for (int i = 0; i < SYSTEM_CIPHER_SIZE; i++) {
        sprintf(chunkHashHexBuffer + 2 * i, "%02X", chunkHashBuffer[i]);
    }
    cout << "Chunk hash : " << chunkHashHexBuffer << endl;
    char chunkContentHexBuffer[64 * 2 + 1];
    for (int i = 0; i < 64; i++) {
        sprintf(chunkContentHexBuffer + 2 * i, "%02X", chunkBuffer[i]);
    }
    cout << "Chunk content : " << chunkContentHexBuffer << endl;
    chunkContentStream.close();
    chunkContentStream.clear();
    // generate baseline
    vector<uint64_t> baselineFeatureList;
    featureGenObj->getFeatureList(chunkBuffer, chunkSize, baselineFeatureList);
    vector<string> baselineSFList, baselineSFListSorted;
    for (int i = 0; i < baselineFeatureList.size(); i += FEATURE_NUMBER_PER_SF) {
        u_char superFeature[SHA256_DIGEST_LENGTH];
        u_char featuresBuffer[FEATURE_NUMBER_PER_SF * sizeof(uint64_t)];
        for (int j = 0; j < FEATURE_NUMBER_PER_SF; j++) {
            memcpy(featuresBuffer + j * sizeof(uint64_t), &baselineFeatureList[i + j], sizeof(uint64_t));
        }
        SHA256(featuresBuffer, FEATURE_NUMBER_PER_SF * sizeof(uint64_t), superFeature);
        string SFStr((char*)superFeature, SHA256_DIGEST_LENGTH);
        baselineSFList.push_back(SFStr);
        baselineSFListSorted.push_back(SFStr);
    }

    cout << "Chunk content feature = " << endl;
    for (int index = 0; index < 4; index++) {
        for (int i = 0; i < SYSTEM_CIPHER_SIZE; i++) {
            sprintf(chunkHashHexBuffer + 2 * i, "%02X", baselineSFListSorted[index][i]);
        }
        cout << "Feature " << index << " : " << chunkHashHexBuffer << endl;
    }
    sort(baselineSFListSorted.begin(), baselineSFListSorted.end());
    // generate baseline cipher
    u_char baselineAllFeatureBuffer[SF_NUMBER * SHA256_DIGEST_LENGTH], baselineAllFeatureHashBuffer[SHA256_DIGEST_LENGTH];
    for (int i = 0; i < SF_NUMBER; i++) {
        memcpy(baselineAllFeatureBuffer + i * SHA256_DIGEST_LENGTH, &baselineSFList[i][0], SHA256_DIGEST_LENGTH);
    }
    SHA256(baselineAllFeatureBuffer, SF_NUMBER * SHA256_DIGEST_LENGTH, baselineAllFeatureHashBuffer);
    u_char baselineCipherMin[chunkSize], baselineCipherAll[chunkSize], baselineCipherFirst[chunkSize];
    encryptWithKey(chunkBuffer, chunkSize, (u_char*)&baselineSFListSorted[0][0], baselineCipherMin);
    encryptWithKey(chunkBuffer, chunkSize, (u_char*)&baselineSFList[0][0], baselineCipherFirst);
    encryptWithKey(chunkBuffer, chunkSize, baselineAllFeatureHashBuffer, baselineCipherAll);
    //generate prefix
    char prefixHashHexBuffer[SYSTEM_CIPHER_SIZE * 2 + 1];
    u_char hash[SHA256_DIGEST_LENGTH];
    u_char content[prefixLength * PREFIX_UNIT];
    memset(hash, 0, SHA256_DIGEST_LENGTH);
    memset(content, 0, prefixLength * PREFIX_UNIT);
    memcpy(content, baselineCipherFirst, prefixLength * PREFIX_UNIT);
    SHA256(content, prefixLength * PREFIX_UNIT, hash);
    string hashStrFirst((char*)hash, SHA256_DIGEST_LENGTH);
    cout << "Chunk first-feature based ciphertext prefix = " << endl;
    for (int i = 0; i < SYSTEM_CIPHER_SIZE; i++) {
        sprintf(prefixHashHexBuffer + 2 * i, "%02X", hash[i]);
    }
    cout << "Prefix hash : " << prefixHashHexBuffer << endl;
    memset(hash, 0, SHA256_DIGEST_LENGTH);
    memset(content, 0, prefixLength * PREFIX_UNIT);
    memcpy(content, baselineCipherMin, prefixLength * PREFIX_UNIT);
    SHA256(content, prefixLength * PREFIX_UNIT, hash);
    string hashStrMin((char*)hash, SHA256_DIGEST_LENGTH);
    cout << "Chunk min-feature based ciphertext prefix = " << endl;
    for (int i = 0; i < SYSTEM_CIPHER_SIZE; i++) {
        sprintf(prefixHashHexBuffer + 2 * i, "%02X", hash[i]);
    }
    cout << "Prefix hash : " << prefixHashHexBuffer << endl;
    memset(hash, 0, SHA256_DIGEST_LENGTH);
    memset(content, 0, prefixLength * PREFIX_UNIT);
    memcpy(content, baselineCipherAll, prefixLength * PREFIX_UNIT);
    SHA256(content, prefixLength * PREFIX_UNIT, hash);
    string hashStrAll((char*)hash, SHA256_DIGEST_LENGTH);
    cout << "Chunk all-feature based ciphertext prefix = " << endl;
    for (int i = 0; i < SYSTEM_CIPHER_SIZE; i++) {
        sprintf(prefixHashHexBuffer + 2 * i, "%02X", hash[i]);
    }
    cout << "Prefix hash : " << prefixHashHexBuffer << endl;
    delete featureGenObj;
    return 0;
}