#ifndef _DATAFRAME_H
#define _DATAFRAME_H

#include "../Core/Type.h"

#define MAX_FRAME_DATA_LENGTH 50
#define FRAME_HEADER 0xAA

//frame struct:
// | header | fnCode |  dataLength | data1 | ...... | datan | checkSum | 

struct DataFrame
{
public:
	const uint8_t header;
	uint8_t fnCode;
	uint8_t dataLength; //当前数据长度
	uint8_t data[MAX_FRAME_DATA_LENGTH];//当前数据
	bool isUpdated;
	uint8_t checkSum;

public:
	DataFrame() :header(FRAME_HEADER)                //constructor
	{
		isUpdated = false;
	}

	bool VerifyCheckCode()
	{
		uint8_t code = header + fnCode + dataLength;
		for (uint8_t i = 0; i<dataLength; i++)
			code += data[i];
		if (code == checkSum)
			return true;
		else
			return false;
	}
	void CreateCheckCode()
	{
		checkSum = header + fnCode + dataLength;
		for (uint8_t i = 0; i<dataLength; i++)
			checkSum += data[i];
	}
	DataFrame& operator=(const DataFrame &df)
	{
		//header = df.header;
		fnCode = df.fnCode;
		dataLength = df.dataLength;
		isUpdated = df.isUpdated;
		checkSum = df.checkSum;
		for (uint8_t i = 0; i<dataLength; i++)
			data[i] = df.data[i];
		return *this;
	}
};

#endif // !_DATAFRAME_H

