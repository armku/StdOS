﻿#ifndef __TinyClient_H__
#define __TinyClient_H__

#include "Kernel\Sys.h"

#include "TinyController.h"
#include "TinyConfig.h"

#include "Message\DataStore.h"

// 微网客户端
class TinyClient
{
public:
	TinyController* Control;
	TinyConfig*	Cfg;

	bool		Opened;		// 是否已经打开
	bool		Joining;	// 是否正在组网
	byte		Server;		// 服务端地址
	ushort		Type;		// 设备类型。两个字节可做二级分类
	ByteArray	Password;	// 通讯密码

	UInt64		LastSend;	// 最后发送时间ms
	UInt64		LastActive;	// 最后活跃时间
	ushort		HardCrc;	// 硬件ID校验
	bool		Encryption;	// 是否加密

	TinyClient(TinyController* control);

	void Open();
	void Close();

	// 发送消息
	bool Send(TinyMessage& msg);
	bool Reply(TinyMessage& msg);
	void OnReceive(TinyMessage& msg, TinyController& ctrl);

	// 收到功能消息时触发
	MessageHandler	Received;
	void*			Param;

	static TinyClient* Current;

	// 数据区
public:
	DataStore	Store;		// 数据存储区

	bool Report(int offset, byte dat);
	bool Report(int offset, const Buffer& bs);

	int NextReport;	// 下次上报偏移，0不动
	int ReportLength;	// 下次上报数据长度
	void ReportAsync(int offset, int length = 1);

private:
	uint _TaskID;

private:
	void OnWrite(const TinyMessage& msg);
	void OnRead(const TinyMessage& msg);

	void GetDeviceKey(byte id, Buffer& key);

	// 常用系统级消息
public:
	// 组网
	ushort		TranID;		// 组网会话
	void Join();
	bool OnJoin(const TinyMessage& msg);
	void DisJoin();
	bool OnDisjoin(const TinyMessage& msg);

	// 心跳
	void Ping();
	bool OnPing(const TinyMessage& msg);
};

#endif
