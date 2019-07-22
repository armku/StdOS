﻿#ifndef __ShunCom_H__
#define __ShunCom_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"
#include "Device\Power.h"
#include "Net\ITransport.h"
#include "Message\DataStore.h"

// 上海顺舟Zigbee协议
// 主从模式时，主站可指定从站地址或广播，从站发送只有主站收到
class ShunCom : public PackPort, public Power
{
public:
	OutputPort	Reset;	// 复位
	IDataPort*	Led;	// 指示灯

	OutputPort	Power;	// 电源
	OutputPort	Sleep;	// 睡眠
	OutputPort	Config;	// 配置

	byte	AddrLength;	// 地址长度。默认0，主站建议设为2

	ShunCom();

	void Init(ITransport* port, Pin rst = P0);

	//virtual const String ToString() const { return String("ShunCom"); }

	// 进入配置模式
	bool EnterConfig();
	// 退出配置模式
	void ExitConfig();
	// 读取配置信息
	void ShowConfig();
	void EraConfig();
	
	void PrintSrc(bool flag);
	// 设置设备的类型：00代表中心、01代表路由，02代表终端
	void SetDevice(byte kind);
	// 设置无线频点，注意大小端，ShunCom是小端存储
	void SetChannel(byte chn);
	// 设置发送模式00为广播、01为主从模式、02为点对点模式
	void SetSend(byte mode);
	// 进入配置PanID,同一网络PanID必须相同
    void SetPanID(ushort id);
	//还原zigbee默认配置
	void ShunComReset();

public:
	// Open 执行的外部配置函数
	Action	ExternalCfg;
private:
	virtual bool OnOpen();
    virtual void OnClose();

	// 电源等级变更（如进入低功耗模式）时调用
	virtual void ChangePower(int level);

	// 引发数据到达事件
	virtual uint OnReceive(Buffer& bs, void* param);

	virtual bool OnWriteEx(const Buffer& bs, const void* opt);
};
#endif
