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
	#if 1
    printf(this->ToString().GetBuffer());

    if (newLine)
    {
        printf("\r\n");
    }
	#else	
	char v6[512]; 
	String v5(v6,512);  
	v5=*this;	
	v5.SetLength(0);  
	v5.Show(newLine);
	#endif
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
const Type Object::GetType()const
{
    Type ret;
    ret._info = "Object";
    ret.Size = 7;
    return ret;
}

Type::Type()
{

}
// Ãû³Æ
const String Type::Name()const
{
    String ret((cstring)(this->_info), this->Size);
    return ret;
}
