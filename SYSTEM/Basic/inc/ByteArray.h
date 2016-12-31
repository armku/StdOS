#pragma once

#include "Array.h"

class ByteArray:public Array
{
	public:
		ByteArray(byte* buf,ushort length);
};
