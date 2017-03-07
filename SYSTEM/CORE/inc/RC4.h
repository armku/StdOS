#pragma once

#include "Type.h"

class RC4
{
	public:
		void Init(byte*s,byte*key, uint Len);
		void rc4_crypt(byte*s,byte*Data,uint Len);
};

