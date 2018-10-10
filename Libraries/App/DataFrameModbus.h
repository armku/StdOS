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
	bool CheckFrame();
	void CreateCheckCode()
	{
		checkSum = fnCode + dataLength;
		for (uint8_t i = 0; i < dataLength; i++)
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
