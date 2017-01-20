#include <stdio.h>
#include "Buffer.h"
#include "Sys.h"

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
Buffer & Buffer::operator = (byte* bufsrc)
{
	for(int  i=0;i<this->bufLength;i++)
	{
		this->pbuf[i]=bufsrc[i];
	}
	return *this;
}
Buffer &Buffer::operator = (Buffer bufsrc)
{
	if(this->bufLength<bufsrc.bufLength)
	{
		debug_printf("Error Buffer copy: Buffer length mismath src: %d ,dst: %d \n",bufsrc.bufLength,this->bufLength);
	}
	else
	{
		for(int i=0;i<bufsrc.Length();i++)
		{
			this->pbuf[i]=bufsrc.GetBuffer()[i];
		}
	}
	return *this;
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
//设置长度，可自动扩容  
void Buffer::SetLength(ushort len)
{
	if(this->bufLength>=len)
	{
		this->bufLength=len;
	}
	else
	{
		//自动扩容
		this->pbuf=new byte[len];
		this->bufLength=len;
	}
}
void Buffer::Show(bool newLine) const
{
	if(newLine)
	{
		for(int i=0;i<this->bufLength-1;i++)
		{
			printf("%02X ",this->pbuf[i]);
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
