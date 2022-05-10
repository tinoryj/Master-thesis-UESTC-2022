#include "chunker.hpp"
#include "configure.hpp"
#include "cryptoPrimitive.hpp"
#include "dataStructure.hpp"
#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char* argv[])
{
    string inputFileName(argv[1]);
    ifstream readFileStream;
    readFileStream.open(inputFileName, ios::in);
    int readSize_ = 20 * 1024 * 1024;
    u_char waitingForChunkingBuffer_[readSize_];
    // time evaluate
    struct timeval timestart;
    struct timeval timeend;
    long diff;
    double second;

    memset(waitingForChunkingBuffer_, 0, sizeof(u_char) * readSize_);
    gettimeofday(&timestart, NULL);
    readFileStream.read((char*)waitingForChunkingBuffer_, sizeof(u_char) * readSize_);
    gettimeofday(&timeend, NULL);
    diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
    second = diff / 1000000.0;
    cerr << "Read file time = " << second << endl;
    int len = readFileStream.gcount();
    cerr << "Read file speed = " << (double)len / 1024.0 / 1024.0 / second << endl;

    ofstream writeFileStream;
    writeFileStream.open("temp.log", ios::out);
    gettimeofday(&timestart, NULL);
    writeFileStream.write((char*)waitingForChunkingBuffer_, len);
    gettimeofday(&timeend, NULL);
    diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
    second = diff / 1000000.0;
    cerr << "Write file time = " << second << endl;
    cerr << "Write file speed = " << (double)len / 1024.0 / 1024.0 / second << endl;

    return 0;
}
