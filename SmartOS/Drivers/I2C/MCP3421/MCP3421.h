#include "../OnChip/I2C.h"

#ifndef _MCP3421_H
#define _MCP3421_H

typedef enum
{
	GAIN1 = 0,
	GAIN2,
	GAIN4,
	GAIN8,
}_Gain3421;//基础输入类型

class MCP3421
{
public:
	MCP3421();
public:
	void SetPin(Pin pscl, Pin psda);
	void Init();
	int GetADValue(_Gain3421 gain= GAIN1);
	float GetVolt();
private:
	I2CSoft IIC; // I2C通信口
	//byte Address; // 设备地址
	void MCP3421_WriteOne(byte  dt);
	int MCP3421_ReadOne(void);
};

#endif // !_MCP3421_H
