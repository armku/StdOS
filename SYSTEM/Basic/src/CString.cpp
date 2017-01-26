#include "CString.h"

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
//Êý×éÈÝÁ¿
uint String::Capacity() const
{
	return this->bufLength;
}

