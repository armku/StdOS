#include "Queue.h"

void ExitCritical()
{
  ;
}
void EnterCritical()
{
  ;
}



byte buf[512];
Queue::Queue():_s(buf,512)
{	
	return ;
}


void Queue::SetCapacity(int len)
{
    this->_s.SetLength(len);
}

void Queue::Clear()
{
	int v2=this->_s.Capacity();
	this->_s.SetLength(v2);
	//this->_s.5 6 7=0;
//	this->_s._canWrite=0;
//	this->_s._Size=0;
//	this->_s._Capacity=0;
}

void Queue::Enqueue(byte dat)
{
    return ;
}

byte Queue::Dequeue()
{
    return 0;
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
