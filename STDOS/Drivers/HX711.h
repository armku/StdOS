#ifndef _HX711_H_
#define _HX711_H_

#include "Device\Port.h"
#include "Device\Power.h"

enum HX711Mode
{
	A128	= 1,	// Aͨ��128��
	A64		= 2,	// Bͨ��32 ��
	B32		= 3,	// Aͨ��64 ��
};

// 24λ ADC
class HX711
{
private:
	OutputPort	SCK;
	InputPort	DOUT;
	bool Opened;

public:
	HX711Mode Mode = A128;

	HX711(Pin sck, Pin dout);
	//~HX711();

	bool Open();
	bool Close();

	uint Read();
};

#endif
