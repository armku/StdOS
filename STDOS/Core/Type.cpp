#include "stdio.h"
#include "Type.h"

void assert_failed(byte* file, uint line,char * errstr)
{
	printf("%s(%d):    %s\n", file, line,errstr);
}

#if 0
    //���������ַ�����ʾ��ʽ
    virtual String &Object::ToStr(String &str)const{}
    //���������ַ�����ʾ��ʽ.֧��RVO�Ż�
    virtual String &Object::ToString()const{}
#endif 
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
//���������ַ�����ʾ��ʽ.֧��RVO�Ż�
String Object::ToString()const
{	
	return String("Object");
}
#endif
