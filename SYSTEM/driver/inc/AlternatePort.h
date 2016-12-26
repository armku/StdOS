#pragma once

#include "Port.h"

class CAlternatePort:public CPort
{
	public:
		CAlternatePort(PinPort pin);
		bool ReadInput();//¶ÁÈ¡¶Ë¿Ú×´Ì¬
};
