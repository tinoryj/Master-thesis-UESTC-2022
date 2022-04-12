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
    string chunkInfoPath(argc[1]);
    string windowSizeStr(argc[2]);
    int windowSize = stoi(windowSizeStr);
    string prefixLengthStr(argc[3]);
    int prefixLength = stoi(prefixLengthStr);
    string thresholdStr(argc[4]);
    float threshold = stof(thresholdStr);
    ifstream chunkInfoStream;
    chunkInfoStream.open(chunkInfoPath, ios::binary | ios::in);
    if (chunkInfoStream.is_open() == false) {
        cerr << "Error load chunk info list: " << chunkInfoPath << endl;
        return 0;
    }
    string currentLine;
    uint64_t currentWindowCount = 0;
    bool first = false, min = false, all = false;
    unordered_map<string, vector<uint64_t>> prefixCounterFirst, prefixCounterMin, prefixCounterAll, dedupLicationMap;
    cout << "firstFeature, minFeature, allFeature" << endl;
    while (getline(chunkInfoStream, currentLine)) {
        int chunkID = stoi(currentLine);
        getline(chunkInfoStream, currentLine);
        string chunkHash = currentLine;
        getline(chunkInfoStream, currentLine);
        int chunkSize = stoi(currentLine);
        if (dedupLicationMap.find(chunkHash) == dedupLicationMap.end()) {
            vector<uint64_t> chunkVec;
            chunkVec.push_back(chunkID);
            dedupLicationMap.insert(make_pair(chunkHash, chunkVec));
        } else {
            dedupLicationMap.at(chunkHash).push_back(chunkID);
        }
        u_char chunkBuffer[chunkSize];
        ifstream chunkContentStream;
        chunkContentStream.open("./Content/" + to_string(chunkID) + ".chunk", ios::binary | ios::out);
        if (chunkContentStream.is_open() == false) {
            cerr << "Error open chunk content file for chunk ID = " << chunkID << endl;
            return 0;
        }
        chunkContentStream.read((char*)chunkBuffer, chunkSize);
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
        sort(baselineSFListSorted.begin(), baselineSFListSorted.end());
        // generate baseline cipher
        u_char baselineAllFeatureBuffer[SF_NUMBER * SHA256_DIGEST_LENGTH], baselineAllFeatureHashBuffer[SHA256_DIGEST_LENGTH];
        for (int i = 0; i < SF_NUMBER; i++) {
            memcpy(baselineAllFeatureBuffer + i * SHA256_DIGEST_LENGTH, &baselineSFList[i][0], SHA256_DIGEST_LENGTH);
        }
        SHA256(baselineAllFeatureBuffer, SF_NUMBER * SHA256_DIGEST_LENGTH, baselineAllFeatureHashBuffer);
        u_char baselineCipherMin[chunkSize], baselineCipherFirst[chunkSize], baselineCipherAll[chunkSize];
        encryptWithKey(chunkBuffer, chunkSize, (u_char*)&baselineSFListSorted[0][0], baselineCipherMin);
        encryptWithKey(chunkBuffer, chunkSize, (u_char*)&baselineSFList[0][0], baselineCipherFirst);
        encryptWithKey(chunkBuffer, chunkSize, baselineAllFeatureHashBuffer, baselineCipherAll);
        //generate prefix

        u_char hash[SHA256_DIGEST_LENGTH];
        u_char content[prefixLength * PREFIX_UNIT];
        memset(hash, 0, SHA256_DIGEST_LENGTH);
        memset(content, 0, prefixLength * PREFIX_UNIT);
        if (chunkSize < prefixLength * PREFIX_UNIT) {
            memcpy(content, baselineCipherFirst, chunkSize);
            SHA256(content, chunkSize, hash);
        } else {
            memcpy(content, baselineCipherFirst, prefixLength * PREFIX_UNIT);
            SHA256(content, prefixLength * PREFIX_UNIT, hash);
        }
        string hashStrFirst((char*)hash, SHA256_DIGEST_LENGTH);
        if (prefixCounterFirst.find(hashStrFirst) == prefixCounterFirst.end()) {
            vector<uint64_t> chunkVec;
            chunkVec.push_back(chunkID);
            prefixCounterFirst.insert(make_pair(hashStrFirst, chunkVec));
        } else {
            prefixCounterFirst.at(hashStrFirst).push_back(chunkID);
        }
        memset(hash, 0, SHA256_DIGEST_LENGTH);
        memset(content, 0, prefixLength * PREFIX_UNIT);
        if (chunkSize < prefixLength * PREFIX_UNIT) {
            memcpy(content, baselineCipherMin, chunkSize);
            SHA256(content, chunkSize, hash);
        } else {
            memcpy(content, baselineCipherMin, prefixLength * PREFIX_UNIT);
            SHA256(content, prefixLength * PREFIX_UNIT, hash);
        }
        string hashStrMin((char*)hash, SHA256_DIGEST_LENGTH);
        if (prefixCounterMin.find(hashStrMin) == prefixCounterMin.end()) {
            vector<uint64_t> chunkVec;
            chunkVec.push_back(chunkID);
            prefixCounterMin.insert(make_pair(hashStrMin, chunkVec));
        } else {
            prefixCounterMin.at(hashStrMin).push_back(chunkID);
        }
        memset(hash, 0, SHA256_DIGEST_LENGTH);
        memset(content, 0, prefixLength * PREFIX_UNIT);
        if (chunkSize < prefixLength * PREFIX_UNIT) {
            memcpy(content, baselineCipherAll, chunkSize);
            SHA256(content, chunkSize, hash);
        } else {
            memcpy(content, baselineCipherAll, prefixLength * PREFIX_UNIT);
            SHA256(content, prefixLength * PREFIX_UNIT, hash);
        }
        string hashStrAll((char*)hash, SHA256_DIGEST_LENGTH);
        if (prefixCounterAll.find(hashStrAll) == prefixCounterAll.end()) {
            vector<uint64_t> chunkVec;
            chunkVec.push_back(chunkID);
            prefixCounterAll.insert(make_pair(hashStrAll, chunkVec));
        } else {
            prefixCounterAll.at(hashStrAll).push_back(chunkID);
        }
        currentWindowCount++;
        if (currentWindowCount == windowSize) {
            // cout << dedupLicationMap.size() << " , ";
            int maxFreq = 0;
            vector<uint64_t> flag;
            for (auto i : dedupLicationMap) {
                if (i.second.size() > maxFreq) {
                    maxFreq = i.second.size();
                    flag.clear();
                    for (int j = 0; j < maxFreq; j++) {
                        flag.push_back(i.second[j]);
                    }
                }
            }
            // cout << maxFreq << " , ";
            // for (int i = 0; i < flag.size(); i++) {
            //     cout << flag[i] << " ";
            // }
            // cout << " , ";
            // cout << (double)maxFreq / (double)windowSize << " , ";
            maxFreq = 0;
            for (auto i : prefixCounterFirst) {
                if (i.second.size() > maxFreq) {
                    maxFreq = i.second.size();
                    flag.clear();
                    for (int j = 0; j < maxFreq; j++) {
                        flag.push_back(i.second[j]);
                    }
                }
            }
            // cout << maxFreq << " , " << prefixCounterFirst.size() << " , " << currentWindowCount << " , ";
            // for (int i = 0; i < flag.size(); i++) {
            //     cout << flag[i] << " ";
            // }
            // cout << " , ";
            cout << (double)maxFreq / (double)windowSize << " , ";
            if (((double)maxFreq / (double)windowSize) >= threshold) {
                first = true;
            }
            maxFreq = 0;
            for (auto i : prefixCounterMin) {
                if (i.second.size() > maxFreq) {
                    maxFreq = i.second.size();
                    flag.clear();
                    for (int j = 0; j < maxFreq; j++) {
                        flag.push_back(i.second[j]);
                    }
                }
            }
            // cout << maxFreq << " , " << prefixCounterMin.size() << " , " << currentWindowCount << " , ";
            // for (int i = 0; i < flag.size(); i++) {
            //     cout << flag[i] << " ";
            // }
            // cout << " , ";
            cout << (double)maxFreq / (double)windowSize << " , ";
            if (((double)maxFreq / (double)windowSize) >= threshold) {
                min = true;
            }
            maxFreq = 0;
            for (auto i : prefixCounterAll) {
                if (i.second.size() > maxFreq) {
                    maxFreq = i.second.size();
                    flag.clear();
                    for (int j = 0; j < maxFreq; j++) {
                        flag.push_back(i.second[j]);
                    }
                }
            }
            // cout << maxFreq << " , " << prefixCounterAll.size() << " , " << currentWindowCount << " , ";
            // for (int i = 0; i < flag.size(); i++) {
            //     cout << flag[i] << " ";
            // }
            // cout << " , ";
            cout << (double)maxFreq / (double)windowSize << " , ";
            if (((double)maxFreq / (double)windowSize) >= threshold) {
                all = true;
            }
            prefixCounterAll.clear();
            prefixCounterFirst.clear();
            prefixCounterMin.clear();
            dedupLicationMap.clear();
            cout << 0 << endl;
            currentWindowCount = 0;
        }
    }
    if (currentWindowCount != 0) {
        // cout << dedupLicationMap.size() << " , ";
        int maxFreq = 0;
        vector<uint64_t> flag;
        for (auto i : dedupLicationMap) {
            if (i.second.size() > maxFreq) {
                maxFreq = i.second.size();
                flag.clear();
                for (int j = 0; j < maxFreq; j++) {
                    flag.push_back(i.second[j]);
                }
            }
        }
        // cout << maxFreq << " , ";
        // for (int i = 0; i < flag.size(); i++) {
        //     cout << flag[i] << " ";
        // }
        // cout << " , ";
        // cout << (double)maxFreq / (double)windowSize << " , ";
        maxFreq = 0;
        for (auto i : prefixCounterFirst) {
            if (i.second.size() > maxFreq) {
                maxFreq = i.second.size();
                flag.clear();
                for (int j = 0; j < maxFreq; j++) {
                    flag.push_back(i.second[j]);
                }
            }
        }
        // cout << maxFreq << " , " << prefixCounterFirst.size() << " , " << currentWindowCount << " , ";
        // for (int i = 0; i < flag.size(); i++) {
        //     cout << flag[i] << " ";
        // }
        // cout << " , ";
        cout << (double)maxFreq / (double)windowSize << " , ";
        if (((double)maxFreq / (double)windowSize) >= threshold) {
            first = true;
        }
        maxFreq = 0;
        for (auto i : prefixCounterMin) {
            if (i.second.size() > maxFreq) {
                maxFreq = i.second.size();
                flag.clear();
                for (int j = 0; j < maxFreq; j++) {
                    flag.push_back(i.second[j]);
                }
            }
        }
        // cout << maxFreq << " , " << prefixCounterMin.size() << " , " << currentWindowCount << " , ";
        // for (int i = 0; i < flag.size(); i++) {
        //     cout << flag[i] << " ";
        // }
        // cout << " , ";
        cout << (double)maxFreq / (double)windowSize << " , ";
        if (((double)maxFreq / (double)windowSize) >= threshold) {
            min = true;
        }
        maxFreq = 0;
        for (auto i : prefixCounterAll) {
            if (i.second.size() > maxFreq) {
                maxFreq = i.second.size();
                flag.clear();
                for (int j = 0; j < maxFreq; j++) {
                    flag.push_back(i.second[j]);
                }
            }
        }
        // cout << maxFreq << " , " << prefixCounterAll.size() << " , " << currentWindowCount << " , ";
        // for (int i = 0; i < flag.size(); i++) {
        //     cout << flag[i] << " ";
        // }
        // cout << " , ";
        cout << (double)maxFreq / (double)windowSize << endl;
        if (((double)maxFreq / (double)windowSize) >= threshold) {
            all = true;
        }
        prefixCounterAll.clear();
        prefixCounterFirst.clear();
        prefixCounterMin.clear();
        // cout << 0 << endl;
        currentWindowCount = 0;
    }
    if (first == true) {
        cerr << "firstFeature: detected" << endl;
    } else {
        cerr << "firstFeature: not detected" << endl;
    }
    if (min == true) {
        cerr << "minFeature: detected" << endl;
    } else {
        cerr << "minFeature: not detected" << endl;
    }
    if (all == true) {
        cerr << "allFeature: detected" << endl;
    } else {
        cerr << "allFeature: not detected" << endl;
    }
    chunkInfoStream.close();
    delete featureGenObj;
    return 0;
}