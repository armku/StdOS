#ifndef _CHECKSUM_H
#define _CHECKSUM_H

    #include "Type.h"

    class CheckSum
    {
        public:
            // Ð£ÑéºÍ
            static byte CheckSum8(byte *buf, uint16_t len);
            static uint16_t CheckSum16(byte *buf, uint16_t len);
            static uint32_t CheckSum32(byte *buf, uint16_t len);
            static byte IntelHexCheckSum(byte *buf, uint16_t len);
            static uint16_t NetCheckSum(byte *buf, uint16_t len);
            static byte BCCVerify(byte *buf, uint16_t len);
    };
#endif
