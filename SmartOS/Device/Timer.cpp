﻿#include "Kernel\Sys.h"

#include "Timer.h"

// 已经实例化的定时器对象
static Timer* Timers[16] = {
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
};

Timer::Timer(TIMER index)
{
	Timers[index] = this;

	_index = index;
	OnInit();

	// 默认情况下，预分频到1MHz，然后1000个周期，即是1ms中断一次
	SetFrequency(10);

	Opened = false;
}

Timer::~Timer()
{
	Close();

	if (OnTick.Method) SetHandler(false);

	Timers[_index] = nullptr;
}

// 创建指定索引的定时器，如果已有则直接返回，默认0xFF表示随机分配
Timer* Timer::Create(byte index)
{
	TS("Timer::Create");

	byte tcount = ArrayLength(Timers);
	//byte tcount	= 46;
	// 特殊处理随机分配
	if (index == 0xFF)
	{
		// 找到第一个可用的位置，没有被使用，并且该位置定时器存在
		byte i = 0;
		for (; i < tcount && (Timers[i] || !GetTimer(i)); i++);

		if (i >= tcount)
		{
			debug_printf("Timer::Create 失败！没有空闲定时器！\r\n");
			return nullptr;
		}

		index = i;
	}

	assert(index < tcount, "index");

	if (Timers[index])
		return Timers[index];
	else
		return new Timer((TIMER)index);
}

void Timer::Open()
{
	if (Opened) return;

	TS("Timer::Open");

	OnOpen();

	Opened = true;
}

void Timer::Close()
{
	if (!Opened) return;

	TS("Timer::Close");

	debug_printf("Timer%d::Close\r\n", _index + 1);

	OnClose();

	Opened = false;
}

void Timer::Register(const Delegate<Timer&>& dlg)
{
	OnTick = dlg;

	SetHandler(dlg.Method != nullptr);
}

void Timer::OnInterrupt()
{
	OnTick(*this);
}
