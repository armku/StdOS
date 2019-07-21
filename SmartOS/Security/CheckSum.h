#ifndef _CHECKSUM_H
#define _CHECKSUM_H
    #include "Type.h"
    class CheckSum
    {
        public:
			static uint8_t XorCheckSum8(uint8_t *buf, uint16_t len);
			static uint8_t CheckSum8(uint8_t *buf, uint len);
			static uint16_t CheckSum16(uint8_t *buf, uint len);
			static uint CheckSum32(uint8_t *buf, uint len);
			static uint8_t IntelHexCheckSum(uint8_t *buf, uint len);
			static uint16_t NetCheckSum(uint8_t *buf, uint len);
			static uint8_t BCCVerify(uint8_t *buf, uint len);
    }; //Ð£ÑéºÍ
#endif
