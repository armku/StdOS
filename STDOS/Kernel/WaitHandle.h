#ifndef __WaitHandle_H__
#define __WaitHandle_H__
#include "Type.h"
// 等待句柄
class WaitHandle
{
public:
	uint32_t	TaskID;	// 句柄所在任务
	void*	State;	// 用户数据 4
	bool	Result;	// 结果 8

	WaitHandle();
	
	bool WaitOne(int ms);	// 等待一个
	
	//void Reset();
	void Set();	// 设置结果
	
private:
};

#endif
