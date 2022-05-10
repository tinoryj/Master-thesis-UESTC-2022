#ifndef TEEDEDUP_POWTZ_HPP
#define TEEDEDUP_POWTZ_HPP

#include "dataStructure.hpp"
#include "privateSettings.hpp"
#include <bits/stdc++.h>
#include <pow_enclave_ta.h>
#include <tee_client_api.h>

#define RSA_KEY_SIZE 2048
#define RSA_MAX_PLAIN_LEN_2048 214
#define RSA_CIPHER_LEN_2048 (RSA_KEY_SIZE / 8)
#define HMAC_KEY_SIZE 32

struct ta_attrs {
    TEEC_Context ctx;
    TEEC_Session sess;
};

class powTZ {

public:
    struct ta_attrs ta_;
    void CA_PrintfBuffer(char* buf, uint32_t len);
    void prepare_ta_session();
    void terminate_tee_session();
    void prepareRSAOperation(TEEC_Operation* op, char* in, size_t in_sz, char* out, size_t out_sz);
    void rsa_setpublickeys();
    void rsa_setprivatekeys();
    void rsa_encrypt(char* in, size_t in_sz, char* out, size_t out_sz);
    void rsa_decrypt(char* in, size_t in_sz, char* out, size_t out_sz);
    void prepareDetection(uint32_t threshold, uint32_t windowSize);
    void preparePoWKeySetupOperation(TEEC_Operation* op, uint32_t clientID, u_char* out, size_t out_sz, u_char* hmac, size_t hmac_sz);
    void preparePoWOperation(TEEC_Operation* op, u_char* test, uint32_t lenth, u_char* out, uint32_t out_sz, u_char* out2, uint32_t out2_sz, uint32_t count);

    powTZ();
    ~powTZ();
    void getPoWKey(uint32_t clientID, u_char* out, size_t out_sz, u_char* hmac, size_t hmac_sz);
    void getSignedHashList(u_char* data, uint32_t data_size, u_char* hash_allout, u_char* hmac_out, uint32_t chunkNumber);
};

#endif // TEEDEDUP_POWTZ_HPP