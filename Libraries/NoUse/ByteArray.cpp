#include "ByteArray.h"
#include "stddef.h"

uint8_t bytearraybuf[10];

ByteArray::ByteArray(int length ):Array(bytearraybuf,ArrayLength(bytearraybuf))
{
	this->Array::SetLength(length);
}

ByteArray::ByteArray(uint8_t item, int length):Array(&item,length)
{
	this->Array::SetLength(length);
	this->Buffer::Set(item,0,length);
}
// 因为使用外部指针，这里初始化时没必要分配内存造成浪费
ByteArray::ByteArray(const void* data, int length, bool copy):Array(data,length)
{
	if(copy)
	{
		this->_Length=length;
		this->Buffer::Copy(0,data,length);
	}
	else
	{
		this->Array::Set(data,length);
	}
}
ByteArray::ByteArray(void* data, int length, bool copy):Array(data,length)
{
	if(copy)
	{
		this->_Length=length;
		this->Buffer::Copy(0,data,length);
	}
	else
	{
		this->Array::Set(data,length);
	}
}
ByteArray::ByteArray(const Buffer& arr):Array(arr.GetBuffer(),arr.Length())
{
	this->Array::Copy(0,this->Arr,-1);//?
}
ByteArray::ByteArray(ByteArray&& rval):Array(rval._Arr,rval._Length)
{
	this->move(rval);
}
ByteArray& ByteArray::operator = (const Buffer& rhs)
{
	this->Buffer::operator=(rhs);
	return *this;
}
ByteArray& ByteArray::operator = (const ByteArray& rhs)
{
	this->Buffer::operator=(rhs);
	return *this;
}
ByteArray& ByteArray::operator = (const void* p)
{
	this->Buffer::operator= (p);
	
	return *this;
}
ByteArray& ByteArray::operator = (ByteArray&& rval)
{
	this->move(rval);
	
	return *this;
}
// 保存到普通字节数组，首字节为长度
int ByteArray::Load(const void* data, int maxsize)
{
	if(*(uint8_t*)data>maxsize)
		maxsize=*(uint8_t*)data;
	int i;
	for(i=0;i<maxsize;i++)
	{
		this->_Arr[i]=((uint8_t*)data)[i+1];
	}
	
	return i;
}
// 从普通字节数据组加载，首字节为长度
int ByteArray::Save(void* data, int maxsize) const
{
//	if(this->_Length > 255)
//		assert_failed2("_Length <= 0XFF");
	if(this->_Length > maxsize)
		maxsize=maxsize;
	else
		maxsize= this->_Length;
	int i;
	*(uint8_t*)data = maxsize;
	for(int i=0;i<maxsize;i++)
	{
		((uint8_t*)data)[i+1]=this->_Arr[i];
	}
	return i;
}

void* ByteArray::Alloc(int len)
{
	void * ret;
	if(len>0x40)
	{
		this->_needFree=true;
		ret=new char(len);
	}
	else
	{
		this->_needFree=false;
		ret=this->Arr;
	}
	return ret;
}
void ByteArray::move(ByteArray& rval)
{
	if((this->_Arr!=rval._Arr) &&(rval._canWrite))
	{
		return this->Array::move(rval);
	}
	else
	{
		
	}
}
#include <stdio.h>
void ByteArray::Show(bool newLine) const
{
	for(int i=0;i<this->_Length-1;i++)
	{
		StdPrintf("%02X-",this->_Arr[i]);
	}
	StdPrintf("%02X",this->_Arr[this->_Length-1]);
	if(newLine)
		StdPrintf("\r\n");
}
