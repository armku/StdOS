#include "Queue.h"

Queue::Queue()
{
	this->pBuf = 0;
	this->_capacity = 0;
	this->_tail = 0;
	this->_head = 0;
}
void Queue::Clear()
{
	this->_head = 0;
	this->_tail = 0;
	this->_size = 0;
}
void Queue::Enqueue(uint8_t da)
{
	if (this->pBuf == 0)
	{
		return;
	}
	this->pBuf[this->_head] = da;
	if (++this->_head >= this->_capacity)
	{
		this->_head = 0;
	}
	if (this->_size < this->_capacity)
	{
		this->_size++;
		return;
	}
	else
	{
		return;
	}
}

uint8_t Queue::Dequeue()
{
	uint8_t ret = this->pBuf[this->_tail];
	if (++this->_tail >= this->_capacity)
	{
		this->_tail = 0;
	}
	if (this->_size)
	{
		this->_size--;
	}
	else
	{
		
	}
	return ret;
}

// 批量写入
int Queue::Write(const Buffer &bs)
{
	return this->Write((char*)bs.GetBuffer(), bs.Length());
}
//写入数据 返回写入数量(字节数)
int Queue::Write(void* buf, int len)
{
	if (len >= this->RemainLength())
		len = this->RemainLength();
	for (int i = 0; i < len; i++)
		this->Enqueue(((char*)buf)[i]);
	return len;
}

// 批量读取
int Queue::Read(Buffer &bs)
{
	int len = this->Read(bs.GetBuffer(), bs.Length());
	bs.SetLength(len);
	return len;
}
// 读取数据，返回读取数量(字节数) 
int Queue::Read(void* buf, int len)
{
	if (len > this->RemainLength())
		len = this->RemainLength();
	for (int i = 0; i < len; i++)
		((char*)buf)[i] = this->Dequeue();

	return len;
}
void Queue::SetBuf(void *buf, int len)
{
	if (len >= 0)
	{
		this->pBuf = (uint8_t*)buf;
		this->_capacity = len;
	}
	this->Clear();
}
