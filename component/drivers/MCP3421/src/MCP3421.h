#include "../../I2C.h"

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
	int32_t GetADValue(_Gain3421 gain= GAIN1);
	float GetVolt();
private:
	SoftI2C IIC; // I2C通信口
	void MCP3421_WriteOne(uint8_t  dt);
	int32_t MCP3421_ReadOne(void);
	int32_t adOrigin;//ad原始值
	float Value;//测量值
	float ValueVolt;//电压值
	uint8_t adh;//原始字节
	uint8_t adm;//原始字节
	uint8_t adl;//原始字节
	uint32_t volatil;//字节汇总
};

#endif // !_MCP3421_H
