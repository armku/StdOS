#include <stdio.h>
#include "Buffer.h"

Buffer::Buffer(byte* buf,ushort length)
{
	this->pbuf=buf;
	this->bufLength=length;
}
//返回指针
byte* Buffer::GetBuffer()
{
	return this->pbuf;
}
//长度
ushort Buffer::Length()
{
	return this->bufLength;
}
void Buffer::Show(bool newLine) const
{
	printf("%s",this->pbuf);
	Object::Show(newLine);
}
