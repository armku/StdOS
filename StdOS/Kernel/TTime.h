#ifndef __TIME_H__
#define __TIME_H__

#include "Kernel\Sys.h"

// 时间类
// 使用双计数时钟，TIMx专用于毫秒级系统计时，SysTick用于微秒级延迟，秒级以及以上采用全局整数累加
// 这样子可以避免频繁使用微秒时带来长整型乘除法
class TTime
{
public:
    uint32_t	Seconds;		// 全局秒数，系统启动后总秒数。累加
	uint64_t	Milliseconds;	// 全局毫秒数，系统启动后总毫秒（1000ms整部分）。
    uint32_t	BaseSeconds;	// 基准秒数。系统启动时相对于1970年的秒数，时间调节，加上Seconds得到当前时间Now()
    //uint8_t	Ticks;			// 每微秒的时钟滴答数
	uint8_t	Index;			// 定时器
#if ! (defined(STM32F0) || defined(GD32F150))
	uint8_t	Div;			// 分频系数。最大分频64k，无法让大于64M主频的芯片分配得到1k时钟
#endif

    TTime();

	void Init();

    uint32_t CurrentTicks() const;	// 当前滴答时钟
	uint64_t Current() const; 		// 当前毫秒数

	uint32_t TicksToUs(uint32_t ticks) const;
	uint32_t UsToTicks(uint32_t us) const;
};

void delayMicroseconds(uint32_t us);//us延时，100us以下精确
void delay(uint64_t ms);//延时毫秒
uint64_t millis();//开机到现在的毫秒数
uint64_t micros();//开机到现在的微妙数

extern TTime Time;
// 时间开支。借助滴答进行精确计算
class TimeCost
{
public:
	uint64_t	Start;		// 开始时间，毫秒
	uint32_t	StartTicks;	// 开始滴答

	TimeCost();

	void Reset();
	int Elapsed() const;	// 逝去的时间，微秒
	void Show(cstring format = NULL) const;
};

/*
开发历史：

2015-10-05
系统最频繁使用的函数Current()存在除法性能问题，原采用滴答定时器8分频，若干个滴答才得到一个微秒，必然存在一个除法运算。
而任务调度系统等各个地方大量调用Current以获取当前时间，特别是中断函数内部直接或间接调用，造成极大的性能损耗。
根据黄总@405803243的指点，修改为双定时器架构。
系统时钟采用基本定时器，任意分频，很方便凑出每个计数表示1毫秒，彻底根除Current除法问题。
另外，滴答定时器用于微秒级高精度延迟，应用于硬件驱动开发等场合。
*/

#endif
