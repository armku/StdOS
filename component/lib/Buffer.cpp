#include <string.h>
#include "Buffer.h"
#include "Sys.h"

Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = (char*)ptr;
    this->_Length = len;
}

static void ShowChar(char c)
{
	if(c>=10)
	{
		printf("%c",c-10+'A');
	}
	else
	{
		printf("%c",c+'0');
	}
}
void Buffer::Show(bool newLine)const
{	
	for(int i=0;i<this->_Length;i++)
	{
		printf("%c",this->_Arr[i]);
	}	
	if(newLine)
	{
		printf("\n");
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
			printf("%c",sep);
		}
	}
	
	if(newLine)
	{
		printf("\n");
	}
}
