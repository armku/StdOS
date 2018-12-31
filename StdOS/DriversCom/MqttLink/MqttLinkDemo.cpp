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
	uint8_t bufsend[10];
	switch (modbusSlave.step)
	{
	case 0:
		if (modbusSlave.Connect())
		{
			modbusSlave.step++;
		}
		break;
	case 1:
		modbusSlave.Send(bufsend,ArrayLength(bufsend));
		break;
	default:
		break;
	}	
}

void MqttLinkTestInit()
{
	modbusSlave.FixHead = 0X10;
	modbusSlave.ClientID = id;
	Sys.AddTask(MqttLinkRoutin, 0, 0, 1000, "ModbusSlaveLinkRoutin");
}
#endif
