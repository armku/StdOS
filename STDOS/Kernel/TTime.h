#pragma once

#include "Kernel\Sys.h"

class TTime
{
	private:
    static void OnHandler(ushort num, void* param);

public:
    uint	Seconds;		// 全局秒数，系统启动后总秒数。累加
	UInt64	Milliseconds;	// 全局毫秒数，系统启动后总毫秒（1000ms整部分）。累加
    uint	BaseSeconds;	// 基准秒数。系统启动时相对于1970年的秒数，时间调节，加上Seconds得到当前时间Now()
    //byte	Ticks;			// 每微秒的时钟滴答数
	byte	Index;			// 定时器
#if ! (defined(STM32F0) || defined(GD32F150))
	byte	Div;			// 分频系数。最大分频64k，无法让大于64M主频的芯片分配得到1k时钟
#endif

	Func OnInit;
	Func OnLoad;
	Func OnSave;
	typedef void (*FuncInt)(int);
	FuncInt OnSleep;

    TTime();

	void UseRTC();			// 使用RTC，必须在Init前调用
	void Init();

    uint CurrentTicks() const;	// 当前滴答时钟
	UInt64 Current() const; 		// 当前毫秒数
	void SetTime(UInt64 seconds);	// 设置时间

	void Sleep(int ms, bool* running = nullptr) const;
    void Delay(int us) const;	// 微秒级延迟

	uint TicksToUs(uint ticks) const;
	uint UsToTicks(uint us) const;
		
	public:							
		UInt64 Ms() const;//系统启动以来的毫秒数		
		UInt64 mCurrent;//系统启动以来的毫秒数				
};
extern TTime Time; //系统时间，不建议用户直接使用
// 时间轮。用于超时处理
class TimeWheel
{
public:
	UInt64	Expire;		// 到期时间，毫秒
	ushort	Sleep;		// 睡眠时间，默认0毫秒

	TimeWheel(uint ms);

	void Reset(uint ms);

	// 是否已过期
	bool Expired();
};
class TimeCost
{
	public:
	UInt64	Start;		// 开始时间，毫秒
	uint	StartTicks;	// 开始滴答

	TimeCost();

	void Reset();
	int Elapsed() const;	// 逝去的时间，微秒
	void Show(cstring format = nullptr) const;
};
