#include "RingBuffer.h"

void RingBuffer::Init(void* buffer, int bufsize)
{
	this->buf = (uint8_t*)buffer;
	this->length = bufsize;
	this->head = 0;
	this->tail = 0;
	this->fillcnt = 0;
}
int RingBuffer::Put(char data)
{
	if (this->fillcnt >= this->length)
	{
		return;
	}
	else {}
	this->buf[this->head] = data;
	this->head++;
	this->fillcnt++;
	this->head %= this->length;
	return true;
}
int RingBuffer::Put(char *c, int len)
{
	for (int i = 0; i < len; i++)
		this->Put(c[i]);
}
int RingBuffer::Get(char *c, int len)
{
	if (this->fillcnt <= 0)
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
		this->fillcnt--;
		*c = this->buf[this->tail++];
		c++;
		this->tail %= this->length;
	}
	return true;
}
