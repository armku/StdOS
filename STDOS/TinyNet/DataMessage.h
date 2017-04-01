﻿#ifndef __DataMessage_H__
#define __DataMessage_H__

#include "Kernel\Sys.h"
#include "Message\DataStore.h"
#include "Message\Message.h"

// 数据消息
class DataMessage
{
public:
	int	Offset;
	int	Length;

	DataMessage(const Message& msg, Stream* dest, bool isTokenMsg = false);

	bool ReadData(const DataStore& ds);
	bool WriteData(DataStore& ds, bool withData);

	bool ReadData(const Buffer& bs);
	bool WriteData(Buffer& bs, bool withData);

private:
	Stream	_Src;
	Stream*	_Dest;

	bool Write(int remain);
};

#endif
