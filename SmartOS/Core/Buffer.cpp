#if defined(__CC_ARM)
#include <string.h>
#else
#include <cstring>
#endif

#include "_Core.h"

#include "Buffer.h"
//#include "SString.h"

//byte& Buffer::operator[](int i)
//{
//	assert(i >= 0 && i < _Length, "下标越界");

//	byte* buf = (byte*)_Arr;

//	return buf[i];
//}

static void ShowChar(char c)
{
	if (c >= 10)
	{
		//debug_printf("%c", c - 10 + 'A');
	}
	else
	{
		//debug_printf("%c", c + '0');
	}
}


Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = (char*)ptr;
    this->_Length = len;
}

void Buffer::Show(bool newLine)const
{	
	for(int i=0;i<this->_Length;i++)
	{
		//debug_printf("%c",this->_Arr[i]);
	}	
	if(newLine)
	{
		//debug_printf("\n");
	}
}
void Buffer::ShowHex(bool newLine,char sep) const
{
	for(int i=0;i<this->_Length;i++)
	{
		ShowChar(this->_Arr[i]>>4);
		ShowChar(this->_Arr[i]&0x0F);
		if(i!=this->_Length-1)
		{
			//debug_printf("%c",sep);
		}
	}
	
	if(newLine)
	{
		//debug_printf("\n");
	}
}
