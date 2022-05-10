#include "configure.hpp"
#include "cryptoPrimitive.hpp"
#include "dataStructure.hpp"
#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char* argv[])
{
    int testRound = atoi(argv[2]);
    int testSize = atoi(argv[1]);
    u_char chunkContentBuffer[testSize];
    memset(chunkContentBuffer, 1, testSize);
    // time evaluate
    struct timeval timestart;
    struct timeval timeend;
    double hashTime = 0, hmacTime = 0;
    long diff;
    double second;

    CryptoPrimitive* cryptoObj = new CryptoPrimitive();

    // HMAC Test
    unsigned char hmac[SYSTEM_CIPHER_SIZE], key[SYSTEM_CIPHER_SIZE];
    memset(key, 0, SYSTEM_CIPHER_SIZE);
    for (int i = 0; i < testRound; i++) {
        gettimeofday(&timestart, NULL);
        cryptoObj->sha256Hmac(chunkContentBuffer, testSize, hmac, key, SYSTEM_CIPHER_SIZE);
        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;
        hmacTime += second;
    }
    printf("HMAC Total time = %lf\nAverage Speed = %lf\n", hmacTime, (testRound * testSize / 1024 / 1024) / hmacTime);
    // SHA256 Test
    unsigned char hash[SYSTEM_CIPHER_SIZE];
    for (int i = 0; i < testRound; i++) {
        gettimeofday(&timestart, NULL);
        cryptoObj->generateHash(chunkContentBuffer, testSize, hash);
        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;
        hashTime += second;
    }
    printf("SHA256 Total time = %lf\nAverage Speed = %lf\n", hashTime, (testRound * testSize / 1024 / 1024) / hashTime);
    // AES-CFB-256 Enc Test
    unsigned char result[testSize];
    memset(key, 0, SYSTEM_CIPHER_SIZE);
    for (int i = 0; i < testRound; i++) {
        gettimeofday(&timestart, NULL);
        cryptoObj->encryptWithKey(chunkContentBuffer, testSize, key, result);
        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;
        hashTime += second;
    }
    printf("AES-CFB-256 Enc Total time = %lf\nAverage Speed = %lf\n", hashTime, (testRound * testSize / 1024 / 1024) / hashTime);
    // AES-CFB-256 Dec Test
    memset(key, 0, SYSTEM_CIPHER_SIZE);
    for (int i = 0; i < testRound; i++) {
        gettimeofday(&timestart, NULL);
        cryptoObj->decryptWithKey(chunkContentBuffer, testSize, key, result);
        gettimeofday(&timeend, NULL);
        diff = 1000000 * (timeend.tv_sec - timestart.tv_sec) + timeend.tv_usec - timestart.tv_usec;
        second = diff / 1000000.0;
        hashTime += second;
    }
    printf("AES-CFB-256 Dec Total time = %lf\nAverage Speed = %lf\n", hashTime, (testRound * testSize / 1024 / 1024) / hashTime);
    return 0;
}
