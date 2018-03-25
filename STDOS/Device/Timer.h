#ifndef __Timer_H__
#define __Timer_H__

#include "Kernel\Sys.h"
#include "Device\Port.h"

// 定时器
class Timer
{
protected:
	uint8_t	_index;		// 第几个定时器，从0开始
	Delegate<Timer&>	OnTick;	// 带this参数

public:
	void*	_Timer;
	bool	Opened;	// 可能在中断里关闭自己

	uint16_t	Prescaler;	// 预分频。实际值，此时无需减一。
	uint32_t	Period;		// 周期。实际值，此时无需减一。

	Timer(TIMER index);

	virtual void Open();	// 开始定时器
	virtual void Close();	// 停止定时器
	virtual void Config();
	void SetFrequency(uint32_t frequency);	// 设置频率，自动计算预分频

	uint32_t GetCounter();
	void SetCounter(uint32_t cnt);		// 设置计数器值

	void Register(const Delegate<Timer&>& dlg);
	virtual void OnInterrupt();

	static void ClockCmd(int idx, bool state);

private:
	static void OnHandler(uint16_t num, void* param);

protected:

public:
	static const uint8_t	TimerCount;	// 定时器个数

	static Timer* Create(uint8_t index = 0xFF);	// 创建指定索引的定时器，如果已有则直接返回，默认0xFF表示随机分配

private:
	void OnInit();
	void OnOpen();
	void OnClose();

	static const void* GetTimer(uint8_t idx);
};

#endif
