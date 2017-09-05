#include "Queue.h"

void ExitCritical()
{
  ;
}
void EnterCritical()
{
  ;
}

byte buf[64];
Queue::Queue():_s(buf,64)
{		
	this->Clear();
}


void Queue::SetCapacity(int len)
{
    this->_s.SetLength(len);
}

void Queue::Clear()
{
	int v2=this->_s.Capacity();
	this->_s.SetLength(v2);
	this->_head = 0;
	this->_tail = 0;
	this->_size = 0;
}

void Queue::Enqueue(byte dat)
{
	int capacity=this->Capacity();
	if(capacity==0)
	{
		this->_s.SetLength(64);
	}
	int ret=this->_size;
	
	if(ret<capacity)
	{
		int headold=this->_head++;
		this->_s[headold]=dat;
		if(this->_head>=capacity)
			this->_head-=capacity;
		EnterCritical();
		this->_size--;
		ExitCritical();
	}
	return;	
}

byte Queue::Dequeue()
{
	byte ret;
	
	if(this->_size)
	{
		EnterCritical();
		--this->_size;
		ExitCritical();
		int capacity=this->Capacity();
		int tailold=this->_tail++;
		ret=this->_s[tailold];
		if(this->_tail>=capacity)
		{
			this->_tail-=capacity;
		}
	}
	else
	{
		ret=0;
	}
    return ret;
}

// 批量写入
int Queue::Write(const Buffer &bs)
{
    return 0;
}

// 批量读取
int Queue::Read(Buffer &bs)

{
    return 0;
}
