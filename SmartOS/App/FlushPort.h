#ifndef __FlushPort_H__
#define __FlushPort_H__

#include "Kernel\Sys.h"
#include "OnChip\Port.h"

// 闪烁端口
// 默认慢闪，Start后快闪一定时间，-1时间表示一直快闪，Stop停止
// 支持数据操作指令，参数为快闪毫秒数
class FlushPort 
{
private:
	uint	_tid; 

public:
	Port*	Port; 

	int		Fast;	// 快闪间隔，默认50毫秒
	int		Slow;	// 慢闪间隔，默认1000毫秒
	int		Count;	// 剩余快闪次数

	FlushPort();
	virtual ~FlushPort();

	void Start(int ms = 1000);
	// mode 0xff  为不改变Port状态  0为Write false  1为Write true
	void Stop(byte mode = 0xff);
	void Flush();

	virtual int Write(byte* data);
};

#endif
