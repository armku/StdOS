/*
init some hardware
*/
#ifndef _BSPPLATFORM_H
#define _BSPPLATFORM_H
#include "Core\Queue.h"
#ifdef __cplusplus
extern "C" {
#endif
	typedef enum
    {
        Rising	= 0x01,	// 上升沿
        Falling	= 0x02,	// 下降沿
		Both	= 0x03	// 上升下降沿
    }Trigger;
	void SetEXIT(int pinIndex, bool enable, Trigger trigger);

	void BspPlatformInit();
	void TimeTickInit();//系统用定时器初始化
	
	int CurrentTick();
	uint32_t CurrentTicks1();
	
	void SerialPrintInit();//调试串口初始化
	void com3send();
	
	extern Queue	Txx1;
	extern Queue	Rxx1;

#ifdef __cplusplus
}
#endif
#endif // !_BSPPLATFORM_H
