#include "Queue.h"

void ExitCritical()
{
  ;
}
void EnterCritical()
{
  ;
}

Queue::Queue():_s((void*)0,0)
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
		this->_size++;
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
	int ret=0;
 int capacity =this->Capacity();
  if ( !capacity )
    this->_s.SetLength(64);
  if(this->_size<capacity)
  {
	  int v6=bs.Length();
	  if(this->_tail+v6 >capacity)
	  {
		  v6=capacity-this->_size;
	  }
	  int v7 = 0;
	  while(1)
	  {
		  int v8 = this->Capacity();
		  int v9=v8-this->_head;
		  if(v6 <=v9)
			  break;
		  //this->_s.Copy();
		  v6-=v9;
		  v7+=v9;
		  this->_tail=0;
	  }
	  //this->_s.Copy();
	  int v10=v7+v6;
	  this->_head+=v6;
	  if(this->_s.Length()<=this->_head)
		  this->_head=this->_s.Length();
	  EnterCritical();
	  this->_size+=v10;
	  ExitCritical();
	  ret=v10;
  }
  else
  {
	  ret=0;
  } 
  
    return ret;
}

// 批量读取
int Queue::Read(Buffer &bs)
{
	int ret=0;
	if(this->_size!=0)
	{
		int buflen=bs.Length();
		if(buflen!=0)
		{
			if(this->_size<buflen)
			{
				buflen=this->_size;
			}
			int v6=0;
			while(1)
			{
				int v7 = this->Capacity();
				int v8=v7-this->_tail;
				if(buflen<=v8)
					break;
				//this->_s.CopyTo();
				buflen-=v8;
				v6+=v8;
				this->_tail+=buflen;
				if(this->_tail>=v7)
				this->_tail-=v7;
				//this->_s.CopyTo(,v9);
				EnterCritical();
				//this->_size-=v9;
				ExitCritical();
				//ret=v9;
			}
		}
		else
		{
			ret=0;
		}
	}
	else
	{
		ret=0;
	}
    return ret;
}
