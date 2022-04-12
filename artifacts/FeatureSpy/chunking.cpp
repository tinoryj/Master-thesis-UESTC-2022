#include "chunker.hpp"
#include "featureExtraction.hpp"
#include <bits/stdc++.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sys/time.h>
using namespace std;

int main(int argv, char* argc[])
{
    struct timeval timestart;
    struct timeval timeend;
    vector<string> chunkingFileList;
    Chunker* chunkObj = new Chunker();
    if (argv == 2) {
        string fileName(argc[1]);
        ifstream fileListStream;
        fileListStream.open(fileName, ios::in | ios::binary);
        string currentLineStr;
        while (getline(fileListStream, currentLineStr)) {
            string newPath = currentLineStr;
            chunkingFileList.push_back(newPath);
        }
        fileListStream.close();
        fileListStream.clear();
    } else {
        string fileName(argc[1]);
        string fileNameFake(argc[2]);
        Chunker* chunkObj = new Chunker();
        // chunkObj->chunkingWithCount(fileName);
        ifstream fileListStream;
        fileListStream.open(fileName, ios::in | ios::binary);
        string currentLineStr;
        vector<string> filePathList;
        while (getline(fileListStream, currentLineStr)) {
            string newPath = currentLineStr;
            filePathList.push_back(newPath);
        }
        fileListStream.close();
        fileListStream.clear();
        ifstream fakeFileListStream;
        fakeFileListStream.open(fileNameFake, ios::in | ios::binary);
        vector<string> targetFakeFileList;
        while (getline(fakeFileListStream, currentLineStr)) {
            string newPath = currentLineStr;
            targetFakeFileList.push_back(newPath);
        }
        fakeFileListStream.close();
        fakeFileListStream.clear();
        int targetFileNumber = targetFakeFileList.size();
        int originFileNumber = filePathList.size();
        int totalFileNumber = targetFileNumber + originFileNumber;
        srand(1);

        int originIndex = 0, targetIndex = 0;
        for (int i = 0; i < totalFileNumber; i++) {
            int targetPos = rand() % totalFileNumber;
            if (targetPos < originFileNumber) {
                chunkingFileList.push_back(filePathList[originIndex]);
                originIndex++;
                if (originIndex == filePathList.size()) {
                    break;
                }
            } else {
                chunkingFileList.push_back(targetFakeFileList[targetIndex]);
                targetIndex++;
                if (targetIndex == targetFakeFileList.size()) {
                    break;
                }
            }
        }
        if (originIndex != filePathList.size()) {
            for (int i = originIndex; i < filePathList.size(); i++) {
                chunkingFileList.push_back(filePathList[originIndex]);
                originIndex++;
            }
        }
        if (targetIndex != targetFakeFileList.size()) {
            for (int i = targetIndex; i < targetFakeFileList.size(); i++) {
                chunkingFileList.push_back(targetFakeFileList[targetIndex]);
                targetIndex++;
            }
        }
    }
    gettimeofday(&timestart, NULL);

    // for (int i = 0; i < chunkingFileList.size(); i++) {
    //     cout << chunkingFileList[i] << endl;
    // }
    // cout << totalFileNumber << "\t" << chunkingFileList.size() << endl;
    uint64_t chunkID = 0;
    for (int i = 0; i < chunkingFileList.size(); i++) {
        chunkObj->chunkingWithCountID(chunkingFileList[i], chunkID);
    }
    gettimeofday(&timeend, NULL);
    long diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
    double second = diff / 1000000.0;
    cerr << "System : total work time = " << second << " s" << endl;
    return 0;
}
