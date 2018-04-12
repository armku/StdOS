#include "Type.h"
#include "SString.h"

void assert_failed(uint8_t *file, uint32_t line, char *errstr)
{
    StdPrintf("%s(%d):    %s\n", file, line, errstr);
}

//String &Object::ToStr(String &str)const
//{
//    return str;
//}

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

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//const Type Object::GetType()const
//{
//    Type ret;
//    ret._info = "Object";
//    ret.Size = 7;
//    return ret;
//}

//Type::Type()
//{

//}
//// Ãû³Æ
//const String Type::Name()const
//{
//    String ret((cstring)(this->_info), this->Size);
//    return ret;
//}
