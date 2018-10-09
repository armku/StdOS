#include "Type.h"
#include "Security/Crc.h"

#ifndef _DATAFRAMEMODBUS_H
#define _DATAFRAMEMODBUS_H

#define MAX_FRAME_DATA_LENGTH 50

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

public:
	DataFrameModbus()                 //constructor
	{
		isUpdated = false;
	}

	bool VerifyCheckCode()
	{
		auto crcnew = Crc::CRC16RTU(data, dataLength);
		uint8_t code = fnCode + dataLength;
		for (uint8_t i = 0; i < dataLength; i++)
			code += data[i];
		if (code == checkSum)
			return true;
		else
			return false;
	}
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
