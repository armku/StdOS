#include "MqttLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"

#define _MQTTLINKEST_CPP
#ifdef _MQTTLINKEST_CPP

USART usart333(USART3, 115200);
MqttLink modbusSlave(usart333);
char* id = "123456789";

void MqttLinkRoutin(void* param)
{	
	modbusSlave.Connect();
	
}

void MqttLinkTestInit()
{
	modbusSlave.FixHead = 0X10;
	modbusSlave.ClientID = id;
	Sys.AddTask(MqttLinkRoutin, 0, 0, 1000, "ModbusSlaveLinkRoutin");
}
#endif
