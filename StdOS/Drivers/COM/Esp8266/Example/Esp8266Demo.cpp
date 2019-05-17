#include "../Esp8266.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"
#include "Core/RingBuffer.h"

#define _ESP8266EST_CPP
#ifdef _ESP8266EST_CPP

USARTHAL usart222(COM3,115200);

#include "../HAL/STM32F1/ARCH_UART.h"
uint8_t   loop_bufcom3[64] = { 0 };                             //定义环形缓冲区
RingBuffer ringRcvcom3(loop_bufcom3, ArrayLength(loop_bufcom3));
bool FlagInFrame;//接收到完整一帧数据
//向环形缓冲区【写】一字节数据
static void write_loop_buf(uint8_t dat)
{
	ringRcvcom3.Put(dat);
}

void checkComRoutin(void* param)
{
	static int RxCnt = 0;
	static int RxCntOld = 0;
	static int FlagIdleCnt = 0;//空闲时间

	RxCnt = ringRcvcom3.length;
	if (RxCnt != RxCntOld)
	{
		RxCntOld = RxCnt;
		return;
	}
	if (++FlagIdleCnt > 10)
	{
		FlagInFrame = 1;
	}
}

void Esp8266Routin(void* param)
{
	
}

void Esp8266TestInit()
{
	FlagInFrame = false;
	Sys.AddTask(checkComRoutin, 0, 0, 10, "EspCOmCHK");
	Sys.AddTask(Esp8266Routin, 0, 0, 100, "EspRoutin");
}
#endif
