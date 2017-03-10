#pragma once

#include "Type.h"

class CheckSum
{
	public:
		byte CheckSum8(byte *buf,ushort len);
		ushort CheckSum16(byte *buf,ushort len);
		uint CheckSum32(byte *buf,ushort len);
		byte IntelHexCheckSum(byte *buf,ushort len);
		ushort NetCheckSum(byte *buf, ushort len);
		byte BCCVerify(byte *buf,ushort len);
		bool OddParity(byte *buf, ushort len);
		bool EvenParity(byte *buf, ushort len);
		byte CRC8( byte *buf, ushort len) ;
		ushort CRC16Default(byte * pszBuf, ushort unLength);
		ushort CRC16CCITT(byte * pszBuf, ushort unLength);
		ushort CRC16RTU( byte * pszBuf, ushort unLength);
		UInt64 CRC32Default(byte *pszBuf,UInt64 ulLength);
	private:
		unsigned long Reflect(unsigned long ref, char ch);
		
};
