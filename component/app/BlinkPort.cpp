#include "BlinkPort.h"
#include "../../StdOS/Kernel/Sys.h"

BlinkPort::BlinkPort()
{
	this->_tid		= 0;
	this->First		= 1;
	
	this->Count 	= 0;
	this->Times 	= 10;
	this->Interval1 = 100;
	this->Interval2 = 300;
	this->Index		= 0;
	
	this->Current 	= 0;
	
}
BlinkPort::~BlinkPort()
{
	Sys.RemoveTask(this->_tid);
}

void BlinkPort::Add(mcuGpio* port)
{
	this->Ports[this->Count++] = port;
}
void BlinkPort::Start()
{
	if(this->Count)
	{
		this->Current = this->First;
		this->Index = 0;
		for(int i=0;i<this->Count;i++)
		{
			this->Ports[i]->mode(PIN_MODE::OUTPUT_OD_PU);
		}
		if(this->_tid ==0)
			this->_tid = Sys.AddTask((void (BlinkPort::*)())&BlinkPort::Blink,(BlinkPort *)this,-1,-1,"иак╦╤к©з");
		Sys.SetTask(this->_tid,1,-1);
	}
}
void BlinkPort::Stop()
{
	Sys.SetTask(this->_tid,0,-1);
	for(int i=0;;++i)
	{
		if(this->Count <= i)
			break;
		this->Ports[i]->Write(this->First == 0);
	}
}
void BlinkPort::Blink()
{
	int IntervalNext;
	
	IntervalNext= this->Interval1;
	if(this->Index&1)
		IntervalNext=this->Interval2;
		
	for(int i=0;i<this->Count;i++)
	{
		this->Ports[i]->Write(this->Current);
	}
	this->Index++;
	if(this->Index < this->Times)
	{
		this->Current=this->Current == 0;
		Sys.SetTask(this->_tid,1,IntervalNext);
	}
}

