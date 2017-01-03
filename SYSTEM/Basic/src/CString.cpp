#include "CString.h"

//ÁÙÊ±²Ù×÷
String::String(char* buf):Array((byte*)buf,0)
{
}
String::String(byte* buf,ushort length):Array(buf,length)
{
	
}

