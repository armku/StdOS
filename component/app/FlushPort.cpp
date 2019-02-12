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
	sys.RemoveTask(this->_tid);
}

void FlushPort::Start(int ms)
{
	if(this->Port)
	{
		if(this->Port->Open())
		{
			if(this->_tid ==0)
			{
				int taskperiod	= this->Slow;
				if(this->Slow <= 0)
					taskperiod = this->Fast;
				this->_tid	= sys.AddTask((void (FlushPort::*)())&FlushPort::Flush,(FlushPort *)this,taskperiod,taskperiod,"闪烁端口");
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
			sys.SetTaskPeriod(this->_tid,this->Fast);
			sys.SetTask(this->_tid,this->Count>0,0);
		}
	}
}
// mode 0xff  为不改变Port状态  0为Write false  1为Write true
void FlushPort::Stop(uint8_t mode)
{
	if(this->Slow)
		sys.SetTaskPeriod(this->_tid,this->Slow);
	else
		sys.SetTask(this->_tid,0,0);
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
			sys.SetTaskPeriod(this->_tid,this->Fast);
		else
			sys.SetTask(this->_tid,0,0);
		
	}
}
int FlushPort::Write(uint8_t* data)
{
	this->Start(data[0]);
	return 1;
}
