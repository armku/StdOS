#include "Sys.h"
#include "WaitHandle.h"

#define WAITHANDLETEST
#ifdef WAITHANDLETEST

void WaitHandleTest()
{
	WaitHandle handle;
	handle.WaitOne(3000);
	// handle.Result = true;	// 最大等待3000ms，除非其他任务把Result设置为true
}
#endif
