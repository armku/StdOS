#pragma once
#include "Type.h"
#include "Object.h"

class DateTime
{
	public:
		ushort Year;
		byte Month;
		byte Day;
		byte Hour;
		byte Minute;
		byte Second;
		ushort Ms;
	
		DateTime();
	DateTime(ushort year,byte month,byte day);
};
