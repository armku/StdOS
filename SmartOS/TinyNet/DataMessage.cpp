﻿#include "DataMessage.h"

DataMessage::DataMessage(const Message& msg, Stream* dest, bool isTokenMsg) : _Src(msg.ToStream())
{
	_Dest	= dest;

	// TokenMassge 数据区第一个字节是目的地址
	if(isTokenMsg)_Dest->Write(_Src.ReadByte());

	Offset	= _Src.ReadEncodeInt();
	Length	= 0;

	// 读取请求、写入响应、错误响应 等包含偏移和长度
	byte code	= msg.Code & 0x0F;
	if((code == 0x05 && !msg.Reply) || (code == 0x06 && msg.Reply) || msg.Error)
		Length	= _Src.ReadEncodeInt();
}

// 读取数据
bool DataMessage::ReadData(const DataStore& ds)
{
	return ReadData(ds.Data);
}

// 读取数据
bool DataMessage::ReadData(const Buffer& bs)
{
	if(!_Dest) return false;

	TS("DataMessage::ReadData");

	auto& ms	= *_Dest;
	int remain	= bs.Length() - Offset;
	if(remain < 0)
	{
		ms.Write((byte)2);
		ms.WriteEncodeInt(Offset);
		ms.WriteEncodeInt(Length);

		return false;
	}
	else
	{
		ms.WriteEncodeInt(Offset);
		if(Length > remain) Length = remain;
		//if(Length > 0) ms.Write(bs.GetBuffer(), Offset, Length);
		if(Length > 0) ms.Write(bs.Sub(Offset, Length));

		return true;
	}
}

// 写入数据
bool DataMessage::WriteData(DataStore& ds, bool withData)
{
	TS("DataMessage::WriteData");

	Length	= _Src.Remain();
	if(_Dest && !Write(ds.Data.Length() - Offset)) return false;

	Buffer dat(_Src.Current(), Length);
	ds.Write(Offset, dat);

	// 如果携带数据，则把这一段数据附加到后面
	if(_Dest && withData) _Dest->Write(ds.Data.Sub(Offset, Length));

	return true;
}

// 写入数据
bool DataMessage::WriteData(Buffer& bs, bool withData)
{
	TS("DataMessage::WriteData");

	// 剩余可写字节数
	Length	= _Src.Remain();
	if(_Dest && !Write(bs.Length() - Offset)) return false;

	//Buffer dat(_Src.Current(), Length);
	//bs.Copy(Offset, dat, 0, -1);
	bs.Copy(Offset, _Src.Current(), Length);

	// 如果携带数据，则把这一段数据附加到后面
	if(_Dest && withData) _Dest->Write(bs.Sub(Offset, Length));

	return true;
}

// 写入数据
bool DataMessage::Write(int remain)
{
	auto& ms	= *_Dest;
	// 剩余可写字节数
	if(remain < 0)
	{
		ms.Write((byte)2);
		ms.WriteEncodeInt(Offset);
		ms.WriteEncodeInt(Length);

		return false;
	}
	else
	{
		ms.WriteEncodeInt(Offset);

		if(Length > remain) Length = remain;
		ms.WriteEncodeInt(Length);

		return true;
	}
}
