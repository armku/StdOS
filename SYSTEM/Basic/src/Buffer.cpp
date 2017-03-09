#include <stdio.h>
#include "Buffer.h"
#include "Sys.h"

Buffer::Buffer(void* ptr,int len)
{
	this->_Arr=ptr;
	this->_Length=len;
}

Buffer & Buffer::operator = (const void * prt)
{
	for(int  i=0;i<this->bufLength;i++)
	{
		this->pbuf[i]=((byte*)prt)[i];
	}
	return *this;
}
Buffer& Buffer::operator=(Buffer&& rval)
{
	if(this->bufLength<rval.bufLength)
	{
		debug_printf("Error: Buffer copy: Buffer length mismath src: %d ,dst: %d \n",rval.bufLength,this->bufLength);
	}
	else
	{
		for(int i=0;i<rval.Length();i++)
		{
			this->pbuf[i]=rval.GetBuffer()[i];
		}
	}
	return *this;
}
byte &Buffer::operator [] (int pos)
{
	if((pos<0)||(pos>this->bufLength))
	{
		debug_printf("Error: [] length error");
		return this->pbuf[0];
	}
	return this->pbuf[pos];
}

//���ó��ȣ����Զ����� 
bool Buffer::SetLength(int len)
{
	if(this->bufLength>=len)
	{
		this->bufLength=len;
	}
	else
	{
		//�Զ�����
		this->pbuf=new byte[len];
		this->bufLength=len;
		delete []this->pbuf;
	}
	return true;
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
	//Object::Show(newLine);
	Object::Show(true);
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
		len=this->bufLength;
		if(len<src.bufLength)
		{
			this->SetLength(src.bufLength);
			len=this->bufLength;
		}
	}
	for(int i=0;i<len;i++)
	{		
		this->pbuf[destIndex+i] = src.pbuf[srcIndex+i];
	}	
}
//��ȡ�Ի�����  
Buffer Buffer::Sub(int index,int length)
{	
	byte *pbufsub=new byte[length];
	if(length > this->bufLength)
	{
		length= this->bufLength;
	}
	Buffer buf(pbufsub,length);
	
	for(int i=index;i<(index+length);i++)
	{
		pbufsub[i]=this->GetBuffer()[index+i];
	}	
	return buf;
}
