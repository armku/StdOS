#include "stdio.h"
#include "Type.h"
#include "SString.h"

#if 0
int dword_EC = 0; // weak


//----- (00000002) --------------------------------------------------------
int __fastcall Type::Name(Type *this, _DWORD *a2)
{
  Type *v2; // r6@1
  _BYTE *i; // r4@1

  v2 = this;
  for ( i = (_BYTE *)std::type_info::name(*a2); (signed int)*i >= 48 && (signed int)*i <= 57; ++i )
    ;
  return String::String(v2, i);
}
// 140: using guessed type int __fastcall String::String(_DWORD, _DWORD);
// 14C: using guessed type int __fastcall std::type_info::name(_DWORD);

//----- (00000032) --------------------------------------------------------
int __fastcall Object::Show(Object *this, bool a2)
{
  Object *v2; // r4@1
  bool v3; // r5@1
  String v5; // [sp+4h] [bp-264h]@1
  char v6; // [sp+58h] [bp-210h]@1

  v2 = this;
  v3 = a2;
  String::String(&v5, &v6, 512);
  Array::SetLength((Array *)&v5, 0);
  ((void (__fastcall *)(Object *, String *))(*v2)->object.self)(v2, &v5);
  String::Show(&v5, v3);
  return Array::~Array((Array *)&v5);
}
// 144: using guessed type int __fastcall String::String(_DWORD, _DWORD, _DWORD);

//----- (00000072) --------------------------------------------------------
String *__fastcall Object::ToStr(Object *this, String *a2)
{
  String *v2; // r5@1
  int v3; // r0@1
  const char *i; // r4@1

  v2 = a2;
  v3 = _ARM_common_get_typeid((int *)this);
  for ( i = (const char *)std::type_info::name(v3); *i >= 48 && *i <= 57; ++i )
    ;
  String::Concat(v2, i);
  return v2;
}
// 14C: using guessed type int __fastcall std::type_info::name(_DWORD);

//----- (000000AA) --------------------------------------------------------
__int64 __fastcall Object::GetType(Object *this, int *a2, int a3, int a4)
{
  Object *v4; // r5@1
  int *v5; // r4@1
  int *v6; // r6@1
  __int64 result; // r0@1
  int v8; // [sp+0h] [bp-18h]@1
  int v9; // [sp+4h] [bp-14h]@1

  v8 = a3;
  v9 = a4;
  v4 = this;
  v5 = a2;
  v6 = a2;
  Type::Type((Type *)&v8);
  v8 = _ARM_common_get_typeid(v5);
  v9 = *(v6 - 1);
  result = *(_QWORD *)&v8;
  *(_QWORD *)v4 = *(_QWORD *)&v8;
  return result;
}

//----- (000000D0) --------------------------------------------------------
int __fastcall Object::ToString(Object *this, int (__fastcall ***a2)(_DWORD, _DWORD))
{
  int (__fastcall ***v2)(_DWORD, _DWORD); // r4@1
  int v3; // r0@1

  v2 = a2;
  v3 = String::String(this, &dword_EC);
  return (**v2)(v2, v3);
}
// EC: using guessed type int dword_EC;
// 140: using guessed type int __fastcall String::String(_DWORD, _DWORD);

//----- (00000114) --------------------------------------------------------
int __fastcall _ARM_common_get_typeid(int *a1)
{
  int v1; // r0@2
  int v2; // r0@4

  if ( !a1 || (v1 = *a1) == 0 )
    _cxa_bad_typeid();
  v2 = v1 - 64;
  if ( !*(_DWORD *)(v2 + 60) )
    _cxa_bad_typeid();
  return *(_DWORD *)(v2 + 60);
}

#endif
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
	ret._info="Object";
	ret.Size=7;
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
