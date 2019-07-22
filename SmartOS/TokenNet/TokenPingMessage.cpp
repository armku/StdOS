﻿#include "TokenPingMessage.h"

#include "Message\BinaryPair.h"

TokenPingMessage::TokenPingMessage()
{
	LocalTime	= DateTime::Now().TotalMs();
	ServerTime	= 0;
	Data		= nullptr;
}

TokenPingMessage::TokenPingMessage(const TokenPingMessage& msg) : MessageBase(msg)
{
	LocalTime	= msg.LocalTime;
	ServerTime	= msg.ServerTime;
	Data		= msg.Data;
}

// 从数据流中读取消息
bool TokenPingMessage::Read(Stream& ms)
{
	BinaryPair bp(ms);
	bp.Get("Time", LocalTime);
	bp.Get("ServerTime", ServerTime);

	return true;
}

// 把消息写入数据流中
void TokenPingMessage::Write(Stream& ms) const
{
	BinaryPair bp(ms);
	bp.Set("Time", LocalTime);

	if(Data) bp.Set("Data", *Data);
}

#if DEBUG
// 显示消息内容
String& TokenPingMessage::ToStr(String& str) const
{
	str += "Ping";
	if (Reply) str += '#';

	DateTime dt;
	dt.ParseMs(LocalTime);
	str	= str + " Time=" + dt;

	// 服务器响应包含服务器时间
	if(Reply)
	{
		DateTime dt2;
		dt2.ParseMs(ServerTime);
		str	= str + " ServerTime=" + dt2;
	}

	if (Data) str = str + "Data=" + Data->Length();

	return str;
}
#endif
