/*
String str;
	str.Show(true);
	printf("str length:%d capacity:%d\n",str.Length(),str.Capacity());
	
	String str1("456");
	str1.Show();
	printf("Getbuffer:%s length:%d\n",str1.GetBuffer(),str1.Length());
	
	String str2(str1);
	str2.Show();
	printf("str1==str2:%d \n",str1==str2);
	
	char cs[]="Hello Buffer";
	String str4(cs,sizeof(cs));
	str4.Show();
	printf("str4 == cs :%d \n",str4==cs);
	
	String str5((char)'1');
	str5.Show();
	printf("str5 == 1 %d \n",str5=="1");
*/

#include "CString.h"
#include <stdio.h>
#include <string.h>

String::String()
{
	this->initCapacity();
	this->jsLength();
}
String::String(const char * str,int length)
{
	if(length<0)
	{
		this->bufLength=strlen(str);		
	}
	else
	{
		this->bufLength=length;
	}
	this->initCapacity();
	
	this->Copy(0,str,this->bufLength);	
}
String::String(char ch)
{
	this->initCapacity();	
	this->pbuf[0]=ch;
	this->jsLength();
}
//进制字符串
String::String(byte value,byte radix)
{
	this->initCapacity();
	
	switch(radix)
	{
		case 10:
			snprintf((char*)this->pbuf,this->mcapacity,"%d",value);
			break;
		case 16:
			snprintf((char*)this->pbuf,this->mcapacity,"%x",value);
			break;
	}
	this->jsLength();
}
//进制字符串
String::String(short value,byte radix)
{
	this->initCapacity();
	
	switch(radix)
	{
		case 10:
			snprintf((char*)this->pbuf,this->mcapacity,"%d",value);
			break;
		case 16:
			snprintf((char*)this->pbuf,this->mcapacity,"%x",value);
			break;
	}
	this->jsLength();	
}
//进制字符串
String::String(int value,byte radix)
{
	this->initCapacity();
	
	switch(radix)
	{
		case 10:
			snprintf((char*)this->pbuf,this->mcapacity,"%d",value);
			break;
		case 16:
			snprintf((char*)this->pbuf,this->mcapacity,"%x",value);
			break;
	}
	this->jsLength();	
}
//进制字符串
String::String(uint value,byte radix)
{
	this->initCapacity();
	
	switch(radix)
	{
		case 10:
			snprintf((char*)this->pbuf,this->mcapacity,"%u",value);
			break;
		case 16:
			snprintf((char*)this->pbuf,this->mcapacity,"%x",value);
			break;
	}
	this->jsLength();	
}
//进制字符串
String::String(Int64 value,byte radix)
{
	this->initCapacity();
	
	switch(radix)
	{
		case 10:
			snprintf((char*)this->pbuf,this->mcapacity,"%I64d",value);
			break;
		case 16:
			snprintf((char*)this->pbuf,this->mcapacity,"%I64d",value);
			break;
	}
	this->jsLength();	
}
//进制字符串
String::String(UInt64 value,byte radix)
{
	this->initCapacity();
	
	switch(radix)
	{
		case 10:
			snprintf((char*)this->pbuf,this->mcapacity,"%I64u",value);
			break;
		case 16:
			snprintf((char*)this->pbuf,this->mcapacity,"%I64u",value);
			break;
	}
	this->jsLength();	
}
//浮点数
String::String(float value,byte dot)
{
	this->initCapacity();
	switch(dot)
	{
		case 0:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.0f",value);
			break;
		case 1:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.1f",value);
			break;
		case 2:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.2f",value);
			break;
		case 3:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.3f",value);
			break;
		case 4:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.4f",value);
			break;
		case 5:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.5f",value);
			break;
		default:
			break;
	}	
	this->jsLength();	
}
//浮点数
String::String(double value,byte dot)
{
	this->initCapacity();
	switch(dot)
	{
		case 0:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.0lf",value);
			break;
		case 1:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.1lf",value);
			break;
		case 2:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.2lf",value);
			break;
		case 3:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.3lf",value);
			break;
		case 4:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.4lf",value);
			break;
		case 5:
			snprintf((char*)this->pbuf,this->mcapacity,"%0.5lf",value);
			break;
		default:
			break;
	}	
	this->jsLength();	
}
String::String(const String& str1)
{
	#if 1
	this->initCapacity();
	#endif
	#if 1
	for(int i=0;i<str1.Length();i++)
	{
		this->pbuf[i]=str1.GetBuffer()[i];
	}
	this->jsLength();
	#endif
}
bool String::operator ==(const char* str) 
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
	return this->mcapacity;
}
void String::Show(bool newLine) const
{
	printf("%s",this->pbuf);
	newLine ? printf("\n"):0;
}
//初始化容器
void String::initCapacity()
{
	this->pbuf=new byte[0x40];
	this->mcapacity=0x40-1;
	memset(this->pbuf,0x40,0);
}
//计算字符串长度
void String::jsLength()
{
	this->bufLength= strlen((char*)this->pbuf);
}
