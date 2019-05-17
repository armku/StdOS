#include "../Esp8266.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"
#include "Core/RingBuffer.h"

#define _ESP8266EST_CPP
#ifdef _ESP8266EST_CPP

USARTHAL usart333(COM3,115200);
Esp8266 esp;

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
		switch (esp.step)
		{
		case 0:
			break;
		default:
			break;
		}
		FlagInFrame = 0;
		FlagIdleCnt = 0;
	}
}

void Esp8266Routin(void* param)
{
	char cStr[100] = { 0 };
	switch (esp.step)
	{
	case 0:
		//debug_printf("\r\n 正在配置 ESP8266 ......\r\n");
		esp.CH_PD = 1;
		esp.RST = 1;
		esp.step++;
		break;
	case 1:
		esp.step = 0;
		UART3_send_data((uint8_t*)"AT",2);
		break;
	default:
		break;
	}
}

void Esp8266TestInit()
{
	esp.SetPinCH_PD(PG13);
	esp.SetPinRST(PG14);
	esp.Init();

	debug_printf("\r\n WF-ESP8266 WiFi模块测试例程\r\n"); //打印测试例程提示信息)

	FlagInFrame = false;
	Sys.AddTask(checkComRoutin, 0, 0, 10, "EspCOmCHK");
	Sys.AddTask(Esp8266Routin, 0, 0, 500, "EspRoutin");
}
#endif
