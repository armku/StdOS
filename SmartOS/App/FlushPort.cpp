#include "FlushPort.h"
#include "Sys.h"


FlushPort::FlushPort()
{
	this->_tid	= 0;
	this->Port	= 0;
	this->Fast	= 50;
	this->Slow	= 1000;
	this->Count	= 0;
}
FlushPort::~FlushPort()
{
	Sys.RemoveTask(this->_tid);
}

void FlushPort::Start(int ms)
{
	if(this->Port)
	{		
			if(this->_tid ==0)
			{
				int taskperiod	= this->Slow;
				if(this->Slow <= 0)
					taskperiod = this->Fast;
				this->_tid	= Sys.AddTask((void (FlushPort::*)())&FlushPort::Flush,(FlushPort *)this,taskperiod,taskperiod,"��˸�˿�");
			}
			if(ms <=0)
			{
				this->Count = -1;
			}
			else
			{
				this->Count = ms / this->Fast;
				if(this->Count < 2)
					this->Count = 2;
			}
			Sys.SetTaskPeriod(this->_tid,this->Fast);
			Sys.SetTask(this->_tid,this->Count>0,0);		
	}
}
// mode 0xff  Ϊ���ı�Port״̬  0ΪWrite false  1ΪWrite true
void FlushPort::Stop(byte mode)
{
	if(this->Slow)
		Sys.SetTaskPeriod(this->_tid,this->Slow);
	else
		Sys.SetTask(this->_tid,0,0);
	if(mode !=0XFF)
		this->Port->Write(mode > 0);
	
}
void FlushPort::Flush()
{
	this->Port->Write(this->Count&0x01);
	if(this->Count > 0)
	{
		this->Count--;
		if(this->Fast)
			Sys.SetTaskPeriod(this->_tid,this->Fast);
		else
			Sys.SetTask(this->_tid,0,0);
		
	}
}
int FlushPort::Write(byte* data)
{
	this->Start(data[0]);
	return 1;
}
