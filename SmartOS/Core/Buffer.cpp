#if defined(__CC_ARM)
#include <string.h>
#else
#include <cstring>
#endif

#include "_Core.h"

#include "Buffer.h"
//#include "SString.h"

static void ShowChar(char c)
{
	if (c >= 10)
	{
		//SmartOS_printf("%c", c - 10 + 'A');
	}
	else
	{
		//SmartOS_printf("%c", c + '0');
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
		//SmartOS_printf("%c",this->_Arr[i]);
	}	
	if(newLine)
	{
		//SmartOS_printf("\n");
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
			//SmartOS_printf("%c",sep);
		}
	}
	
	if(newLine)
	{
		//SmartOS_printf("\n");
	}
}
