#include "Queue.h"

Queue::Queue()
{
    this->pBuf = 0;
    this->_capacity = 0;
    this->_read = 0;
    this->_write = 0;
}

void Queue::SetBuf(void *buf, int len)
{
    if (len >= 0)
    {
        this->pBuf = (byte*)buf;
        this->_capacity = len;
    }
    this->Clear();
}

void Queue::Clear()
{
    this->_write = 0;
    this->_read = 0;
    this->_count = 0;
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
