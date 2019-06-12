#include <string.h>
#include "Buffer.h"

static void ShowChar(char c)
{
	if (c >= 10)
	{
		StdPrintf("%c", c - 10 + 'A');
	}
	else
	{
		StdPrintf("%c", c + '0');
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
		StdPrintf("%c",this->_Arr[i]);
	}	
	if(newLine)
	{
		StdPrintf("\n");
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
			StdPrintf("%c",sep);
		}
	}
	
	if(newLine)
	{
		StdPrintf("\n");
	}
}
