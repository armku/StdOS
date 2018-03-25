#ifndef _CHECKSUM_H
#define _CHECKSUM_H

    #include "Type.h"

    class CheckSum
    {
        public:
            // Ð£ÑéºÍ
            static byte CheckSum8(byte *buf, ushort len);
            static ushort CheckSum16(byte *buf, ushort len);
            static uint32_t CheckSum32(byte *buf, ushort len);
            static byte IntelHexCheckSum(byte *buf, ushort len);
            static ushort NetCheckSum(byte *buf, ushort len);
            static byte BCCVerify(byte *buf, ushort len);
    };
#endif
