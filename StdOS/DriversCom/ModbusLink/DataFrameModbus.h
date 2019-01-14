#include "Type.h"
#include "Security/Crc.h"
#include "../DataFrame.h"

#ifndef _DATAFRAMEMODBUS_H
#define _DATAFRAMEMODBUS_H

struct DataFrameModbus:public DataFrame
{
public:
	uint8_t Address;	// 地址
	uint8_t Code;		// 功能码
	uint8_t Error;		// 是否异常
	uint16_t regAddr;
	uint16_t regLength;
	bool isUpdated;
	uint16_t Crc;		// 校验码
	uint16_t Crc2;		// 动态计算得到的校验码
	int frameLength;//当前数据帧长度
	int Cnt;//数据帧数量

public:
	DataFrameModbus()                 //constructor
	{
		isUpdated = false;
		Cnt = 0;
	}

	bool VerifyCheckCode()
	{
		this->Crc2 = Crc::CRC16RTU(data, frameLength-2);
		if (this->Crc2 == this->Crc)
			return true;
		else
			return false;
	}
	//设置传输寄存器值，地址从0开始编码
	void SetReg(uint8_t regaddr, uint16_t val)
	{
		this->data[3 + regaddr * 2] = val >> 8;
		this->data[3 + regaddr * 2 + 1] = val & 0xff;
	}
	void SetRegLen(uint8_t len)
	{
		this->data[2] = len * 2;
		this->frameLength = 5 + len * 2;
		this->dataLength = this->frameLength;
	}
	bool RemoveOneFrame()
	{
		if (frameLength<0 || frameLength>this->dataLength)
			return false;
		for (int i = 0; i < this->dataLength - frameLength; i++)
		{
			this->data[i] = this->data[i + frameLength];
		}
		this->dataLength -= frameLength;
		this->frameLength = 0;
		return true;
	}
	bool CheckFrame()
	{
		if (this->dataLength >= 8)
		{
			this->Address = this->data[0];
			this->Code = this->data[1];
			this->regAddr = this->data[2];
			this->regAddr <<= 8;
			this->regAddr |= this->data[3];
			this->regLength = this->data[4];
			this->regLength <<= 8;
			this->regLength |= this->data[5];

			//this->frameLength = 8;
			//switch (this->Code)
			//{
			//case 0X02:
			//case 0X01:
			//case 0X05:
			//case 0X04:
			//case 0X03:
			//case 0X06:
			//	frameLength = 8;
			//	break;
			//case 0x0F://多个写 0 区输出继电器(指令代码: 0X0F) 
			//case 0x10://多个写 4 区输出寄存器(指令代码: 0X10) 
			//	frameLength = data[6] + frameLength + 1;
			//	break;
			//default:
			//	frameLength = 0;
			//	break;
			//}
			if (frameLength == 0)
			{
				//非法指令，直接清空缓冲区
				frameLength = this->dataLength;
				this->RemoveOneFrame();
				return false;
			}
			//需要的长度不够，直接返回
			if (frameLength > this->dataLength)
				return false;
			this->Crc = this->data[this->frameLength - 1];
			this->Crc <<= 8;
			this->Crc |= this->data[this->frameLength - 2];

			if (this->VerifyCheckCode())
			{				
				return true;
			}
			else
			{
				this->RemoveOneFrame();
				return false;
			}
		}
		return false;
	}
	void CreateCheckCode()
	{
		Crc = Code + frameLength;
		for (uint8_t i = 0; i < frameLength; i++)
			Crc += data[i];
	}
};

#endif // !_DATAFRAMEMODBUS_H
