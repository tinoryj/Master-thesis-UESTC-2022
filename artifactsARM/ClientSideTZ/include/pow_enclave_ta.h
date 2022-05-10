#ifndef __RSA_TA_H__
#define __RSA_TA_H__

#define TA_CHUNK_HMAC_UUID                                 \
    {                                                      \
        0x22d38c70, 0xf25a, 0x4b95,                        \
        {                                                  \
            0xac, 0x25, 0x07, 0xd1, 0xbf, 0xdb, 0xc1, 0x12 \
        }                                                  \
    }

/* The function ID(s) implemented in this TA */
#define TA_RSA_CMD_SETPUBLICKKEY 0
#define TA_RSA_CMD_ENCRYPT 1
#define TA_RSA_CMD_DECRYPT 2
#define TA_RSA_CMD_GETHMACKEY 3
#define TA_RSA_CMD_SETPRIVATEKEY 4
#define TA_CMD_SHA256 5
#define TA_CMD_HMAC 6
#define TA_CMD_SHA256_HMAC 7
#define TA_CMD_DETECTION_PREPARE 8
#define TA_CMD_SHA256_HMAC_DETECTION 9

#endif
