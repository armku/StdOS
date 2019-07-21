#ifndef _CHECKSUM_H
#define _CHECKSUM_H
    #include "Type.h"
    class CheckSum
    {
        public:
			static byte XorCheckSum8(byte *buf, uint16_t len);
			static byte CheckSum8(byte *buf, uint len);
			static uint16_t CheckSum16(byte *buf, uint len);
			static uint CheckSum32(byte *buf, uint len);
			static byte IntelHexCheckSum(byte *buf, uint len);
			static uint16_t NetCheckSum(byte *buf, uint len);
			static byte BCCVerify(byte *buf, uint len);
    }; //Ð£ÑéºÍ
#endif
