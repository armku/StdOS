﻿#ifndef __Pulse_Port_H__
#define __Pulse_Port_H__

#include "Kernel\Sys.h"

class InputPort;

// 脉冲端口。用于计算脉冲被遮挡的时间
class PulsePort
{
public:
	InputPort*	Port;	// 输入引脚

	uint	Min;		// 最小时间间隔 单位 ms
	uint	Max;		// 最大时间间隔 单位 ms

	bool	Filter;		//是否过滤形脉冲 

	UInt64 	Start;		// 开始遮挡时间
	UInt64 	Last;		// 上一次脉冲时间
	uint 	Time;		// 遮挡时间
	uint 	Count;		// 次数
	bool	Opened;		// 是否打开

	Delegate<PulsePort&> Press;	// 被遮挡时触发

	PulsePort();
	~PulsePort();

	void Open();
	void Close();

private:
	//任务的调度
	uint	_task;

	// 内部中断函数
	void OnPress(InputPort& port, bool down);
};

#endif
