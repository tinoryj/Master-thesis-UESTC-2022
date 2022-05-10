#include "powTrustZone.hpp"
#include <cstdio>
#include <cstring>
#include <err.h>

powTZ::powTZ()
{
    prepare_ta_session();
}

powTZ::~powTZ()
{
    terminate_tee_session();
}

void powTZ::CA_PrintfBuffer(char* buf, uint32_t len)
{
    uint32_t index = 0U;
    for (index = 0U; index < len; index++) {
        if (index < 15U) {
        } else if (0U == index % 16U) {
            printf("\n");

        } else {
        }

        printf("0x%02x ", (buf[index] & 0x000000FFU));
    }
    printf("\n");
}

void powTZ::prepare_ta_session()
{
    TEEC_UUID uuid = TA_CHUNK_HMAC_UUID;
    uint32_t origin;
    TEEC_Result res;

    /* Initialize a context connecting us to the TEE */
    res = TEEC_InitializeContext(NULL, &ta_.ctx);
    if (res != TEEC_SUCCESS)
        errx(1, "\nTEEC_InitializeContext failed with code 0x%x\n", res);

    /* Open a session with the TA */
    res = TEEC_OpenSession(&ta_.ctx, &ta_.sess, &uuid,
        TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
    if (res != TEEC_SUCCESS)
        errx(1, "\nTEEC_Opensession failed with code 0x%x origin 0x%x\n", res, origin);
}

void powTZ::terminate_tee_session()
{
    TEEC_CloseSession(&ta_.sess);
    TEEC_FinalizeContext(&ta_.ctx);
}

void powTZ::prepareRSAOperation(TEEC_Operation* op, char* in, size_t in_sz, char* out, size_t out_sz)
{
    memset(op, 0, sizeof(*op));

    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE, TEEC_NONE);
    op->params[0].tmpref.buffer = in;
    op->params[0].tmpref.size = in_sz;
    op->params[1].tmpref.buffer = out;
    op->params[1].tmpref.size = out_sz;
}

void powTZ::preparePoWOperation(TEEC_Operation* op, u_char* test, uint32_t lenth, u_char* out, uint32_t out_sz, u_char* out2, uint32_t out2_sz, uint32_t count)
{
    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INPUT);
    op->params[0].tmpref.buffer = test;
    op->params[0].tmpref.size = lenth;
    op->params[1].tmpref.buffer = out;
    op->params[1].tmpref.size = out_sz;
    op->params[2].tmpref.buffer = out2;
    op->params[2].tmpref.size = out2_sz;
    op->params[3].value.a = count;
}

void powTZ::rsa_setpublickeys()
{
    TEEC_Result res;

    res = TEEC_InvokeCommand(&ta_.sess, TA_RSA_CMD_SETPUBLICKKEY, NULL, NULL);
    if (res != TEEC_SUCCESS)
        errx(1, "\nTEEC_InvokeCommand(TA_RSA_CMD_GENKEYS) failed %#x\n", res);
}

void powTZ::rsa_setprivatekeys()
{
    TEEC_Result res;

    res = TEEC_InvokeCommand(&ta_.sess, TA_RSA_CMD_SETPRIVATEKEY, NULL, NULL);
    if (res != TEEC_SUCCESS)
        errx(1, "\nTEEC_InvokeCommand(TA_RSA_CMD_GENKEYS) failed %#x\n", res);
}

void powTZ::preparePoWKeySetupOperation(TEEC_Operation* op, uint32_t clientID, u_char* out, size_t out_sz, u_char* hmac, size_t hmac_sz)
{
    memset(op, 0, sizeof(*op));
    op->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_VALUE_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE);
    op->params[0].tmpref.buffer = out;
    op->params[0].tmpref.size = out_sz;
    op->params[1].value.a = clientID;
    op->params[2].tmpref.buffer = hmac;
    op->params[2].tmpref.size = hmac_sz;
}

void powTZ::prepareDetection(uint32_t threshold, uint32_t windowSize)
{
    TEEC_Operation op;
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op.params[0].value.a = threshold;
    op.params[0].value.b = windowSize;
    uint32_t err_origin;
    TEEC_Result res = TEEC_InvokeCommand(&ta_.sess, TA_CMD_DETECTION_PREPARE, &op, &err_origin);
    if (res != TEEC_SUCCESS) {
        errx(1, "\nTEEC_InvokeCommand(0) failed 0x%x origin 0x%x\n", res, err_origin);
    }
}

void powTZ::rsa_encrypt(char* in, size_t in_sz, char* out, size_t out_sz)
{
    TEEC_Operation op;
    uint32_t origin;
    TEEC_Result res;
    prepareRSAOperation(&op, in, in_sz, out, out_sz);

    res = TEEC_InvokeCommand(&ta_.sess, TA_RSA_CMD_ENCRYPT,
        &op, &origin);
    if (res != TEEC_SUCCESS)
        errx(1, "\nTEEC_InvokeCommand(TA_RSA_CMD_ENCRYPT) failed 0x%x origin 0x%x\n",
            res, origin);
}

void powTZ::rsa_decrypt(char* in, size_t in_sz, char* out, size_t out_sz)
{
    TEEC_Operation op;
    uint32_t origin;
    TEEC_Result res;
    rsa_setprivatekeys();
    prepareRSAOperation(&op, in, in_sz, out, out_sz);

    res = TEEC_InvokeCommand(&ta_.sess, TA_RSA_CMD_DECRYPT, &op, &origin);
    if (res != TEEC_SUCCESS)
        errx(1, "\nTEEC_InvokeCommand(TA_RSA_CMD_DECRYPT) failed 0x%x origin 0x%x\n",
            res, origin);
}

void powTZ::getPoWKey(uint32_t clientID, u_char* out, size_t out_sz, u_char* hmac, size_t hmac_sz)
{
    TEEC_Operation op;
    uint32_t origin;
    TEEC_Result res;
    rsa_setpublickeys();
    preparePoWKeySetupOperation(&op, clientID, out, out_sz, hmac, hmac_sz);
    res = TEEC_InvokeCommand(&ta_.sess, TA_RSA_CMD_GETHMACKEY, &op, &origin);
    if (res != TEEC_SUCCESS) {
        errx(1, "\nTEEC_InvokeCommand(TA_RSA_CMD_DECRYPT) failed 0x%x origin 0x%x\n", res, origin);
    }
}

void powTZ::getSignedHashList(u_char* data, uint32_t data_size, u_char* hash_allout, u_char* hmac_out, uint32_t chunkNumber)
{
    TEEC_Operation op;
    uint32_t err_origin;
    TEEC_Result res;
    uint32_t hash_lenth = chunkNumber * SYSTEM_CIPHER_SIZE;
    preparePoWOperation(&op, data, data_size, hash_allout, hash_lenth, hmac_out, SYSTEM_CIPHER_SIZE, chunkNumber);
#if SUPER_FEATURE_GEN_METHOD != NAN_FEATURE
    res = TEEC_InvokeCommand(&ta_.sess, TA_CMD_SHA256_HMAC_DETECTION, &op, &err_origin);
#else
    res = TEEC_InvokeCommand(&ta_.sess, TA_CMD_SHA256_HMAC, &op, &err_origin);
#endif
    if (res != TEEC_SUCCESS) {
        errx(1, "\nTEEC_InvokeCommand(0) failed 0x%x origin 0x%x\n",
            res, err_origin);
    }
}
