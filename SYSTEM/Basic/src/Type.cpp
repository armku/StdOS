#include "stdio.h"
#include "Type.h"

void assert_failed(uint8_t* file, uint32_t line,char * errstr)
{
	printf("%s(%d):    %s\n", file, line,errstr);
}
