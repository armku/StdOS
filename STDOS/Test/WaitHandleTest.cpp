#include "Sys.h"
#include "WaitHandle.h"

#define WAITHANDLETEST
#ifdef WAITHANDLETEST

void WaitHandletest1(void * param)
{
	static int icnt=0;
	WaitHandle handle;
	debug_printf("%03d %d\r\n",icnt,Sys.Ms());
	handle.WaitOne(300);
	//Sys.Sleep(200);
	handle.Result = true;	// 最大等待3000ms，除非其他任务把Result设置为true
	
	debug_printf("%03d %d\r\n",icnt++,Sys.Ms());
}

void WaitHandleTest()
{
	Sys.AddTask(WaitHandletest1,0,0,4000,"WaitHandle");
}
#endif
