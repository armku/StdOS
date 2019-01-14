#ifndef _DATAFRAME_H
#define _DATAFRAME_H

#include "../Core/Type.h"

#define MAX_FRAME_DATA_LENGTH 50
#define FRAME_HEADER 0xAA

struct DataFrame
{
public:
	const uint8_t header;
	uint8_t dataLength; //��ǰ���ݳ���
	uint8_t data[MAX_FRAME_DATA_LENGTH];//��ǰ����
	bool isUpdated;

public:
	DataFrame() :header(FRAME_HEADER)                //constructor
	{
		isUpdated = false;
	}
};

#endif // !_DATAFRAME_H

