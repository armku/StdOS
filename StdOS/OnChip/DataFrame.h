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
};

#endif // !_DATAFRAME_H

