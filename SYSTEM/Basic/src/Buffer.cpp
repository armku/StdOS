#include "Buffer.h"

Buffer::Buffer(byte* buf,ushort length)
{
	this->pbuf=buf;
	this->bufLength=length;
}
//����ָ��
byte* Buffer::GetBuffer()
{
	return this->pbuf;
}
//����
ushort Buffer::Length()
{
	return this->bufLength;
}

