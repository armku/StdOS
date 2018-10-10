#include "Type.h"
#include "Security/Crc.h"

#ifndef _DATAFRAMEMODBUS_H
#define _DATAFRAMEMODBUS_H

#define MAX_FRAME_DATA_LENGTH 200

struct DataFrameModbus
{
public:
	uint8_t devid;
	uint8_t fnCode;
	uint8_t dataLength;
	uint16_t regAddr;
	uint16_t regLength;
	uint8_t data[MAX_FRAME_DATA_LENGTH];
	bool isUpdated;
	uint16_t checkSum;
	int frameLength;//当前数据帧长度

public:
	DataFrameModbus()                 //constructor
	{
		isUpdated = false;
	}

	bool VerifyCheckCode()
	{
		auto crcnew = Crc::CRC16RTU(data, dataLength-2);
		if (crcnew == this->checkSum)
			return true;
		else
			return false;
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
				frameLength = data[6] + frameLength;
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
			//需要的长度不够，直接返回
			if (frameLength > this->dataLength)
				return false;
			this->checkSum = this->data[this->frameLength - 1];
			this->checkSum <<= 8;
			this->checkSum |= this->data[this->frameLength - 2];

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
