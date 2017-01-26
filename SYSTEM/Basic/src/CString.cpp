#include "CString.h"
#if 1
String::String()
{
	byte *buf=new byte[1];
	buf[0]=0;
	this->pbuf=buf;
	this->bufLength=1;
}
#endif
String::String(byte* buf,ushort length):Array(buf,length)
{
	
}

