#include "../ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"
#include "Core/RingBuffer.h"
#include "Buffer.h"

#define _MODBUSLINKEST_CPP
#ifdef _MODBUSLINKEST_CPP

void DealRcv(char* buf, int length);

/******************************************串口参数开始**************************************************/
USARTHAL usart222(COM2, 115200);
#include "../HAL/STM32F1/ARCH_UART.h"
static uint8_t   loop_bufcom2[64] = { 0 };                             //定义环形缓冲区
static RingBuffer ringRcvcom2(loop_bufcom2, ArrayLength(loop_bufcom2));
static bool FlagInFrame;//接收到完整一帧数据
//向环形缓冲区【写】一字节数据
static void write_loop_buf(uint8_t dat)
{
	ringRcvcom2.Put(dat);
}
static void checkComRoutin(void* param)
{
	static int RxCnt = 0;
	static int RxCntOld = 0;
	static int FlagIdleCnt = 0;//空闲时间

	/*if (!esp.FlagRcvAuto)
		return;*/

	RxCnt = ringRcvcom2.length;
	if (RxCnt != RxCntOld)
	{
		RxCntOld = RxCnt;
		return;
	}
	if (RxCnt == 0)
		return;

	if (++FlagIdleCnt > 10)
	{
		FlagInFrame = 1;

		int readlen = ringRcvcom2.Get((char*)loop_bufcom2, ArrayLength(loop_bufcom2));
		DealRcv((char*)loop_bufcom2, readlen);
		
		FlagInFrame = 0;
		FlagIdleCnt = 0;
	}
}
/******************************************串口参数结束**************************************************/
/******************************************MODBUS开始**************************************************/
//测试 01 03 00 00 00 0A C5 CD
ModbusSlaveLink modbusSlave(usart222);
Port p485dr;
uint16_t RegInputu16[144]; //输入寄存器
uint16_t RegHoilding16[60];

static void DealRcv(char* buf, int length)
{
	Buffer(loop_bufcom2, length).ShowHex(true);
	if (length >= 8)
	{
		uint16_t crccal = Crc::CRC16RTU(loop_bufcom2, length - 2);
		uint16_t crcrcv = loop_bufcom2[length - 1];
		crcrcv <<= 8;
		crcrcv |= loop_bufcom2[length - 2];
		if (crcrcv == crccal)
		{
			modbusSlave.rxFrame.CheckFrame((char*)loop_bufcom2, length);
			debug_printf("Rcv Frame OK\r\n");
		}
		else
		{
			debug_printf("crc Error:%04x\r\n", crccal);
		}
	}
	else
	{
		debug_printf("Rcv Length Error:%d\r\n", length);
	}
}


static void ModbusSlaveLinkRoutin(void* param)
{
#if 0
	if ((modbusSlave.com.RxSize() > 0)&& modbusSlave.CheckFrame())
	{
		modbusSlave.DealFrame();
		modbusSlave.rxFrame.RemoveOneFrame();
	}
	else if ((modbusSlave.com.RxSize() > 0 || (modbusSlave.rxFrame.Length > 0)) && (millis() - modbusSlave.com.LastRcvTime) > 5)
	{
		debug_printf("Clear RcvBuffer rxFrame:%d  com.RxSize:%d\n", modbusSlave.rxFrame.Length,modbusSlave.com.RxSize());
		modbusSlave.rxFrame.Length = 0;
		//modbusSlave.com.ClearRxBuf();
	}
	else {}
#endif
}
/******************************************MODBUS结束**************************************************/
void ModbusSlaveLinkTestInit()
{
	p485dr.SetPin(PC2);
	p485dr.pinMode(GPIO_Out_PP);
	p485dr = 0;//接收模式
	modbusSlave.com.RS485 = &p485dr;
	modbusSlave.id = 1;
	modbusSlave.SetRegInput(0, ArrayLength(RegInputu16), RegInputu16, 0);
	modbusSlave.SetRegHoid(0, ArrayLength(RegHoilding16), RegHoilding16, 0);

	UART_2_rcv_IRQ = write_loop_buf;

	RegInputu16[0] = 12;
	RegInputu16[1] = 13;

	Sys.AddTask(checkComRoutin, 0, 0, 10, "ModbusCHK");
	Sys.AddTask(ModbusSlaveLinkRoutin, 0, 0, 1, "ModbusSlaveLinkRoutin");
}
#endif
