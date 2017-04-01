﻿#ifndef __TokenClient_H__
#define __TokenClient_H__

#include "Kernel\Sys.h"
#include "TokenNet\TokenConfig.h"
#include "TokenMessage.h"
#include "HelloMessage.h"

#include "TokenNet\TokenController.h"

#include "Message\DataStore.h"
#include "Message\Pair.h"

// 微网客户端
class TokenClient
{
public:
	uint	Token;	// 令牌
	bool	Opened;
	int		Status;	// 状态。0准备、1握手完成、2登录后

	UInt64	LoginTime;	// 登录时间ms
	UInt64	LastSend;	// 最后发送时间ms
	UInt64	LastActive;	// 最后活跃时间ms
	int		Delay;		// 心跳延迟。一条心跳指令从发出到收到所花费的时间
	int		MaxNotActive;	// 最大不活跃时间ms，超过该时间时重启系统。默认0

	String		Cookie;	// 访问令牌

	TokenController*		Master;		// 主通道
	List<TokenController*>	Controls;	// 从通道
	IList					Sessions;	// 会话集合
	TokenConfig*	Cfg;
	DataStore	Store;	// 数据存储区
	Dictionary<cstring, IDelegate*>	Routes;	// 路由集合

	TokenClient();

	void Open();
	void Close();

	// 启用内网功能。必须显式调用，否则内网功能不参与编译链接，以减少大小
	void UseLocal();

	// 发送消息
	bool Send(TokenMessage& msg, TokenController* ctrl = nullptr);
	bool Reply(TokenMessage& msg, TokenController* ctrl = nullptr);
	void OnReceive(TokenMessage& msg, TokenController& ctrl);
	void OnReceiveLocal(TokenMessage& msg, TokenController& ctrl);
	void LocalSend(int start, const Buffer& bs);

	// 收到功能消息时触发
	MessageHandler	Received;
	void*			Param;

	// 常用系统级消息
	// 握手广播
	void SayHello(bool broadcast);

	// 注册
	void Register();

	// 登录
	void Login();
	// 重置并上报
	void Reset(const String& reason);
	void Reboot(const String& reason);

	// Ping指令用于保持与对方的活动状态
	void Ping();

	void Read(int start, int size);
	void Write(int start, const Buffer& bs);
	void Write(int start, byte dat);
	// 必须满足 start > 0 才可以。
	void ReportAsync(int start, uint length = 1);

	// 远程调用
	void Invoke(const String& action, const Buffer& bs);

	// 远程调用委托。传入参数名值对以及结果缓冲区引用，业务失败时返回false并把错误信息放在结果缓冲区
	typedef bool(*InvokeHandler)(void* param, const Pair& args, Stream& result);
	// 注册远程调用处理器
	void Register(cstring action, InvokeHandler handler, void* param = nullptr);
	// 模版支持成员函数
	template<typename T>
	void Register(cstring action, bool(T::*func)(const Pair&, Stream&), T* target)
	{
		Register(action, *(InvokeHandler*)&func, target);
	}

	static TokenClient* Current;

	// 快速建立令牌客户端，注册默认Api
	static TokenClient* CreateFast(const Buffer& store);

	// 重启，重置
	static bool InvokeRestart(void * param, const Pair& args, Stream& result);
	static bool InvokeReset(void * param, const Pair& args, Stream& result);
	// 设置远程服务器地址
	static bool InvokeSetRemote(void * param, const Pair& args, Stream& result);
	// 获取远程服务器地址
	static bool InvokeGetRemote(void * param, const Pair& args, Stream& result);
	// 获取所有API
	static bool InvokeGetAllApi(void * param, const Pair& args, Stream& result);

private:
	bool OnHello(TokenMessage& msg, TokenController* ctrl);

	// 跳转
	bool OnRedirect(HelloMessage& msg);

	void OnRegister(TokenMessage& msg, TokenController* ctrl);

	bool OnLogin(TokenMessage& msg, TokenController* ctrl);

	bool OnPing(TokenMessage& msg, TokenController* ctrl);

	bool ChangeIPEndPoint(const NetUri& uri);

	void OnRead(const TokenMessage& msg, TokenController* ctrl);
	void OnWrite(const TokenMessage& msg, TokenController* ctrl);

	void OnInvoke(const TokenMessage& msg, TokenController* ctrl);
	bool OnInvoke(const String& action, const Pair& args, Stream& result);

private:
	uint	_task;
	uint	_taskBroadcast;	// 广播任务

	Delegate2<Message&, Controller&>	_LocalReceive;

	int		NextReport = -1;	// 下次上报偏移，-1不动
	byte	ReportLength;	// 下次上报数据长度

	void LoopTask();
	bool CheckReport();
	void CheckNet();
};

#endif
