#include "Sys.h"
#include "WaitHandle.h"

#define WAITHANDLETEST
#ifdef WAITHANDLETEST

void WaitHandleTest()
{
	WaitHandle handle;
	handle.WaitOne(3000);
	// handle.Result = true;	// ���ȴ�3000ms���������������Result����Ϊtrue
}
#endif
