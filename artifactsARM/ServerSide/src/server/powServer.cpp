#include "powServer.hpp"

void PRINT_BYTE_ARRAY_POW_SERVER(
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

void powServer::closeSession(int fd)
{
    if (sessions.find(fd) != sessions.end()) {
        sessions.erase(fd);
    }
}

powServer::powServer()
{
    cryptoObj_ = new CryptoPrimitive();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    memset(globalRand, 0, 32);
}

powServer::~powServer()
{
    auto it = sessions.begin();
    while (it != sessions.end()) {
        delete it->second;
        it++;
    }
    sessions.clear();
    delete cryptoObj_;
}

bool decryptPoWKey(u_char* inputData, u_char* outputData)
{
    RSA* p_rsa;
    FILE* file;
    if ((file = fopen("./key/sslKeys/server_rsa_private.pem", "r")) == NULL) {
        perror("open key file error");
        return false;
    }
    if ((p_rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL) {
        ERR_print_errors_fp(stdout);
        return false;
    }
    fclose(file);
    int rsa_len = RSA_size(p_rsa);
    int ret = RSA_private_decrypt(rsa_len, inputData, outputData, p_rsa, RSA_PKCS1_PADDING);
    if (ret < 0) {
        return false;
    } else {
        return true;
    }
}

bool powServer::powkeyGen(TrustZoneSession* current, int fd, u_char* powKeyEnc, u_char* powKeySig)
{
    u_char serverMac[32];
    u_char verifyKey[32];
    u_char verifyKeySeed[32 + sizeof(uint32_t)];
    memcpy(verifyKeySeed, globalRand, 32);
    uint32_t clientIDTemp = fd;
    memcpy(verifyKeySeed + 32, &clientIDTemp, sizeof(uint32_t));
    SHA256(verifyKeySeed, 32 + sizeof(uint32_t), verifyKey);
    cryptoObj_->sha256Hmac(powKeyEnc, 256, serverMac, verifyKey, 32);
    if (memcmp(powKeySig, serverMac, 32) == 0) {
        u_char plaintextPoWkey[256];
        decryptPoWKey(powKeyEnc, plaintextPoWkey);
#if SYSTEM_DEBUG_FLAG == 1
        cerr << "decrypted pow key = " << endl;
        PRINT_BYTE_ARRAY_POW_SERVER(stderr, plaintextPoWkey, 256);
#endif
        memcpy(current->powKey, plaintextPoWkey, 32);
        current->enclaveTrusted = true;
        current->clientID = fd;
        return true;
    } else {
        cerr << "PowServer : client pow key signature unvalid, client mac = " << endl;
        PRINT_BYTE_ARRAY_POW_SERVER(stderr, powKeySig, 32);
        cerr << "\t server mac = " << endl;
        PRINT_BYTE_ARRAY_POW_SERVER(stderr, serverMac, 32);
        return false;
    }
}

bool powServer::process_signedHash(TrustZoneSession* session, u_char* mac, u_char* hashList, int chunkNumber)
{
    u_char serverMac[32];
    cryptoObj_->sha256Hmac(hashList, chunkNumber * SYSTEM_CIPHER_SIZE, serverMac, session->powKey, 32);
    if (memcmp(mac, serverMac, 32) == 0) {
        return true;
    } else {
        cerr << "PowServer : client signature unvalid, hash list = " << endl;
        PRINT_BYTE_ARRAY_POW_SERVER(stderr, hashList, chunkNumber * SYSTEM_CIPHER_SIZE);
        cerr << "PowServer : client signature unvalid, client mac = " << endl;
        PRINT_BYTE_ARRAY_POW_SERVER(stderr, mac, SYSTEM_CIPHER_SIZE);
        cerr << "\t server mac = " << endl;
        PRINT_BYTE_ARRAY_POW_SERVER(stderr, serverMac, SYSTEM_CIPHER_SIZE);
        cerr << "\t server pow key = " << endl;
        PRINT_BYTE_ARRAY_POW_SERVER(stderr, session->powKey, SYSTEM_CIPHER_SIZE);
        return false;
    }
}
