#ifndef __CRC_H__
#define __CRC_H__

#include "Kernel\Sys.h"

// CRC 校验算法
class Crc
{
public:
    static ushort CRC16RTU( byte * pszBuf, uint unLength);
	static ushort CRC16CCITT(byte * pszBuf, uint unLength);
	static ushort CRC16Default(byte * pszBuf, uint unLength);
};

#endif
