#include "../../I2C.h"

#ifndef _MCP3421_H
#define _MCP3421_H

typedef enum
{
	GAIN1 = 0,
	GAIN2,
	GAIN4,
	GAIN8,
}_Gain3421;//������������

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
	SoftI2C IIC; // I2Cͨ�ſ�
	void MCP3421_WriteOne(uint8_t  dt);
	int32_t MCP3421_ReadOne(void);
	int32_t adOrigin;//adԭʼֵ
	float Value;//����ֵ
	float ValueVolt;//��ѹֵ
	uint8_t adh;//ԭʼ�ֽ�
	uint8_t adm;//ԭʼ�ֽ�
	uint8_t adl;//ԭʼ�ֽ�
	uint32_t volatil;//�ֽڻ���
};

#endif // !_MCP3421_H
