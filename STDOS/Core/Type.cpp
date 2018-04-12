#include "Type.h"
#include "SString.h"

void assert_failed(uint8_t *file, uint32_t line, char *errstr)
{
    StdPrintf("%s(%d):    %s\n", file, line, errstr);
}


String Object::ToString()const
{
    return String("Object");
}

void Object::Show(bool newLine)const
{
//	char bufch[512];
//	String strtmp(bufch,ArrayLength(bufch));
//	strtmp.SetLength(0);
//	this->ToStr(strtmp);	
//	strtmp.Show(newLine);
}
