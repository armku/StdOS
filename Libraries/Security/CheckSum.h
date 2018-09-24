#ifndef _CHECKSUM_H
#define _CHECKSUM_H
    #include "Type.h"
    class CheckSum
    {
        public:
            static uint8_t CheckSum8(uint8_t *buf, uint32_t len);
			static uint16_t CheckSum16(uint8_t *buf, uint32_t len);
			static uint32_t CheckSum32(uint8_t *buf, uint32_t len);
			static uint8_t IntelHexCheckSum(uint8_t *buf, uint32_t len);
			static uint16_t NetCheckSum(uint8_t *buf, uint32_t len);
			static uint8_t BCCVerify(uint8_t *buf, uint32_t len);
    }; //Ð£ÑéºÍ
#endif
