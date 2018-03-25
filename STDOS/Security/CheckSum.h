#ifndef _CHECKSUM_H
#define _CHECKSUM_H
    #include "Type.h"
    class CheckSum
    {
        public:
            static byte CheckSum8(byte *buf, uint32_t len);
			static ushort CheckSum16(byte *buf, uint32_t len);
			static uint32_t CheckSum32(byte *buf, uint32_t len);
			static byte IntelHexCheckSum(byte *buf, uint32_t len);
			static ushort NetCheckSum(byte *buf, uint32_t len);
			static byte BCCVerify(byte *buf, uint32_t len);
    }; //Ð£ÑéºÍ
#endif
