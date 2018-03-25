#ifndef __CRC_H__
#define __CRC_H__

    #include "Kernel\Sys.h"

    // CRC 校验算法
    class Crc
    {
        public:
            static uint8_t CRC8(uint8_t *buf, uint32_t len);
            static uint16_t CRC16RTU(uint8_t *pszBuf, uint32_t unLength);
            static uint16_t CRC16CCITT(uint8_t *pszBuf, uint32_t unLength);
            static uint16_t CRC16Default(uint8_t *pszBuf, uint32_t unLength);
            static uint64_t CRC32Default(uint8_t *pszBuf, uint64_t ulLength);
            static uint64_t Reflect(uint64_t ref, char ch);
    }; //循环冗余校验

#endif
