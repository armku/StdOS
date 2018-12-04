#include "Type.h"
#include "Security/Crc.h"

#ifndef _DATAFRAMEMODBUS_H
#define _DATAFRAMEMODBUS_H

#define MAX_FRAMEMODBUS_DATA_LENGTH 400

struct DataFrameModbus
{
public:
	uint8_t devid;
	uint8_t fnCode;
	uint8_t dataLength;
	uint16_t regAddr;
	uint16_t regLength;
	uint8_t data[MAX_FRAMEMODBUS_DATA_LENGTH];
	bool isUpdated;
	uint16_t checkSum;
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
		auto crcnew = Crc::CRC16RTU(data, frameLength-2);
		debug_printf("%x %x\n",crcnew, checkSum);
		if (crcnew == this->checkSum)
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
		if (frameLength<0 || frameLength>dataLength)
			return false;
		for (int i = 0; i < dataLength - frameLength; i++)
		{
			this->data[i] = this->data[i + frameLength];
		}
		dataLength -= frameLength;
		this->frameLength = 0;
		return true;
	}
	bool CheckFrame()
	{
		if (dataLength >= 8)
		{
			this->devid = this->data[0];
			this->fnCode = this->data[1];
			this->regAddr = this->data[2];
			this->regAddr <<= 8;
			this->regAddr |= this->data[3];
			this->regLength = this->data[4];
			this->regLength <<= 8;
			this->regLength |= this->data[5];

			this->frameLength = 8;
			switch (this->fnCode)
			{
			case 0X02:
			case 0X01:
			case 0X05:
			case 0X04:
			case 0X03:
			case 0X06:
				frameLength = 8;
				break;
			case 0x0F://多个写 0 区输出继电器(指令代码: 0X0F) 
			case 0x10://多个写 4 区输出寄存器(指令代码: 0X10) 
				frameLength = data[6] + frameLength + 1;
				debug_printf("%d %d\n", frameLength, data[6]);
				break;
			default:
				frameLength = 0;
				break;
			}
			if (frameLength == 0)
			{
				//非法指令，直接清空缓冲区
				frameLength = this->dataLength;
				this->RemoveOneFrame();
				return false;
			}
			debug_printf("frameLength %d %d\n", frameLength, dataLength);
			//需要的长度不够，直接返回
			if (frameLength > this->dataLength)
				return false;
			this->checkSum = this->data[this->frameLength - 1];
			this->checkSum <<= 8;
			this->checkSum |= this->data[this->frameLength - 2];

			if (this->VerifyCheckCode())
			{
				debug_printf("true\n");
				return true;
			}
			else
			{
				debug_printf("false\n");
				this->RemoveOneFrame();
				return false;
			}
		}
		return false;
	}
	void CreateCheckCode()
	{
		checkSum = fnCode + frameLength;
		for (uint8_t i = 0; i < frameLength; i++)
			checkSum += data[i];
	}
	DataFrameModbus& operator=(const DataFrameModbus &df)
	{
		fnCode = df.fnCode;
		dataLength = df.dataLength;
		isUpdated = df.isUpdated;
		checkSum = df.checkSum;
		for (uint8_t i = 0; i < dataLength; i++)
			data[i] = df.data[i];
		return *this;
	}
};

#endif // !_DATAFRAMEMODBUS_H
