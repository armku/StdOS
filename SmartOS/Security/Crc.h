#ifndef __CRC_H__
#define __CRC_H__

    #include "Kernel\Sys.h"

    // CRC 校验算法
    class Crc
    {
        public:
            static byte CRC8(byte *buf, uint len);
            static ushort CRC16RTU(byte *pszBuf, uint unLength);
            static ushort CRC16CCITT(byte *pszBuf, uint unLength);
            static ushort CRC16Default(byte *pszBuf, uint unLength);
            static UInt64 CRC32Default(byte *pszBuf, UInt64 ulLength);
            static UInt64 Reflect(UInt64 ref, char ch);
			static uint Hash(const Buffer& arr, uint crc = 0);
			static ushort Hash16(const Buffer& arr, ushort crc = 0xFFFF);
    }; //循环冗余校验

#endif
