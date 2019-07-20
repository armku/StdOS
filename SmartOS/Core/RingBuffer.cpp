#include "RingBuffer.h"
//暂时未用

RingBuffer::RingBuffer(void* buffer, int bufsize)
{
	this->Init(buffer,bufsize);
}

void RingBuffer::Init(void* buffer, int bufsize)
{
	this->buf = (uint8_t*)buffer;
	this->BufLen = bufsize;
	this->head = 0;
	this->tail = 0;
	this->length= 0;
}
int RingBuffer::Put(char data)
{
	if (this->length >= this->BufLen)
	{
		return 0;
	}
	else {}
	this->buf[this->head] = data;
	this->head++;
	this->length++;
	this->head %= this->BufLen;
	return 1;
}
int RingBuffer::Put(char *c, int len)
{
	for (int i = 0; i < len; i++)
		this->Put(c[i]);
	return len;
}
int RingBuffer::Get(char *c, int len)
{
	if (this->length <= 0)
	{
		return false;
	}
	else {}
	if (len > this->length)
	{
		len = this->length;
	}
	for (int i = 0; i < len; i++)
	{
		this->length--;
		*c = this->buf[this->tail++];
		c++;
		this->tail %= this->BufLen;
	}
	return len;
}
bool RingBuffer::Empty()
{
	return this->length == 0;
}
