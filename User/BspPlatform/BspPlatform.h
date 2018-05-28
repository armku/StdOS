/*
init some hardware
*/
#ifndef _BSPPLATFORM_H
#define _BSPPLATFORM_H
#include "Core\Queue.h"
#ifdef __cplusplus
extern "C" {
#endif
	void BspPlatformInit();
		
	void SerialPrintInit();//调试串口初始化
#ifdef __cplusplus
}
#endif
#endif // !_BSPPLATFORM_H
