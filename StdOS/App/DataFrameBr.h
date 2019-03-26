#include "Security/CheckSum.h"

#ifndef _DATAFRAMEBR_H
#define _DATAFRAMEBR_H

#define MAX_FRAME_DATA_LENGTH 200

struct DataFrameBr
{
public:
	uint8_t fnCode;
	uint8_t dataLength;
	uint8_t data[MAX_FRAME_DATA_LENGTH];
	bool isUpdated;
	uint8_t checkSum;
	int frameLength;//当前数据帧长度
public:
	DataFrameBr()                 //constructor
	{
		isUpdated = false;
	}

	bool VerifyCheckCode()
	{
		uint8_t crcnew = CheckSum::XorCheckSum8(this->data, frameLength - 1);
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
		if (dataLength >= 3)
		{
			this->frameLength = this->data[0];
			this->fnCode = this->data[1];
					
			//需要的长度不够，直接返回
			if (frameLength > this->dataLength)
				return false;
			this->checkSum = this->data[this->frameLength - 1];
			
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
	DataFrameBr& operator=(const DataFrameBr &df)
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

#endif // !_DATAFRAMEBR_H
