#include <stdio.h>
#include "../src/MqttLink.h"
#include "Sys.h"
#include "../../../../Bsp/Porting.h"
#include "Task.h"

//#define _MQTTLINKEST_CPP
#ifdef _MQTTLINKEST_CPP

HardwareSerial0_T usart333(USART3, 115200);
MqttLink mqttSlave(usart333);
char* id = "123456789";
char clientids[24];//
char* topic = "G/ddd";

void MqttLinkRoutin(void* param)
{	
	uint8_t bufsend[4];
	bufsend[0] = 0X61;
	bufsend[1] = 0X62;
	bufsend[2] = 0X63;
	bufsend[3] = 0X64;
	switch (mqttSlave.step)
	{
	case 0:
		if (mqttSlave.Connect())
		{
			mqttSlave.step++;
		}
		break;
	case 1:
		mqttSlave.Puslish(bufsend,ArrayLength(bufsend));
		break;
	default:
		break;
	}	
}

void MqttLinkTestInit()
{
	mqttSlave.FixHead = 0X10;
	mqttSlave.ClientID = id;
	for (int i = 0; i < ArrayLength(clientids); i++)
	{
		clientids[i] = 0;
	}
	
	for (int i = 0; i < 12; i++)
	{
		//sprintf(&clientids[i * 2], "%02X", sys.ID[i+1]);
		clientids[i] = sys.ID[i];
	}
	
	mqttSlave.ClientID = clientids;
	
	mqttSlave.Topic = topic;
	sys.AddTask(MqttLinkRoutin, 0, 0, 1000, "ModbusSlaveLinkRoutin");
}
#endif
