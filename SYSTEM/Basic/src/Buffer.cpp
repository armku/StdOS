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
//�����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
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
//�������ݣ�Ĭ��-1���ȱ�ʾ��ǰ���� 
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
//�������ݣ�Ĭ��-1���ȱ�ʾ������С����
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
