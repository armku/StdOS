#include "stdio.h"
#include "Object.h"

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
        printf("\n");
    }
}

#if 0
    const CType Object::GetType()const{}
#endif
