#include "Type.h"
#include "Security/Crc.h"
#include "../DataFrame.h"

#ifndef _DATAFRAMEEspDemoLink_H
#define _DATAFRAMEEspDemoLink_H

struct DataFrameEspDemoLink:public DataFrame
{
public:
	int frameLength;//当前数据帧长度
	int Cnt;//数据帧数量

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
		this->dataLength = 0;//测试代码
		return true;
	}
	//查找AT指令
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
		static uint64_t oldTime = 0;
		uint64_t curTime;
		curTime = Sys.Ms();
		if (curTime - oldTime < 10)
			return false;
		oldTime = curTime;

		this->frameLength = this->dataLength;

		if (this->frameLength == 0)
		{
			oldTime = curTime;
			return false;
		}
		else
			return true;

		/*if (dataLength >= 2)
		{
			this->frameLength = this->dataLength;

			return true;
			auto end = this->SearchATEnd(dataLength);
			if (end >= 0)
			{
				this->frameLength = end + 1;
				return true;
			}
			else
				return false;
		}
		return false;*/
	}
};

#endif // !_DATAFRAMEMODBUS_H
