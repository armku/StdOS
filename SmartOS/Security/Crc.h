#ifndef __CRC_H__
#define __CRC_H__

    #include "Kernel\Sys.h"

    // CRC 校验算法
    class Crc
    {
        public:
            static byte CRC8(byte *buf, uint32_t len);
            static uint16_t CRC16RTU(byte *pszBuf, uint32_t unLength);
            static uint16_t CRC16CCITT(byte *pszBuf, uint32_t unLength);
            static uint16_t CRC16Default(byte *pszBuf, uint32_t unLength);
            static uint64_t CRC32Default(byte *pszBuf, uint64_t ulLength);
            static uint64_t Reflect(uint64_t ref, char ch);
			static uint Hash(const Buffer& arr, uint crc = 0);
			static ushort Hash16(const Buffer& arr, ushort crc = 0xFFFF);
    }; //循环冗余校验

#endif
