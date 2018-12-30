#include "MqttLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"

#define _MQTTLINKEST_CPP
#ifdef _MQTTLINKEST_CPP

//���� 01 03 00 00 00 0A C5 CD
USART usart222(USART2, 115200);
MqttLink modbusSlave(usart222);
//uint16_t RegInputu16[144]; //����Ĵ���
//uint16_t RegHoilding16[60];

void MqttLinkRoutin(void* param)
{
	//if ((modbusSlave.com.RxSize() > 0) && modbusSlave.CheckFrame())
	//{
	//	/*modbusSlave.DealFrame();*/
	//	/*modbusSlave.rxFrame.RemoveOneFrame();*/
	//}
	//else if ((modbusSlave.com.RxSize() > 0 || (modbusSlave.rxFrame.Length > 0)) && (Sys.Ms() - modbusSlave.com.LastRcvTime) > 5)
	//{
	//	debug_printf("Clear RcvBuffer rxFrame:%d  com.RxSize:%d\n", modbusSlave.rxFrame.Length, modbusSlave.com.RxSize());
	//	/*modbusSlave.rxFrame.Length = 0;*/
	//	modbusSlave.com.ClearRxBuf();
	//}
	//else {}
}

void MqttLinkTestInit()
{
	
	Sys.AddTask(MqttLinkRoutin, 0, 0, 1, "ModbusSlaveLinkRoutin");
}
#endif
