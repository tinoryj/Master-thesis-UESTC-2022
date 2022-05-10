#include "chunker.hpp"
#include "featureExtraction.hpp"
#include <bits/stdc++.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sys/time.h>
using namespace std;

#define PREFIX_UNIT 16 // block unit
vector<string> split(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

int main(int argv, char* argc[])
{
    struct timeval timestart;
    struct timeval timeend;
    // string thresholdStr(argc[1]);
    // double threshold = stod(thresholdStr);
    string mixedStreamPath(argc[1]);
    string originStreamPath(argc[2]);
    ifstream mixedStream, originStream;
    mixedStream.open(mixedStreamPath, ios::binary | ios::in);
    if (mixedStream.is_open() == false) {
        cerr << "Error load mixed info list: " << mixedStreamPath << endl;
        return 0;
    }
    originStream.open(originStreamPath, ios::binary | ios::in);
    if (originStream.is_open() == false) {
        cerr << "Error load origin info list: " << originStreamPath << endl;
        return 0;
    }
    vector<double> originDupVec, originFirstVec, originMinVec, originAllVec;
    vector<double> mixedDupVec, mixedFirstVec, mixedMinVec, mixedAllVec;
    string currentLine;
    uint64_t windowCountOrigin = 0, windowCountMixed = 0;
    getline(mixedStream, currentLine); //skip title
    while (getline(mixedStream, currentLine)) {
        windowCountMixed++;
        vector<string> currentLineSplit = split(currentLine, ",");
        // double dupRatio = stod(currentLineSplit[3]);
        // mixedDupVec.push_back(dupRatio);
        // cout << dupRatio << endl;
        double firstRatio = stod(currentLineSplit[0]);
        mixedFirstVec.push_back(firstRatio);
        // cout << firstRatio << endl;
        double minRatio = stod(currentLineSplit[1]);
        mixedMinVec.push_back(minRatio);
        double allRatio = stod(currentLineSplit[2]);
        mixedAllVec.push_back(allRatio);
        // cout << minRatio << endl;
    }
    mixedStream.close();
    getline(originStream, currentLine); //skip title
    while (getline(originStream, currentLine)) {
        windowCountOrigin++;
        vector<string> currentLineSplit = split(currentLine, ",");
        // double dupRatio = stod(currentLineSplit[3]);
        // originDupVec.push_back(dupRatio);
        // cout << dupRatio << endl;
        double firstRatio = stod(currentLineSplit[0]);
        originFirstVec.push_back(firstRatio);
        // cout << firstRatio << endl;
        double minRatio = stod(currentLineSplit[1]);
        originMinVec.push_back(minRatio);
        // cout << minRatio << endl;
        double allRatio = stod(currentLineSplit[2]);
        originAllVec.push_back(allRatio);
    }
    sort(originFirstVec.begin(), originFirstVec.end(), greater<double>());
    sort(originMinVec.begin(), originMinVec.end(), greater<double>());
    sort(originAllVec.begin(), originAllVec.end(), greater<double>());
    // sort(originDupVec.begin(), originDupVec.end()), greater<double>();
    sort(mixedFirstVec.begin(), mixedFirstVec.end(), greater<double>());
    sort(mixedMinVec.begin(), mixedMinVec.end(), greater<double>());
    sort(mixedAllVec.begin(), mixedAllVec.end(), greater<double>());
    // sort(mixedDupVec.begin(), mixedDupVec.end(), greater<double>());
    // if (originFirstVec[0] >= threshold) {
    //     cout << "yes, ";
    // } else {
    //     cout << "no, ";
    // }
    // if (originMinVec[0] >= threshold) {
    //     cout << "yes, ";
    // } else {
    //     cout << "no, ";
    // }
    // if (originAllVec[0] >= threshold) {
    //     cout << "yes, ";
    // } else {
    //     cout << "no, ";
    // }
    // if (mixedFirstVec[0] >= threshold) {
    //     cout << "yes, ";
    // } else {
    //     cout << "no, ";
    // }
    // if (mixedMinVec[0] >= threshold) {
    //     cout << "yes, ";
    // } else {
    //     cout << "no, ";
    // }
    // if (mixedAllVec[0] >= threshold) {
    //     cout << "yes" << endl;
    // } else {
    //     cout << "no" << endl;
    // }
    cout << originFirstVec[0] << " , " << originMinVec[0] << " , " << originAllVec[0] << " , " << mixedFirstVec[0] << " , " << mixedMinVec[0] << " , " << mixedAllVec[0] << endl;
    originStream.close();
    return 0;
}