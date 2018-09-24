#ifndef __BlinkPort_H__
#define __BlinkPort_H__

#include "Kernel\Sys.h"
#include "OnChip\Port.h"

// 闪烁端口
class BlinkPort
{
private:
	uint32_t	_tid; //0

public:
	OutputPort*	Ports[0x10]; //
	int		Count; //17

	bool	First;		// 开始状态 18 72
	int		Times;		// 变更次数。高低算两次 19
	int		Interval1;	// 间隔1(单位ms)。第一次改变状态后暂停时间，默认100ms 20
	int		Interval2;	// 间隔2(单位ms)。第二次改变状态后暂停时间，默认300ms 21

	bool 	Current;	// 当前值 22 88 92
	int		Index;		// 当前闪烁次数  23

	BlinkPort();
	~BlinkPort();

	void Add(OutputPort* port);
	void Start();
	void Stop();
	void Blink();
};

#endif
