#include "BlinkPort.h"
#include "Sys.h"

BlinkPort::BlinkPort()
{
	this->Count 	= 0;
	this->First		= 1;
	this->Times 	= 10;
	this->Interval1 = 100;
	this->Interval2 = 300;
	this->Current 	= 0;
	this->Index		= 0;
}
BlinkPort::~BlinkPort()
{
	Sys.RemoveTask(this->_tid);
}

void BlinkPort::Add(OutputPort* port)
{
	this->Ports[this->Count++] = port;
}
void BlinkPort::Start()
{
	if(this->Count)
	{
		this->Current = 0;
		for(int i=0;i<this->Count;i++)
		{
			this->Ports[i]->Open();
		}
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
		this->Ports[i]->Write(0);
	}
}
void BlinkPort::Blink()
{
	for(int i=0;i<this->Count;i++)
	{
		this->Ports[i]->Write(0);
	}
	this->Current++;
	if(this->Current < this->Times)
	{
		this->First=!this->First;
		Sys.SetTask(this->_tid,1,this->Interval2);
	}
}

