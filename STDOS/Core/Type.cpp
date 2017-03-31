#include "stdio.h"
#include "Type.h"
#include "SString.h"

void assert_failed(byte *file, uint line, char *errstr)
{
    printf("%s(%d):    %s\n", file, line, errstr);
}
String& Object::ToStr(String& str) const
{
	return str;
}
String Object::ToString() const
{
	return String("");
}

void Object::Show(bool newLine)const
{
    if (newLine)
    {
        printf("\r\n");
    }
}

#if 0
    const CType Object::GetType()const{}
#endif 
#if 0
    //输出对象的字符串表示方式.支持RVO优化
    String Object::ToString()const
    {
        return String("Object");
    }
#endif
