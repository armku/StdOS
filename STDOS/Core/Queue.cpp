#include "Queue.h"

Queue::Queue()
{
#if _USEARRAY
	this->pBuf = 0;
	this->_capacity = 0;
	this->_read = 0;
	this->_write = 0;
#else
	this->pBuf = 0;
	this->_capacity = 0;
	this->_tail = 0;
	this->_head = 0;
#endif
}
void Queue::Clear()
{
#if _USEARRAY
	this->_write = 0;
	this->_read = 0;
	this->_count = 0;
#else
	this->_head = 0;
	this->_tail = 0;
	this->_size = 0;
#endif
}





#if _USEARRAY
void Queue::SetBuf(void *buf, int len)
{
    if (len >= 0)
    {
        this->pBuf = (byte*)buf;
        this->_capacity = len;
    }
    this->Clear();
}


int Queue::Enqueue(byte da)
{
    if (this->pBuf == 0)
    {
        return  - 1;
    }
    this->pBuf[this->_write] = da;
    if (++this->_write >= this->_capacity)
    {
        this->_write = 0;
    }
    if (this->_count < this->_capacity)
    {
        this->_count++;
        return 0;
    }
    else
    {
        return  - 1;
    }
}

byte Queue::Dequeue()
{
    byte ret = this->pBuf[this->_read];
    if (++this->_read >= this->_capacity)
    {
        this->_read = 0;
    }
    if (this->_count)
    {
        this->_count--;
        //return 0;
    }
    else
    {
        //return  - 1;
    }
    return ret;
}

// 批量写入
int Queue::Write(const Buffer &bs)
{
    for (int i = 0; i < bs.Length(); i++)
    {
        this->Enqueue(bs[i]);
    }
    return bs.Length();
}

// 批量读取
int Queue::Read(Buffer &bs)
{
    int len = this->Length();
    for (int i = 0; i < len; i++)
    {
        bs[i] = this->Dequeue();
    }
    bs.SetLength(len);
    return len;
}
#else

void Queue::SetBuf(void *buf, int len)
{
	if (len >= 0)
	{
		this->pBuf = (byte*)buf;
		this->_capacity = len;
	}
	this->Clear();
}

void Queue::Enqueue(byte da)
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

byte Queue::Dequeue()
{
	byte ret = this->pBuf[this->_tail];
	if (++this->_tail >= this->_capacity)
	{
		this->_tail = 0;
	}
	if (this->_size)
	{
		this->_size--;
		//return 0;
	}
	else
	{
		//return  - 1;
	}
	return ret;
}

// 批量写入
int Queue::Write(const Buffer &bs)
{
	for (int i = 0; i < bs.Length(); i++)
	{
		this->Enqueue(bs[i]);
	}
	return bs.Length();
}

// 批量读取
int Queue::Read(Buffer &bs)
{
	int len = this->Length();
	for (int i = 0; i < len; i++)
	{
		bs[i] = this->Dequeue();
	}
	bs.SetLength(len);
	return len;
}
#endif

