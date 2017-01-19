#include <stdio.h>
#include "Buffer.h"

Buffer::Buffer(char* buf,ushort length)
{
	this->pbuf=(byte*)buf;
	this->bufLength=length;
}

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
	if(newLine)
	{
		for(int i=0;i<this->bufLength-1;i++)
		{
			printf("%02X-",this->pbuf[i]);
		}
		printf("%02X",this->pbuf[this->bufLength-1]);
	}
	else
	{
		printf("%s",this->pbuf);
	}
	Object::Show(newLine);
}
//把数据复制到目标缓冲区，默认-1长度表示当前长度
void Buffer::CopyTo(int destIndex, const void *dest, int len)
{
	if(len==-1)
	{
		len=this->bufLength;
	}
	if(len<=0)
	{
		return;
	}
	for(int i=0;i<len;i++)
	{
		((byte*)dest)[destIndex+i]=this->pbuf[i];
	}
}
//拷贝数据，默认-1长度表示当前长度 
void Buffer::Copy(int destIndex, const void *src, int len)
{
	if(len==-1)
	{
		len=this->bufLength;
	}
	if(len<=0)
	{
		return;
	}
	for(int i=0;i<len;i++)
	{
	this->pbuf[destIndex+i]=((byte*)src)[i];	
	}	
}
//拷贝数据，默认-1长度表示两者最小长度
void Buffer::Copy(int destIndex, const Buffer &src, int srcIndex, int len)
{
	if(len==-1)
	{		
		//len=(*src).Length()>this->Length?src.Length():this->Length;
	}
	if(len<=0)
	{
		return;
	}
	for(int i=0;i<len;i++)
	{
		//this->pbuf[destIndex+i]=src.GetBuffer()[srcIndex+i];
	}	
}
