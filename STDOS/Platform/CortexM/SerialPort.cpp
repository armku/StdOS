#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Task.h"

void SerialPort_Opening()
{
  ;
}
void SerialPort_Closeing()
{
  ;
}
void SerialPort::OnRxHandler()
{
	((Task*)(this->_task))->Set(true,20);	
}
void SerialPort::OnTxHandler()
{
	
}
bool SerialPort::OnSet()
{
	return false;
}
void SerialPort::Init()
{
	this->Index = COM_NONE;
	RS485 = NULL;
	Error = 0;

	Remap = 0;
	this->OnInit();
}

void SerialPort::OnOpen2()
{

}

void SerialPort::OnClose2()
{

}
void SerialPort::OnWrite2()
{
	while(!this->Tx.Empty())
	{
		this->SendData(this->Tx.Dequeue());
	}	
}

void SerialPort::OnHandler(ushort num, void* param)
{
	
}
