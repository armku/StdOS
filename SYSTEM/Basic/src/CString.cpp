#include "CString.h"
#include <stdio.h>

String::String()
{
	byte *buf=new byte[1];
	buf[0]=0;
	this->pbuf=buf;
	this->bufLength=1;
}
String::String(char * str)
{
	this->bufLength=sizeof(str);
	this->pbuf=new byte[this->bufLength];
	this->Copy(0,str,this->bufLength);
}
String::String(char ch)
{
	this->bufLength=2;
	this->pbuf=new byte[this->bufLength];
	this->pbuf[0]=ch;
	this->pbuf[1]=0;
}
//进制字符串
String::String(byte value,byte radix)
{
	switch(radix)
	{
		case 10:
			if(value<10)
			{
				this->bufLength=1;
			}
			else if(value<100)
			{
				this->bufLength=2;
			}
			else
			{
				this->bufLength=3;
			}
			this->pbuf=new byte[this->bufLength+1];
			snprintf((char*)this->pbuf,2,"%d",value);
			break;
		case 16:
			if(value<0x10)
			{
				this->bufLength=1;
			}
			else
			{
				this->bufLength=2;
			}
			this->pbuf=new byte[this->bufLength+1];
			snprintf((char*)this->pbuf,2,"%X",value);
			break;
		default:
			break;
	}
}
//进制字符串
String::String(short value,byte radix)
{
	switch(radix)
	{
		case 10:
			
			break;
		case 16:
			break;
	}
}
String::String(byte* buf,ushort length):Array(buf,length)
{
	
}
bool String::operator ==(char* str)
{
	for(int i=0;i<this->bufLength;i++)
	{
		if(this->pbuf[i]!=str[i])
		{
			return false;
		}
	}
	return true;
}
bool String::operator==(const String& str1)
{
	if(this->Length()!=str1.bufLength)
	{
		return false;
	}
	for(int i=0;i<this->bufLength;i++)
	{
		if(this->pbuf[i]!=str1.pbuf[i])
		{
			return false;
		}
	}
	return true;
}
//数组容量
uint String::Capacity() const
{
	return this->bufLength;
}

