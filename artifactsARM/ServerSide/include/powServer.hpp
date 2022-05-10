#ifndef SGXDEDUP_POWSERVER_HPP
#define SGXDEDUP_POWSERVER_HPP

#include "configure.hpp"
#include "cryptoPrimitive.hpp"
#include "dataStructure.hpp"
#include "messageQueue.hpp"
#include "protocol.hpp"
#include <bits/stdc++.h>
#include <openssl/err.h>

#define CA_BUNDLE "/etc/ssl/certs/ca-certificates.crt"
#define IAS_SIGNING_CA_FILE "key/AttestationReportSigningCACert.pem"
#define IAS_CERT_FILE "key/iasclient.crt"
#define IAS_CLIENT_KEY "key/iasclient.pem"

struct TrustZoneSession {
    bool enclaveTrusted;
    int clientID;
    u_char powKey[32];
};

extern Configure config;

using namespace std;

class powServer {
private:
    CryptoPrimitive* cryptoObj_;
    u_char globalRand[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

public:
    powServer();
    ~powServer();
    map<int, TrustZoneSession*> sessions;
    void closeSession(int fd);
    bool powkeyGen(TrustZoneSession* current, int fd, u_char* powKeyEnc, u_char* powKeySig);
    bool process_signedHash(TrustZoneSession* session, u_char* mac, u_char* hashList, int chunkNumber);
};

#endif //SGXDEDUP_POWSERVER_HPP
