#include "Modbus.h"
#include "Master.h"
#include "Slave.h"
#include "Security\crc.h"

Modbus::Modbus()
{
	this->Address=0;
	this->Code=0;
	this->Crc=0;
	this->Crc2=0;
	this->Length=0;
}
/*
测试数组 
地址 功能码 地址Hi 地址Lo 个数Hi 个数Lo CrcLo CrcHi
01     03    00     00     00    01    84     0A
*/
bool Modbus::Read(Stream &ms)
{
	if(ms.Capacity()<5)
	{
		return false;
	}
	this->Address=ms.GetBuffer()[0];
	this->Code=ms.GetBuffer()[1];
	this->Length=ms.Capacity()-4;
	for(int i=0;i<this->Length;i++)
	{
		this->Data[i]=ms.GetBuffer()[i+2];
	}
	this->Crc=ms.GetBuffer()[ms.Capacity()-1];
	this->Crc<<=8;
	this->Crc|=ms.GetBuffer()[ms.Capacity()-2];
	
	this->Crc2=Crc::CRC16RTU(ms.GetBuffer(),ms.Capacity()-2);
	
	
	if(this->Crc==this->Crc2)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void Modbus::Write(Stream &ms)
{	
	ms.GetBuffer()[0]=this->Address;
	ms.GetBuffer()[1]=this->Code;
	this->Crc2=Crc::CRC16RTU(this->Data,this->Length);
	ms.GetBuffer()[this->Length+2]=this->Crc2&0xff;
	ms.GetBuffer()[this->Length+3]=(this->Crc2>>8)&0xff;
	for(int i=0;i<this->Length;i++)
	{
		ms.GetBuffer()[2+i]=this->Data[i];
	}	
}

void Modbus::SetError(ModbusErrors::Errors error){}
