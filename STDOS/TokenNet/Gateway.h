﻿#ifndef __SERVER__H__
#define __SERVER__H__

#include "Kernel\Sys.h"
#include "Device\Port.h"
#include "Net\ITransport.h"
#include "Message\DataStore.h"
#include "TokenClient.h"

#include "TinyNet\TinyServer.h"

#include "DeviceMessage.h"
#include "TokenNet/TokenDataMessage.h"

// 网关服务器
class Gateway
{
public:
	TinyServer*		Server;		// 内网服务端
	TokenClient*	Client;		// 外网客户端
	IDataPort*		Led;		// 指示灯
	DevicesManagement * pDevMgmt;	// 设备集合管理的指针

	Gateway();
	~Gateway();

	bool Running;
	void Start();
	void Stop();

	// 收到功能消息时触发
	MessageHandler Received;

	// 数据接收中心
	bool OnLocal(const TinyMessage& msg);
	bool OnRemote(const TokenMessage& msg);

	/******** 远程网业务逻辑 ********/
	// 学习模式 0x20
	void SetMode(uint sStudy);
	uint GetMode();
	void Clear();
	bool OnMode(const Message& msg);

	/******** 本地网业务逻辑 ********/
	// 设备发现
	// bool OnDiscover(const TinyMessage& msg);

	static Gateway*	Current;
	static Gateway* CreateGateway(TokenClient* client, TinyServer* server);

	// 学习模式
	bool InvokeStudy(const Pair& args, Stream& result);

public:
	uint	_task = 0;	// 定时任务，10秒
	int		_Study = 0;	// 自动退出学习时间，秒

	static void Loop(void* param);
	//远程调用操作
};

#endif
