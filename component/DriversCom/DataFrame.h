#ifndef _DATAFRAME_H
#define _DATAFRAME_H

#include <stdint.h>

#define MAX_FRAME_DATA_LENGTH 100
//#define FRAME_HEADER 0xAA

struct DataFrame
{
public:
	//const uint8_t header;
	uint8_t dataLength; //��ǰ���ݳ���
	uint8_t data[MAX_FRAME_DATA_LENGTH];//��ǰ����
	bool isUpdated;

public:
	DataFrame()// :header(FRAME_HEADER)
	{
		isUpdated = false;
	}
};

#endif // !_DATAFRAME_H

