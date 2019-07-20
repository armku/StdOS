#include "Security/CheckSum.h"

#ifndef _DATAFRAMESENSOR_H
#define _DATAFRAMESENSOR_H

#define MAX_FRAMESENSOR_DATA_LENGTH 200

struct DataFrameBrSensor
{
public:
	uint8_t devID;
	uint8_t fnCode;
	uint8_t dataLength;
	uint8_t data[MAX_FRAMESENSOR_DATA_LENGTH];
	bool isUpdated;
	uint8_t checkSum;
	int frameLength;//��ǰ����֡����
public:
	DataFrameBrSensor()                 //constructor
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
	//���ô���Ĵ���ֵ����ַ��0��ʼ����
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

			//��Ҫ�ĳ��Ȳ�����ֱ�ӷ���
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
	DataFrameBrSensor& operator=(const DataFrameBrSensor &df)
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

#endif // !_DATAFRAMESENSOR_H
