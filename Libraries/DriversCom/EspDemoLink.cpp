#include "EspDemoLink.h"
#include "Sys.h"
#include <string.h>  
#include "OnChip/USART.h"
#include "BspPlatform/Interrupt.h"
#include "BspPlatform\BspPlatform.h"
#include "OnChip\Port.h"
#include "OnChip\Configuration.h"
#include "Buffer.h"
#include <stdio.h>  
#include <string.h>  
#include "Sys.h"

EspDemoLink::EspDemoLink(USART &uart) :com(uart)
{

}

bool EspDemoLink::CheckFrame()
{
	bool ret = false;
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
	if (!rxFrame.CheckFrame())
		return false;
//	if (rxFrame.frameLength > 3)
//	{
//		auto crc11 = Crc::CRC16RTU(rxFrame.data, rxFrame.frameLength - 2);
//	}
//#if defined DEBUG
//	/*Buffer(rxFrame.data, rxFrame.dataLength).ShowHex(true,' ');*/
//#endif
//	ret = rxFrame.CheckFrame();
//	if (ret)
//		rxFrame.Cnt++;
	return ret;
	//return com.CheckFrame(rxFrame);
}

bool EspDemoLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;
	//if (!com.SendByte(txFrame.header))  //send frame header
	//	return false;
	//if (txFrame.fnCode > MAX_FN_CODE || !com.SendByte(txFrame.fnCode))  //send function code
	//	return false;
	//txFrame.dataLength = DATA_LENGTH[txFrame.fnCode][DIRECTION_SEND];
	//if (!com.SendByte(txFrame.dataLength))  //send data length
	//	return false;
	//if (!com.SendBytes(txFrame.data, txFrame.dataLength)) //send data;
	//	return false;
	//txFrame.CreateCheckCode();
	//if (!com.SendByte(txFrame.checkSum))    //send check code
	//	return false;
	txFrame.isUpdated = false;
	return true;
}
void EspDemoLink::cmd(char *cmd)
{
	static char buf[100];
	static int len = 0;
	len = sprintf(buf, "%s\r\n", cmd);
	this->com.SendBytes(buf, len);
}
void EspDemoLink::SetPin(Pin pch, Pin prst)
{
	this->pinch.Set(PG13);
	this->pinch.Invert = false;
	this->pinch.OpenDrain = false;
	this->pinch.Open();
	this->pinch = 0;

	this->pinrst.Set(PG14);
	this->pinrst.Invert = false;
	this->pinrst.OpenDrain = false;
	this->pinrst.Open();
	this->pinrst = 0;
}
void EspDemoLink::Init()
{
	Sys.Delay(500);
	this->pinch = 1;
	this->pinrst = 1;
}
