#ifndef FEATURESPY_PRIVATESETTINGS_HPP
#define FEATURESPY_PRIVATESETTINGS_HPP

/* System Test Settings: 0-disable; 1-enable */
#define SYSTEM_BREAK_DOWN 0
#define SYSTEM_DEBUG_FLAG 0
#define SYSTEM_LOG_FLAG 0
#define ENCLAVE_SEALED_INIT_ENABLE 1 // set to 0 means do remote attestation every startup

/* FBE Key Generation method Settings */
#define FIRST_FEATURE 0
#define MIN_FEATURE 1
#define ALL_FEATURE 2

/* Key Generation method Settings */
#define KEY_GEN_SGX_CFB 0
#define KEY_GEN_SGX_CTR 1
#define KEY_GEN_METHOD_TYPE KEY_GEN_SGX_CTR

#define SYSTEM_CIPHER_SIZE 32
#define CRYPTO_BLOCK_SIZE 16

#if PREFIX_LENGTH > 32
#define COUNT_PREFIX_BY_HASH 1
#else
#define COUNT_PREFIX_BY_HASH 0
#endif

/* System Infomation Size Settings */

#define MAX_CHUNK_SIZE 16384 //macro for the max size of variable-size chunker
#define NETWORK_MESSAGE_DATA_SIZE 18 * 1024 * 1024
#define SGX_MESSAGE_MAX_SIZE 1024 * 1024

/* System Infomation Type Settings */
#define DATA_TYPE_RECIPE 1
#define DATA_TYPE_CHUNK 2
#define CHUNK_TYPE_NEED_UPLOAD 3
#define MULTI_CLIENT_UPLOAD_TEST 0 // set to 1 means not write content to disk on server side
#define TRACE_DRIVEN_TEST 1
#define STORAGE_CORE_READ_CACHE 1

#endif //FEATURESPY_PRIVATESETTINGS_HPP