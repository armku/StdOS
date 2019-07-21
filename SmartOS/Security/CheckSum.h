#ifndef _CHECKSUM_H
#define _CHECKSUM_H
    #include "Type.h"
    class CheckSum
    {
        public:
			static byte XorCheckSum8(byte *buf, ushort len);
			static byte CheckSum8(byte *buf, uint len);
			static ushort CheckSum16(byte *buf, uint len);
			static uint CheckSum32(byte *buf, uint len);
			static byte IntelHexCheckSum(byte *buf, uint len);
			static ushort NetCheckSum(byte *buf, uint len);
			static byte BCCVerify(byte *buf, uint len);
    }; //Ð£ÑéºÍ
#endif
