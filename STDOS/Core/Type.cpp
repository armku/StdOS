#include "stdio.h"
#include "Type.h"
#include "SString.h"

void assert_failed(byte *file, uint line, char *errstr)
{
    printf("%s(%d):    %s\n", file, line, errstr);
}

String &Object::ToStr(String &str)const
{
    return str;
}

String Object::ToString()const
{
    return String("Object");
}

void Object::Show(bool newLine)const
{
    printf(this->ToString().GetBuffer());

    if (newLine)
    {
        printf("\r\n");
    }
}

const Type Object::GetType()const
{
    Type ret;
    ret._info = "Object";
    ret.Size = 7;
    return ret;
}

Type::Type(){

}
// Ãû³Æ
const String Type::Name()const
{
    String ret((cstring)(this->_info), this->Size);
    return ret;
}
