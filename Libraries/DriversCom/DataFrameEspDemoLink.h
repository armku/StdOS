#include "Type.h"
#include "Security/Crc.h"

#ifndef _DATAFRAMEEspDemoLink_H
#define _DATAFRAMEEspDemoLink_H

#define MAX_FRAMEMODBUS_DATA_LENGTH 400

struct DataFrameEspDemoLink
{
public:
	uint8_t dataLength;
	uint8_t data[MAX_FRAMEMODBUS_DATA_LENGTH];
	bool isUpdated;
	int frameLength;//��ǰ����֡����
	int Cnt;//����֡����

public:
	DataFrameEspDemoLink()                 //constructor
	{
		isUpdated = false;
		Cnt = 0;
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
	//����ATָ��
	int SearchATEnd(int len)
	{
		for (int i = 0; i < len-1; i++)
		{
			if (data[i] == '\r'&&data[i + 1] == '\n')
				return i;
		}
		return -1;
	}
	bool CheckFrame()
	{
		if (dataLength >= 2)
		{
			auto end = this->SearchATEnd(dataLength);
			if (end >= 0)
				this->frameLength = end + 1;
			else
				return false;			
			
			/*if (this->VerifyCheckCode())
			{
				return true;
			}
			else
			{
				this->RemoveOneFrame();
				return false;
			}*/
		}
		return false;
	}
};

#endif // !_DATAFRAMEMODBUS_H
