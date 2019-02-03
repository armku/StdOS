#ifndef _DS18B20_H
#define _DS18B20_H
#include "../../../Bsp/Porting.h"
class DS18B20
{
public:
	uint8_t Init();
	void SetPin(Pin pin);
	float GetTemp();
	void ReadId();
	bool Presence();
private:
	void Start();
	void WriteByte(uint8_t dat);
	uint8_t ReadByte();
	uint8_t ReadBit();
	void Rest();
	void SkipRom();
private:
	mcuGpio _dio;
public:
	char id[8];//…Ë±∏ID

};
#endif
