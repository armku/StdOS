/*
进行到字符串搜索
 */

#include "SString.h"
#include <string.h>
#include <CType.h>
#include "Sys.h"

#if 0
_UNKNOWN locret_14; // weak
int dword_F88 = 25381; // weak
int dword_F8C = 2573; // weak
int dword_10A8 = 0; // weak
int dword_12D8 = 0; // weak
int dword_14DC = 0; // weak
char String::operator[](int)::dummy_writable_char = '\0'; // weak
void *off_1630 = (void *)0x10AD; // weak


//----- (00000000) --------------------------------------------------------
char *__fastcall itoa(__int64 a1, char *a2, char *a3)
{
  return ltoa((signed int)a1, (char *)HIDWORD(a1), (int)a2);
}

//----- (00000016) --------------------------------------------------------
char *__fastcall ltoa(__int64 a1, char *a2, int a3)
{
  __int64 v3; // r8@1
  char *v4; // r11@1
  char *result; // r0@2
  int v6; // r0@9
  unsigned __int64 v7; // r4@12
  signed __int64 v8; // r6@15
  char *v9; // [sp+4h] [bp-8h]@20
  char v10; // [sp+Ch] [bp+0h]@1
  __int64 anonymous0; // [sp+30h] [bp+24h]@1
  char *vars8; // [sp+38h] [bp+2Ch]@1
  int _3C; // [sp+3Ch] [bp+30h]@1

  anonymous0 = a1;
  vars8 = a2;
  _3C = a3;
  v3 = a1;
  v4 = &v10;
  if ( a2 )
  {
    if ( a3 <= 36 && a3 > 1 )
    {
      v6 = a3 == 10 && a1 < 0;
      if ( v6 )
      {
        LODWORD(v7) = -(signed int)v3;
        HIDWORD(v7) = -(HIDWORD(v3) + ((unsigned int)v3 > 0));
      }
      else
      {
        v7 = v3;
      }
      while ( v7 || v4 == &v10 )
      {
        v8 = v7 % a3;
        v7 /= (unsigned __int64)a3;
        if ( v8 >= 10 )
          *v4++ = v8 + 87;
        else
          *v4++ = v8 + 48;
      }
      v9 = vars8;
      if ( v6 )
      {
        *vars8 = 45;
        ++v9;
      }
      while ( v4 > &v10 )
        *v9++ = *--v4;
      *v9 = 0;
      result = vars8;
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00000108) --------------------------------------------------------
int __fastcall trim(int result, int *a2, int a3, int a4)
{
  int v4; // r6@1
  int *v5; // r7@1
  int v6; // r9@1
  _BYTE *v7; // r4@4
  _BYTE *v8; // r5@8

  v4 = result;
  v5 = a2;
  v6 = a4;
  if ( result && *a2 )
  {
    v7 = (_BYTE *)result;
    if ( a3 )
    {
      while ( isspace(*v7) )
        ++v7;
    }
    v8 = (_BYTE *)(*v5 + v4 - 1);
    if ( v6 )
    {
      while ( isspace(*v8) && v8 >= v7 )
        --v8;
    }
    *v5 = v8 + 1 - v7;
    result = (int)v7;
  }
  return result;
}

//----- (0000015C) --------------------------------------------------------
__int64 __fastcall utoa(unsigned __int64 a1, char *a2, char *a3)
{
  return (unsigned int)ultoa(a1, (char *)HIDWORD(a1), (int)a2);
}

//----- (00000172) --------------------------------------------------------
// local variable allocation has failed, the output may be wrong!
char *__fastcall ultoa(unsigned __int64 a1, char *a2, int a3)
{
  char *result; // r0@2
  char *v4; // r9@7
  unsigned int v5; // r5@7 OVERLAPPED
  unsigned int v6; // r6@7 OVERLAPPED
  char v7; // r0@8
  unsigned int v8; // r7@11
  unsigned int v9; // r8@11
  unsigned __int64 v10; // kr08_8@11
  char *v11; // r11@16
  char v12; // [sp+4h] [bp-5Ch]@7
  unsigned __int64 v13; // [sp+28h] [bp-38h]@1
  char *v14; // [sp+30h] [bp-30h]@1
  int v15; // [sp+34h] [bp-2Ch]@1

  v13 = a1;
  v14 = a2;
  v15 = a3;
  if ( a2 )
  {
    if ( a3 <= 36 && a3 > 1 )
    {
      v4 = &v12;
      *(_QWORD *)&v5 = v13;
      if ( a3 >= 0 )
        v7 = 97;
      else
        v7 = 65;
      while ( v5 | v6 || v4 == &v12 )
      {
        v9 = __PAIR__(v6, v5) % a3 >> 32;
        v8 = __PAIR__(v6, v5) % a3;
        v10 = __PAIR__(v6, v5) / a3;
        v6 = __PAIR__(v6, v5) / a3 >> 32;
        v5 = v10;
        if ( __PAIR__(v9, v8) >= 0xA )
          *v4++ = v8 + v7 - 10;
        else
          *v4++ = v8 + 48;
      }
      v11 = a2;
      while ( v4 > &v12 )
        *v11++ = *--v4;
      *v11 = 0;
      result = a2;
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}
// 172: variables would overlap: r5.4 and r5.8
// 172: variables would overlap: r6.4 and r5.8

//----- (00000226) --------------------------------------------------------
char *__fastcall utohex(unsigned int a1, int a2, char *a3, int a4)
{
  unsigned int v4; // r5@1
  char *result; // r0@2
  int v6; // r4@4
  char *v7; // r1@4
  char v8; // r0@5
  char v9; // r7@7
  int i; // r6@7
  int v11; // r0@8

  v4 = a1;
  if ( a3 )
  {
    v6 = 2 * a2 & 0xFF;
    a3[v6] = 0;
    v7 = &a3[v6 - 1];
    if ( a4 )
      v8 = 65;
    else
      v8 = 97;
    v9 = v8;
    for ( i = 0; i < v6; ++i )
    {
      v11 = v4 & 0xF;
      v4 >>= 4;
      if ( v11 >= 10 )
        *v7-- = v11 + v9 - 10;
      else
        *v7-- = v11 + 48;
    }
    result = a3;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00000278) --------------------------------------------------------
char *__fastcall dtostrf(double a1, unsigned __int8 a2, char *a3, int a4)
{
  double v4; // kr00_8@1
  char *v5; // r4@1
  int i; // r0@1
  char v8; // [sp+4h] [bp-2Ch]@1

  v4 = a1;
  v5 = a3;
  _2sprintf(&v8, "%%.%df");
  for ( i = _2sprintf(v5, &v8, v4); i >= 0; --i )
  {
    if ( v5[i] == 48 )
      v5[i] = 0;
  }
  return v5;
}
// 1710: using guessed type int _2sprintf(_DWORD, const char *, ...);

//----- (000002C8) --------------------------------------------------------
int __fastcall StringSplit::Next(StringSplit *this, int a2)
{
  StringSplit *v2; // r7@1
  int v3; // r4@1
  int v4; // r8@1
  int v5; // r9@1
  int v6; // r5@3
  int v7; // r5@4
  int v8; // r0@8
  int v9; // r10@9
  String v11; // [sp+4h] [bp-74h]@3

  v2 = this;
  v3 = a2;
  v4 = 0;
  v5 = 0;
  if ( *(_DWORD *)(a2 + 4) >= -1 && *(_DWORD *)a2 )
  {
    String::String((int)&v11, *(const char **)a2);
    v6 = *(_QWORD *)(v3 + 4) + (*(_QWORD *)(v3 + 4) >> 32);
    if ( v6 >= 0 )
      v7 = v6 + Buffer::Length((Buffer *)&v11);
    else
      v7 = 0;
    if ( Buffer::Length(*(Buffer **)(v3 + 12)) > v7 )
    {
      v8 = String::IndexOf(*(String **)(v3 + 12), *(const char **)v3, v7);
      if ( v8 >= 0 )
        v9 = v8 - v7;
      else
        v9 = Buffer::Length(*(Buffer **)(v3 + 12)) - v7;
      *(_DWORD *)(v3 + 4) = v7;
      *(_DWORD *)(v3 + 8) = v9;
      v4 = *(_DWORD *)(*(_DWORD *)(v3 + 12) + 4) + *(_DWORD *)(v3 + 4);
      v5 = *(_DWORD *)(v3 + 8);
    }
    else
    {
      *(_DWORD *)(v3 + 4) = -2;
      *(_DWORD *)(v3 + 8) = 0;
    }
    String::~String(&v11);
  }
  return String::String((int)v2, v4, v5);
}

//----- (0000036E) --------------------------------------------------------
int __fastcall StringSplit::StringSplit(int result, const String *a2, const char *a3)
{
  *(_DWORD *)(result + 12) = a2;
  *(_DWORD *)result = a3;
  *(_DWORD *)(result + 4) = -1;
  *(_DWORD *)(result + 8) = 0;
  return result;
}

//----- (0000037E) --------------------------------------------------------
char *__fastcall _INTERNAL_10_String_cpp_f0ff2ffe::ftoa(_INTERNAL_10_String_cpp_f0ff2ffe *this, char *a2, int a3, int a4, double a5, int a6)
{
  char *v6; // r6@1
  int v7; // r10@1
  int v8; // r9@1
  int v9; // r4@1
  int v10; // r1@1
  int v11; // r5@1
  char v12; // cf@1
  size_t v13; // r7@5
  signed int i; // r8@5
  int v15; // r0@8
  int v16; // r1@8
  int v17; // r0@8
  int v18; // r1@8
  int v19; // r2@8
  int v20; // r3@8
  __int64 v21; // r0@8
  size_t v22; // r7@8
  int v23; // r0@8

  v6 = (char *)this;
  v7 = LODWORD(a5);
  v8 = _aeabi_d2iz(a3, a4);
  ((void (*)(void))_aeabi_i2d)();
  v9 = _aeabi_drsub();
  v11 = v10;
  _aeabi_cdcmple();
  if ( !v12 )
    v11 ^= 0x80000000;
  ltoa(v8, v6, 10);
  if ( SLODWORD(a5) > 0 )
  {
    _aeabi_cdrcmple(v9, v11);
    if ( !v12 )
    {
      v13 = strlen(v6);
      for ( i = 10; --v7 > 0; i *= 10 )
        ;
      v6[v13] = 46;
      v15 = _aeabi_i2d(i);
      v17 = _aeabi_dmul(v15, v16);
      LODWORD(v21) = _aeabi_d2lz(v17, v18, v19, v20);
      ltoa(v21, &v6[v13 + 1], 10);
      v22 = strlen(v6);
      v23 = v22 - 1;
      while ( v23 >= 0 && v6[v23] == 48 )
      {
        v6[v23--] = 0;
        --v22;
      }
      if ( v6[v22 - 1] == 46 )
        v6[v22 - 1] = 0;
    }
  }
  return v6;
}
// 1714: using guessed type int _aeabi_cdcmple(void);
// 1718: using guessed type int __fastcall _aeabi_cdrcmple(_DWORD, _DWORD);
// 1720: using guessed type int __fastcall _aeabi_d2iz(_DWORD, _DWORD);
// 1724: using guessed type int __cdecl _aeabi_d2lz(_DWORD, _DWORD, _DWORD, _DWORD);
// 1728: using guessed type int __cdecl _aeabi_dmul(_DWORD, _DWORD);
// 172C: using guessed type int _aeabi_drsub(void);
// 1734: using guessed type int __fastcall _aeabi_i2d(_DWORD);

//----- (00000466) --------------------------------------------------------
signed int __fastcall String::CopyOrWrite(String *this)
{
  signed int result; // r0@2

  if ( *((_BYTE *)this + 14) )
    result = 0;
  else
    result = String::CheckCapacity(this, (int)this[2]);
  return result;
}

//----- (0000047C) --------------------------------------------------------
signed int __fastcall String::CheckCapacity(String *this, int a2)
{
  String *v2; // r4@1
  String v3; // r6@1
  signed int result; // r0@2

  v2 = this;
  v3 = this[4];
  Array::CheckCapacity((Array *)this, a2 + 1, (int)this[2]);
  if ( v2[4] == v3 )
  {
    result = 1;
  }
  else
  {
    *(_BYTE *)(*(_QWORD *)(v2 + 1) + (*(_QWORD *)(v2 + 1) >> 32)) = 0;
    --v2[4];
    result = 1;
  }
  return result;
}

//----- (000004AA) --------------------------------------------------------
int __fastcall String::Copy(String *this, int a2, const void *a3, int a4)
{
  String *v4; // r4@1
  int result; // r0@1

  v4 = this;
  result = Buffer::Copy((Buffer *)this, a2, a3, a4);
  if ( result )
    *(_BYTE *)(*(_QWORD *)(v4 + 1) + (*(_QWORD *)(v4 + 1) >> 32)) = 0;
  else
    result = 0;
  return result;
}

//----- (000004D8) --------------------------------------------------------
int __fastcall String::copy(int result, const char *a2, int a3, int a4)
{
  int v4; // r5@1
  const char *v5; // r6@1
  int v6; // r4@1
  int v7; // r2@7
  int v8; // r1@7
  Buffer *v9; // r0@7
  const char *v10; // [sp+0h] [bp-20h]@1
  int v11; // [sp+4h] [bp-1Ch]@1
  int v12; // [sp+8h] [bp-18h]@1

  v10 = a2;
  v11 = a3;
  v12 = a4;
  v4 = result;
  v5 = a2;
  v6 = a3;
  if ( a2 && a3 )
  {
    if ( String::CheckCapacity((String *)result, a3) )
    {
      *(_DWORD *)(v4 + 8) = v6;
      if ( v6 )
      {
        v7 = *(_DWORD *)(v4 + 16);
        v8 = *(_DWORD *)(v4 + 4);
        v9 = (Buffer *)Buffer::Buffer(&v10);
        Buffer::Copy(v9, 0, v5, v6);
      }
      *(_BYTE *)(*(_DWORD *)(v4 + 4) + v6) = 0;
    }
    else
    {
      String::release((String *)v4);
    }
    result = v4;
  }
  return result;
}
// 16F4: using guessed type int __fastcall Buffer::Buffer(_DWORD);

//----- (00000520) --------------------------------------------------------
int __fastcall String::move(String *this, String *a2)
{
  String *v2; // r5@1
  String *v3; // r4@1
  int result; // r0@3
  int v5; // r0@5
  int v6; // r3@5

  v2 = this;
  v3 = a2;
  if ( (String *)a2[1] != a2 + 5 && *((_BYTE *)a2 + 13) )
  {
    result = Array::move((Array *)this, (Array *)a2);
  }
  else
  {
    v5 = Buffer::Length((Buffer *)a2);
    (*((void (__fastcall **)(_DWORD, _DWORD))*v2 + 3))(v2, v5);
    result = String::copy((int)v2, (const char *)*(_QWORD *)(v3 + 1), *(_QWORD *)(v3 + 1) >> 32, v6);
  }
  return result;
}

//----- (0000055E) --------------------------------------------------------
String *__fastcall String::Alloc(String *this, int a2)
{
  String *result; // r0@2

  if ( a2 > 64 )
  {
    *((_BYTE *)this + 13) = 1;
    result = (String *)operator new[](a2);
  }
  else
  {
    *((_BYTE *)this + 13) = 0;
    result = this + 5;
  }
  return result;
}

//----- (0000057E) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, const char *a2)
{
  String *v2; // r5@1
  const char *v3; // r4@1
  signed int result; // r0@2
  int v5; // r0@4
  int v6; // r3@4

  v2 = this;
  v3 = a2;
  if ( a2 )
  {
    v5 = strlen(a2);
    result = String::Concat(v2, v3, v5, v6);
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0000059E) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, const char *a2, int a3, int a4)
{
  String *v4; // r4@1
  const char *v5; // r6@1
  int v6; // r5@1
  signed int result; // r0@2
  char *v8; // r7@6
  String v9; // r2@8
  String v10; // r1@8
  Buffer *v11; // r0@8
  const char *v12; // [sp+0h] [bp-28h]@1
  int v13; // [sp+4h] [bp-24h]@1
  int v14; // [sp+8h] [bp-20h]@1

  v12 = a2;
  v13 = a3;
  v14 = a4;
  v4 = this;
  v5 = a2;
  v6 = a3;
  if ( a2 )
  {
    if ( a3 )
    {
      v8 = &this[2][a3];
      if ( String::CheckCapacity(this, (int)v8) )
      {
        v9 = v4[4];
        v10 = v4[1];
        v11 = (Buffer *)Buffer::Buffer(&v12);
        Buffer::Copy(v11, (int)v4[2], v5, v6);
        v4[2] = v8;
        *(_BYTE *)(*(_QWORD *)(v4 + 1) + (*(_QWORD *)(v4 + 1) >> 32)) = 0;
        result = 1;
      }
      else
      {
        result = 0;
      }
    }
    else
    {
      result = 1;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}
// 16F4: using guessed type int __fastcall Buffer::Buffer(_DWORD);

//----- (000005EC) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, const Object *a2)
{
  String *v2; // r5@1
  int v3; // r2@1
  int v4; // r3@1
  signed int v5; // r6@1
  String v7; // [sp+4h] [bp-64h]@1

  v2 = this;
  ((void (__fastcall *)(String *))(*a2)->object.widget_class)(&v7);
  v5 = String::Concat(v2, &v7, v3, v4);
  String::~String(&v7);
  return v5;
}

//----- (00000614) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, const String *a2, int a3, int a4)
{
  return String::Concat(this, (const char *)*(_QWORD *)(a2 + 1), *(_QWORD *)(a2 + 1) >> 32, a4);
}

//----- (00000628) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, int a2)
{
  String *v2; // r5@1
  const char *v3; // r0@2

  v2 = this;
  if ( a2 )
    v3 = "true";
  else
    v3 = "false";
  return String::Concat(v2, v3);
}

//----- (00000650) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, char a2)
{
  String *v2; // r4@1
  char v3; // r5@1
  signed int result; // r0@2
  String v5; // r1@4

  v2 = this;
  v3 = a2;
  if ( String::CheckCapacity(this, (int)(this[2] + 1)) )
  {
    v5 = v2[2];
    v2[2] = v5 + 1;
    v5[(_DWORD)v2[1]] = v3;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00000674) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, int a2, double a3, double a4)
{
  String *v4; // r6@1
  int v5; // r0@1
  int v6; // r3@1
  double v8; // [sp+0h] [bp-30h]@1
  int v9; // [sp+8h] [bp-28h]@1

  v4 = this;
  _INTERNAL_10_String_cpp_f0ff2ffe::ftoa(
    (_INTERNAL_10_String_cpp_f0ff2ffe *)((char *)&v8 + 4),
    (char *)&locret_14,
    SLODWORD(a3),
    SHIDWORD(a3),
    v8,
    v9);
  v5 = strlen((const char *)&v8 + 4);
  return String::Concat(v4, (const char *)&v8 + 4, v5, v6);
}

//----- (000006A8) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, float a2, int a3)
{
  String *v3; // r8@1
  int v4; // r0@1
  int v5; // r1@1
  int v6; // r0@1
  int v7; // r3@1
  double v9; // [sp+0h] [bp-30h]@1
  int v10; // [sp+8h] [bp-28h]@1

  v3 = this;
  v4 = _aeabi_f2d(LODWORD(a2));
  _INTERNAL_10_String_cpp_f0ff2ffe::ftoa(
    (_INTERNAL_10_String_cpp_f0ff2ffe *)((char *)&v9 + 4),
    (char *)&locret_14,
    v4,
    v5,
    v9,
    v10);
  v6 = strlen((const char *)&v9 + 4);
  return String::Concat(v3, (const char *)&v9 + 4, v6, v7);
}
// 1730: using guessed type int __fastcall _aeabi_f2d(_DWORD);

//----- (000006E2) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, int a2, __int64 a3)
{
  String *v3; // r4@1
  unsigned int v4; // r6@1
  unsigned int v5; // r5@1
  signed int result; // r0@4
  int v7; // r0@7
  int v8; // r3@7
  int v9; // [sp+0h] [bp-18h]@1

  v9 = HIDWORD(a3);
  v3 = this;
  v4 = a2;
  v5 = a3;
  if ( (_DWORD)a3 != 16 && (_DWORD)a3 != -16 )
  {
    itoa(__PAIR__(&v9, a2), (char *)a3, (char *)HIDWORD(a3));
    v7 = strlen((const char *)&v9);
    result = String::Concat(v3, (const char *)&v9, v7, v8);
  }
  else if ( String::CheckCapacity(this, (int)(this[2] + 2)) )
  {
    utohex(v4, 1, (char *)(*(_QWORD *)(v3 + 1) + (*(_QWORD *)(v3 + 1) >> 32)), v5 >> 31);
    v3[2] += 2;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0000073C) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, unsigned int a2, unsigned __int64 a3)
{
  String *v3; // r6@1
  signed int result; // r0@3
  int v5; // r0@5
  int v6; // r3@5
  String *v7; // [sp+0h] [bp-28h]@1
  unsigned int v8; // [sp+4h] [bp-24h]@1
  unsigned __int64 v9; // [sp+8h] [bp-20h]@1

  v7 = this;
  v8 = a2;
  v9 = a3;
  v3 = this;
  if ( (_DWORD)a3 != 16 && (_DWORD)a3 != -16 )
  {
    itoa(__PAIR__(&v7, a2), (char *)a3, (char *)HIDWORD(a3));
    v5 = strlen((const char *)&v7);
    result = String::Concat(v3, (const char *)&v7, v5, v6);
  }
  else
  {
    result = String::Concat(this, a2, a3);
  }
  return result;
}

//----- (0000077E) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, unsigned int a2, unsigned __int64 a3)
{
  String *v3; // r4@1
  unsigned int v4; // r6@1
  unsigned int v5; // r5@1
  signed int result; // r0@4
  int v7; // r0@7
  int v8; // r3@7
  String *v9; // [sp+0h] [bp-28h]@1
  unsigned int v10; // [sp+4h] [bp-24h]@1
  unsigned __int64 v11; // [sp+8h] [bp-20h]@1

  v9 = this;
  v10 = a2;
  v11 = a3;
  v3 = this;
  v4 = a2;
  v5 = a3;
  if ( (_DWORD)a3 != 16 && (_DWORD)a3 != -16 )
  {
    utoa(__PAIR__(&v9, a2), (char *)a3, (char *)HIDWORD(a3));
    v7 = strlen((const char *)&v9);
    result = String::Concat(v3, (const char *)&v9, v7, v8);
  }
  else if ( String::CheckCapacity(this, (int)(this[2] + 8)) )
  {
    utohex(v4, 4, (char *)(*(_QWORD *)(v3 + 1) + (*(_QWORD *)(v3 + 1) >> 32)), v5 >> 31);
    v3[2] += 8;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (000007E0) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, int a2, __int64 a3)
{
  String *v3; // r6@1
  signed int result; // r0@3
  int v5; // r0@5
  int v6; // r3@5
  String *v7; // [sp+0h] [bp-28h]@1
  int v8; // [sp+4h] [bp-24h]@1
  __int64 v9; // [sp+8h] [bp-20h]@1

  v7 = this;
  v8 = a2;
  v9 = a3;
  v3 = this;
  if ( (_DWORD)a3 != 16 && (_DWORD)a3 != -16 )
  {
    itoa(__PAIR__(&v7, a2), (char *)a3, (char *)HIDWORD(a3));
    v5 = strlen((const char *)&v7);
    result = String::Concat(v3, (const char *)&v7, v5, v6);
  }
  else
  {
    result = String::Concat(this, (unsigned __int16)a2, a3);
  }
  return result;
}

//----- (00000822) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, int a2, unsigned __int64 a3)
{
  String *v3; // r4@1
  unsigned int v4; // r6@1
  unsigned int v5; // r5@1
  signed int result; // r0@4
  int v7; // r0@7
  int v8; // r3@7
  String *v9; // [sp+0h] [bp-28h]@1
  int v10; // [sp+4h] [bp-24h]@1
  unsigned __int64 v11; // [sp+8h] [bp-20h]@1

  v9 = this;
  v10 = a2;
  v11 = a3;
  v3 = this;
  v4 = a2;
  v5 = a3;
  if ( (_DWORD)a3 != 16 && (_DWORD)a3 != -16 )
  {
    utoa(__PAIR__(&v9, a2), (char *)a3, (char *)HIDWORD(a3));
    v7 = strlen((const char *)&v9);
    result = String::Concat(v3, (const char *)&v9, v7, v8);
  }
  else if ( String::CheckCapacity(this, (int)(this[2] + 4)) )
  {
    utohex(v4, 2, (char *)(*(_QWORD *)(v3 + 1) + (*(_QWORD *)(v3 + 1) >> 32)), v5 >> 31);
    v3[2] += 4;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00000882) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, int a2, unsigned __int64 a3, int a4)
{
  String *v4; // r7@1
  signed int result; // r0@3
  int v6; // r0@5
  int v7; // r3@5
  char s; // [sp+4h] [bp-34h]@5

  v4 = this;
  if ( a4 != 16 && a4 != -16 )
  {
    ltoa(a3, &s, a4);
    v6 = strlen(&s);
    result = String::Concat(v4, &s, v6, v7);
  }
  else
  {
    result = String::Concat(this, a2, a3, a4);
  }
  return result;
}

//----- (000008CC) --------------------------------------------------------
signed int __fastcall String::Concat(String *this, int a2, unsigned __int64 a3, int a4)
{
  String *v4; // r4@1
  unsigned __int64 v5; // r6@1
  signed int result; // r0@4
  int v7; // r0@7
  int v8; // r3@7
  char s; // [sp+4h] [bp-34h]@7

  v4 = this;
  v5 = a3;
  if ( a4 != 16 && a4 != -16 )
  {
    ultoa(a3, &s, a4);
    v7 = strlen(&s);
    result = String::Concat(v4, &s, v7, v8);
  }
  else if ( String::CheckCapacity(this, (int)(this[2] + 16)) )
  {
    utohex(HIDWORD(v5), 4, (char *)(*(_QWORD *)(v4 + 1) + (*(_QWORD *)(v4 + 1) >> 32)), (unsigned int)a4 >> 31);
    v4[2] += 8;
    utohex(v5, 4, (char *)(*(_QWORD *)(v4 + 1) + (*(_QWORD *)(v4 + 1) >> 32)), (unsigned int)a4 >> 31);
    v4[2] += 8;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0000094E) --------------------------------------------------------
String *String::Format(String *this, const char *a2, ...)
{
  const char *varg_r1; // ST1C_4@1
  String *v3; // r4@1
  size_t v4; // r0@1
  va_list va; // [sp+20h] [bp-8h]@1

  va_start(va, a2);
  varg_r1 = a2;
  v3 = this;
  v4 = strlen(a2);
  String::CheckCapacity(v3, (int)&v3[2][2 * v4]);
  v3[2] += _c89vsnprintf(&v3[2][(_DWORD)v3[1]], v3[4] - v3[2], varg_r1, va);
  return v3;
}
// 173C: using guessed type int __fastcall _c89vsnprintf(_DWORD, _DWORD, _DWORD, _DWORD);

//----- (00000992) --------------------------------------------------------
int __fastcall String::Compare(String *this, const void *a2, const void *a3)
{
  String *v3; // r4@1
  const char *v4; // r5@1
  int result; // r0@3
  size_t v6; // r0@8

  v3 = this;
  v4 = (const char *)a2;
  if ( this )
  {
    if ( a2 )
    {
      v6 = strlen((const char *)this);
      result = strncmp((const char *)v3, v4, v6);
    }
    else
    {
      result = 1;
    }
  }
  else if ( this == a2 )
  {
    result = 0;
  }
  else
  {
    result = -1;
  }
  return result;
}

//----- (000009CA) --------------------------------------------------------
int __fastcall String::release(String *this)
{
  String *v1; // r4@1

  v1 = this;
  Array::Release((Array *)this);
  return String::init((int)v1);
}

//----- (000009DC) --------------------------------------------------------
signed int __fastcall String::SetLength(String *this, int a2, int a3)
{
  String *v3; // r4@1
  int v4; // r5@1
  String v5; // r0@4

  v3 = this;
  v4 = a2;
  if ( (signed int)this[4] <= a2 )
  {
    if ( a3 )
      v5 = this[2];
    else
      v5 = 0;
    if ( !Array::CheckCapacity((Array *)v3, a2 + 1, (int)v5) )
      return 0;
    if ( (signed int)v3[2] < v4 )
      v3[2] = (String)v4;
  }
  else
  {
    this[2] = (String)a2;
  }
  *(_BYTE *)(*(_QWORD *)(v3 + 1) + (*(_QWORD *)(v3 + 1) >> 32)) = 0;
  return 1;
}

//----- (00000A1C) --------------------------------------------------------
String *__fastcall String::String(int a1, String *a2)
{
  String *v2; // r5@1
  String *v3; // r0@1
  String *v4; // r4@1

  v2 = a2;
  v3 = (String *)Array::Array(a1);
  v4 = v3;
  *v3 = (String)&off_1630;
  String::init((int)v3);
  String::move(v4, v2);
  return v4;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000A4C) --------------------------------------------------------
int __fastcall String::String(int a1, const char *a2)
{
  const char *v2; // r5@1
  int v3; // r4@1
  size_t v4; // r0@2

  v2 = a2;
  v3 = Array::Array(a1);
  *(_DWORD *)v3 = &off_1630;
  String::init(v3);
  if ( v2 )
    v4 = strlen(v2);
  else
    v4 = 0;
  *(_DWORD *)(v3 + 8) = v4;
  if ( *(_DWORD *)(v3 + 8) )
  {
    *(_DWORD *)(v3 + 4) = v2;
    *(_DWORD *)(v3 + 16) = *(_DWORD *)(v3 + 8) + 1;
    *(_BYTE *)(v3 + 14) = 0;
  }
  return v3;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000A90) --------------------------------------------------------
int __fastcall String::String(int a1, int a2, int a3)
{
  int v3; // r5@1
  int result; // r0@1

  v3 = a3;
  result = Array::Array(a1);
  *(_DWORD *)result = &off_1630;
  *(_DWORD *)(result + 16) = v3 + 1;
  *(_BYTE *)(result + 14) = 0;
  return result;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000AB8) --------------------------------------------------------
int __fastcall String::String(int a1, const char *a2, signed int a3)
{
  const char *v3; // r6@1
  signed int v4; // r5@1
  int v5; // r4@1
  signed int v6; // r7@1

  v3 = a2;
  v4 = a3;
  v5 = Array::Array(a1);
  *(_DWORD *)v5 = &off_1630;
  *(_DWORD *)(v5 + 4) = v3;
  *(_DWORD *)(v5 + 16) = v4 - 1;
  v6 = strlen(v3);
  if ( v6 >= v4 )
    v6 = v4 - 1;
  *(_DWORD *)(v5 + 8) = v6;
  *(_BYTE *)(*(_QWORD *)(v5 + 4) + (*(_QWORD *)(v5 + 4) >> 32)) = 0;
  return v5;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000AFC) --------------------------------------------------------
int __fastcall String::String(int a1, const char *a2, signed int a3, char a4)
{
  char v4; // r5@1
  int result; // r0@1

  v4 = a4;
  result = String::String(a1, a2, a3);
  *(_BYTE *)(result + 12) = v4;
  return result;
}

//----- (00000B1C) --------------------------------------------------------
int __fastcall String::String(int a1, int a2)
{
  int v2; // r5@1
  _DWORD *v3; // r0@1
  int v4; // r4@1
  int v5; // r2@1
  int v6; // r3@1

  v2 = a2;
  v3 = (_DWORD *)Array::Array(a1);
  v4 = (int)v3;
  *v3 = &off_1630;
  String::init((int)v3);
  String::operator=(v4, v2, v5, v6);
  return v4;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000B4C) --------------------------------------------------------
String *__fastcall String::String(int a1, int a2)
{
  int v2; // r5@1
  String *v3; // r0@1
  String *v4; // r4@1

  v2 = a2;
  v3 = (String *)Array::Array(a1);
  v4 = v3;
  *v3 = (String)&off_1630;
  String::init((int)v3);
  String::Concat(v4, v2);
  return v4;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000B7C) --------------------------------------------------------
_DWORD *__fastcall String::String(int a1, char a2)
{
  char v2; // r5@1
  _DWORD *v3; // r0@1
  _DWORD *v4; // r4@1

  v2 = a2;
  v3 = (_DWORD *)Array::Array(a1);
  v4 = v3;
  *v3 = &off_1630;
  String::init((int)v3);
  *(_BYTE *)v4[1] = v2;
  *(_BYTE *)(v4[1] + 1) = 0;
  v4[2] = 1;
  return v4;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000BB0) --------------------------------------------------------
String *__fastcall String::String(int a1, int a2, unsigned int a3, unsigned int a4, int a5)
{
  unsigned int v5; // r5@1
  unsigned int v6; // r6@1
  String *v7; // r0@1
  String *v8; // r4@1
  int v9; // r1@1
  double v11; // [sp+0h] [bp-18h]@0

  v5 = a3;
  v6 = a4;
  v7 = (String *)Array::Array(a1);
  v8 = v7;
  *v7 = (String)&off_1630;
  String::init((int)v7);
  LODWORD(v11) = a5;
  String::Concat(v8, v9, COERCE_DOUBLE(__PAIR__(v6, v5)), v11);
  return v8;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000BE8) --------------------------------------------------------
String *__fastcall String::String(int a1, float a2, int a3)
{
  float v3; // r5@1
  int v4; // r6@1
  String *v5; // r0@1
  String *v6; // r4@1

  v3 = a2;
  v4 = a3;
  v5 = (String *)Array::Array(a1);
  v6 = v5;
  *v5 = (String)&off_1630;
  String::init((int)v5);
  String::Concat(v6, v3, v4);
  return v6;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000C1C) --------------------------------------------------------
String *__fastcall String::String(int a1, int a2, int a3)
{
  int v3; // r5@1
  int v4; // r6@1
  String *v5; // r0@1
  String *v6; // r4@1
  __int64 v7; // r2@1

  v3 = a2;
  v4 = a3;
  v5 = (String *)Array::Array(a1);
  v6 = v5;
  *v5 = (String)&off_1630;
  String::init((int)v5);
  LODWORD(v7) = v4;
  String::Concat(v6, v3, v7);
  return v6;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000C50) --------------------------------------------------------
String *__fastcall String::String(int a1, unsigned int a2, int a3)
{
  unsigned int v3; // r5@1
  int v4; // r6@1
  String *v5; // r0@1
  String *v6; // r4@1
  unsigned __int64 v7; // r2@1

  v3 = a2;
  v4 = a3;
  v5 = (String *)Array::Array(a1);
  v6 = v5;
  *v5 = (String)&off_1630;
  String::init((int)v5);
  LODWORD(v7) = v4;
  String::Concat(v6, v3, v7);
  return v6;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000C84) --------------------------------------------------------
String *__fastcall String::String(int a1, unsigned int a2, int a3)
{
  unsigned int v3; // r5@1
  int v4; // r6@1
  String *v5; // r0@1
  String *v6; // r4@1
  unsigned __int64 v7; // r2@1

  v3 = a2;
  v4 = a3;
  v5 = (String *)Array::Array(a1);
  v6 = v5;
  *v5 = (String)&off_1630;
  String::init((int)v5);
  LODWORD(v7) = v4;
  String::Concat(v6, v3, v7);
  return v6;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000CB8) --------------------------------------------------------
String *__fastcall String::String(int a1, int a2, int a3)
{
  int v3; // r5@1
  int v4; // r6@1
  String *v5; // r0@1
  String *v6; // r4@1
  __int64 v7; // r2@1

  v3 = a2;
  v4 = a3;
  v5 = (String *)Array::Array(a1);
  v6 = v5;
  *v5 = (String)&off_1630;
  String::init((int)v5);
  LODWORD(v7) = v4;
  String::Concat(v6, v3, v7);
  return v6;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000CEC) --------------------------------------------------------
String *__fastcall String::String(int a1, int a2, int a3)
{
  int v3; // r5@1
  int v4; // r6@1
  String *v5; // r0@1
  String *v6; // r4@1
  unsigned __int64 v7; // r2@1

  v3 = a2;
  v4 = a3;
  v5 = (String *)Array::Array(a1);
  v6 = v5;
  *v5 = (String)&off_1630;
  String::init((int)v5);
  LODWORD(v7) = v4;
  String::Concat(v6, v3, v7);
  return v6;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000D20) --------------------------------------------------------
String *__fastcall String::String(int a1, int a2, unsigned int a3, unsigned int a4, int a5)
{
  unsigned int v5; // r5@1
  unsigned int v6; // r6@1
  String *v7; // r0@1
  String *v8; // r4@1
  int v9; // r1@1

  v5 = a3;
  v6 = a4;
  v7 = (String *)Array::Array(a1);
  v8 = v7;
  *v7 = (String)&off_1630;
  String::init((int)v7);
  String::Concat(v8, v9, __PAIR__(v6, v5), a5);
  return v8;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000D58) --------------------------------------------------------
String *__fastcall String::String(int a1, int a2, unsigned int a3, unsigned int a4, int a5)
{
  unsigned int v5; // r5@1
  unsigned int v6; // r6@1
  String *v7; // r0@1
  String *v8; // r4@1
  int v9; // r1@1

  v5 = a3;
  v6 = a4;
  v7 = (String *)Array::Array(a1);
  v8 = v7;
  *v7 = (String)&off_1630;
  String::init((int)v7);
  String::Concat(v8, v9, __PAIR__(v6, v5), a5);
  return v8;
}
// 1630: using guessed type void *off_1630;
// 16E8: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000D90) --------------------------------------------------------
String *__fastcall String::operator=(String *this, String *a2)
{
  String *v2; // r5@1

  v2 = this;
  if ( this != a2 )
    String::move(this, a2);
  return v2;
}

//----- (00000DA6) --------------------------------------------------------
String *__fastcall String::operator=(String *this, char *s)
{
  String *v2; // r5@1
  char *v3; // r4@1
  int v4; // r0@2
  int v5; // r3@2

  v2 = this;
  v3 = s;
  if ( s )
  {
    v4 = strlen(s);
    String::copy((int)v2, v3, v4, v5);
  }
  else
  {
    String::release(this);
  }
  return v2;
}

//----- (00000DCC) --------------------------------------------------------
int __fastcall String::operator=(int result, int a2, int a3, int a4)
{
  int v4; // r5@1

  v4 = result;
  if ( result != a2 )
  {
    if ( *(_DWORD *)(a2 + 4) )
      String::copy(result, (const char *)*(_QWORD *)(a2 + 4), *(_QWORD *)(a2 + 4) >> 32, a4);
    else
      String::release((String *)result);
    result = v4;
  }
  return result;
}

//----- (00000DF4) --------------------------------------------------------
char *__fastcall String::operator[](String *this, signed int a2)
{
  String *v2; // r4@1
  signed int v3; // r5@1
  char *result; // r0@3

  v2 = this;
  v3 = a2;
  if ( (signed int)this[2] > a2 && this[1] )
  {
    String::CopyOrWrite(this);
    result = &v2[1][v3];
  }
  else
  {
    String::operator[](int)::dummy_writable_char = 0;
    result = &String::operator[](int)::dummy_writable_char;
  }
  return result;
}
// 1626: using guessed type char String::operator[](int)::dummy_writable_char;

//----- (00000E20) --------------------------------------------------------
int __fastcall String::StartsWith(String *this, const char *a2, int a3)
{
  String *v3; // r5@1
  const char *v4; // r6@1
  int v5; // r7@1
  int result; // r0@3
  size_t v7; // r4@5

  v3 = this;
  v4 = a2;
  v5 = a3;
  if ( this[1] && a2 )
  {
    v7 = strlen(a2);
    if ( v7 )
    {
      if ( (signed int)(v5 + v7) <= (signed int)v3[2] )
        result = strncmp(&v3[1][v5], v4, v7) == 0;
      else
        result = 0;
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00000E66) --------------------------------------------------------
int __fastcall String::StartsWith(String *this, const String *a2, int a3)
{
  int result; // r0@3

  if ( this[1] && *((_DWORD *)a2 + 1) )
  {
    if ( *((_DWORD *)a2 + 2) )
    {
      if ( *((_DWORD *)a2 + 2) + a3 <= (signed int)this[2] )
        result = strncmp(&this[1][a3], (const char *)*(_QWORD *)(a2 + 1), *(_QWORD *)(a2 + 1) >> 32) == 0;
      else
        result = 0;
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (00000EA6) --------------------------------------------------------
int __fastcall String::LastIndexOf(String *this, const char *a2, int a3)
{
  String *v3; // r6@1
  const char *v4; // r4@1
  int v5; // r5@1
  int v6; // r0@1

  v3 = this;
  v4 = a2;
  v5 = a3;
  v6 = strlen(a2);
  return String::Search(v3, v4, v6, v5, 1);
}

//----- (00000EC8) --------------------------------------------------------
int __fastcall String::LastIndexOf(String *this, const String *a2, int a3)
{
  return String::Search(this, (const char *)*(_QWORD *)(a2 + 1), *(_QWORD *)(a2 + 1) >> 32, a3, 1);
}

//----- (00000EE2) --------------------------------------------------------
String __fastcall String::LastIndexOf(String *this, int a2, int a3)
{
  String *v3; // r3@1
  String result; // r0@2

  v3 = this;
  if ( (signed int)this[2] > a3 )
  {
    for ( result = this[2] - 1; (signed int)result >= a3; --result )
    {
      if ( result[(_DWORD)v3[1]] == a2 )
        return result;
    }
    result = (String)-1;
  }
  else
  {
    result = (String)-1;
  }
  return result;
}

//----- (00000F0E) --------------------------------------------------------
int __fastcall String::EqualsIgnoreCase(String *this, const char *a2)
{
  String *v2; // r4@1
  const char *v3; // r5@1
  int v4; // r6@1
  int result; // r0@2

  v2 = this;
  v3 = a2;
  v4 = strlen(a2);
  if ( v2[2] == (String)v4 )
    result = String::CompareTo(v2, v3, v4, 1) == 0;
  else
    result = 0;
  return result;
}

//----- (00000F3C) --------------------------------------------------------
_BOOL4 __fastcall String::EqualsIgnoreCase(String *this, const String *a2)
{
  return this[2] == a2[2] && !String::CompareTo(this, (const char *)*(_QWORD *)(a2 + 1), *(_QWORD *)(a2 + 1) >> 32, 1);
}

//----- (00000F60) --------------------------------------------------------
int __fastcall String::Show(String *this, int a2)
{
  String *v2; // r4@1
  int v3; // r6@1
  int i; // r5@1
  int result; // r0@3

  v2 = this;
  v3 = a2;
  for ( i = 0; ; ++i )
  {
    result = (int)v2[2];
    if ( result <= i )
      break;
    SmartOS_printf((const char *)&dword_F88, (unsigned __int8)v2[1][i]);
  }
  if ( v3 )
    result = SmartOS_printf((const char *)&dword_F8C);
  return result;
}
// F88: using guessed type int dword_F88;
// F8C: using guessed type int dword_F8C;
// 16C8: using guessed type int SmartOS_printf(const char *, ...);

//----- (00000F90) --------------------------------------------------------
int __fastcall String::Trim(String *this, int a2, int a3, int a4)
{
  String *v4; // r5@1
  int v5; // r0@1
  int v7; // [sp+0h] [bp-18h]@1

  v7 = a4;
  v4 = this;
  v7 = *(_DWORD *)(a2 + 8);
  v5 = trim(*(_DWORD *)(a2 + 4), &v7, 1, 1);
  return String::String((int)v4, v5, v7);
}

//----- (00000FB6) --------------------------------------------------------
int __fastcall String::Split(String *this, String *a2, const char *a3)
{
  return StringSplit::StringSplit((int)this, a2, a3);
}

//----- (00000FCA) --------------------------------------------------------
int __fastcall String::Split(String *this, const String *a2, int a3)
{
  return StringSplit::StringSplit((int)this, a2, *(const char **)(a3 + 4));
}

//----- (00000FE4) --------------------------------------------------------
int __fastcall String::ToHex(String *this, int a2, int a3, int a4)
{
  int v4; // r5@1
  int v5; // r7@1
  _BYTE *v6; // r8@1
  _BYTE *v7; // r4@1
  int v8; // r9@1
  int i; // r6@1
  _BYTE *v10; // r4@2
  char v11; // t1@2
  char v12; // t1@2
  int v14; // [sp+0h] [bp-20h]@1

  v14 = a4;
  v4 = a2;
  v5 = ByteArray::ByteArray((ByteArray *)this, 0);
  Array::SetLength((Array *)v5, *(_DWORD *)(v4 + 8) / 2);
  BYTE2(v14) = 0;
  v6 = *(_BYTE **)(v5 + 4);
  v7 = *(_BYTE **)(v4 + 4);
  v8 = 0;
  for ( i = 0; *(_DWORD *)(v4 + 8) > i; i += 2 )
  {
    v11 = *v7;
    v10 = v7 + 1;
    LOBYTE(v14) = v11;
    v12 = *v10;
    v7 = v10 + 1;
    BYTE1(v14) = v12;
    *v6++ = strtol((const char *)&v14, 0, 16);
    if ( *v7 == 45 || isspace(*v7) )
    {
      ++v7;
      ++i;
    }
    ++v8;
  }
  return Array::SetLength((Array *)v5, v8);
}

//----- (00001064) --------------------------------------------------------
int __fastcall String::ToInt(String *this)
{
  String *v1; // r4@1
  int result; // r0@2
  int v3; // r3@6
  int v4; // r5@6
  int v5; // [sp+0h] [bp-60h]@6

  v1 = this;
  if ( this[2] )
  {
    if ( *(_BYTE *)(*(_QWORD *)(this + 1) + (*(_QWORD *)(this + 1) >> 32)) )
    {
      String::String((int)&v5, (const char *)&dword_10A8);
      String::copy((int)&v5, (const char *)*(_QWORD *)(v1 + 1), *(_QWORD *)(v1 + 1) >> 32, v3);
      v4 = String::ToInt((String *)&v5);
      String::~String((String *)&v5);
      result = v4;
    }
    else
    {
      result = atoi(this[1]);
    }
  }
  else
  {
    result = 0;
  }
  return result;
}
// 10A8: using guessed type int dword_10A8;

//----- (000010AC) --------------------------------------------------------
String *__fastcall String::ToStr(String *this, String *a2, int a3, int a4)
{
  String *v4; // r5@1

  v4 = this;
  String::Concat(a2, this, a3, a4);
  return v4;
}

//----- (000010C2) --------------------------------------------------------
int __fastcall String::CopyTo(String *this, int a2, void *a3, int a4)
{
  _BYTE *v4; // r5@1
  int result; // r0@1

  v4 = a3;
  result = Buffer::CopyTo((Buffer *)this, a2, a3, a4);
  if ( result )
    v4[result] = 0;
  else
    result = 0;
  return result;
}

//----- (000010EC) --------------------------------------------------------
int __fastcall String::Equals(String *this, const char *a2)
{
  String *v2; // r4@1
  const char *v3; // r5@1
  int v4; // r6@1
  int result; // r0@2

  v2 = this;
  v3 = a2;
  v4 = strlen(a2);
  if ( v2[2] == (String)v4 )
    result = String::CompareTo(v2, v3, v4, 0) == 0;
  else
    result = 0;
  return result;
}

//----- (0000111A) --------------------------------------------------------
_BOOL4 __fastcall String::Equals(String *this, const String *a2)
{
  return this[2] == a2[2] && !String::CompareTo(this, (const char *)*(_QWORD *)(a2 + 1), *(_QWORD *)(a2 + 1) >> 32, 0);
}

//----- (0000113E) --------------------------------------------------------
int __fastcall String::Search(String *this, const char *s2, int a3, int a4, int a5)
{
  String *v5; // r4@1
  const char *v6; // r8@1
  int result; // r0@2
  char *v8; // r7@6
  char *v9; // r10@8
  char *v10; // r0@9
  const char *v11; // r6@11
  signed int i; // r11@11
  char *s1; // [sp+0h] [bp-38h]@8
  int n; // [sp+Ch] [bp-2Ch]@1

  n = a3;
  v5 = this;
  v6 = s2;
  if ( s2 )
  {
    if ( a4 >= 0 )
    {
      v8 = &this[2][-a3];
      if ( a4 <= (signed int)v8 )
      {
        v9 = &this[1][a4];
        s1 = (char *)(*(_QWORD *)(this + 1) + (*(_QWORD *)(this + 1) >> 32) - 1);
        if ( a5 )
          v10 = (char *)(*(_QWORD *)(this + 1) + (*(_QWORD *)(this + 1) >> 32) - 1);
        else
          v10 = &this[1][a4];
        v11 = v10;
        for ( i = 0; i <= (signed int)v8; ++i )
        {
          if ( !strncmp(v11, v6, n) )
            return v11 - v5[1];
          if ( a5 )
          {
            if ( --v11 < v9 )
              break;
          }
          else if ( ++v11 > s1 )
          {
            break;
          }
        }
        result = -1;
      }
      else
      {
        result = -1;
      }
    }
    else
    {
      result = -1;
    }
  }
  else
  {
    result = -1;
  }
  return result;
}

//----- (000011D8) --------------------------------------------------------
int __fastcall String::IndexOf(String *this, const char *a2, int a3)
{
  String *v3; // r6@1
  const char *v4; // r4@1
  int v5; // r5@1
  int v6; // r0@1

  v3 = this;
  v4 = a2;
  v5 = a3;
  v6 = strlen(a2);
  return String::Search(v3, v4, v6, v5, 0);
}

//----- (000011FA) --------------------------------------------------------
int __fastcall String::IndexOf(String *this, const String *a2, int a3)
{
  return String::Search(this, (const char *)*(_QWORD *)(a2 + 1), *(_QWORD *)(a2 + 1) >> 32, a3, 0);
}

//----- (00001214) --------------------------------------------------------
int __fastcall String::IndexOf(String *this, int a2, int a3)
{
  String *v3; // r3@1
  int result; // r0@2

  v3 = this;
  if ( a3 >= 0 )
  {
    if ( (signed int)this[2] > a3 )
    {
      for ( result = a3; (signed int)v3[2] > result; ++result )
      {
        if ( (unsigned __int8)v3[1][result] == a2 )
          return result;
      }
      result = -1;
    }
    else
    {
      result = -1;
    }
  }
  else
  {
    result = -1;
  }
  return result;
}

//----- (0000124A) --------------------------------------------------------
int __fastcall String::Replace(String *this, int a2, int a3, char a4)
{
  Buffer *v4; // r6@1
  int v5; // r7@1
  char v6; // r8@1
  _BYTE *v7; // r4@1
  int v8; // r9@1
  int result; // r0@5

  v4 = (Buffer *)a2;
  v5 = a3;
  v6 = a4;
  v7 = *(_BYTE **)(String::String((int)this, a2) + 4);
  v8 = 0;
  while ( 1 )
  {
    result = Buffer::Length(v4);
    if ( result <= v8 )
      break;
    if ( *v7 == v5 )
      *v7 = v6;
    ++v8;
    ++v7;
  }
  return result;
}

//----- (0000128C) --------------------------------------------------------
int __fastcall String::ToFloat(String *this)
{
  String *v1; // r4@1
  int result; // r0@2
  int v3; // r3@6
  int v4; // r7@6
  int v5; // [sp+0h] [bp-68h]@6

  v1 = this;
  if ( this[2] )
  {
    if ( *(_BYTE *)(*(_QWORD *)(this + 1) + (*(_QWORD *)(this + 1) >> 32)) )
    {
      String::String((int)&v5, (const char *)&dword_12D8);
      String::copy((int)&v5, (const char *)*(_QWORD *)(v1 + 1), *(_QWORD *)(v1 + 1) >> 32, v3);
      v4 = String::ToFloat((String *)&v5);
      String::~String((String *)&v5);
      result = v4;
    }
    else
    {
      atof(this[1]);
      result = _aeabi_d2f();
    }
  }
  else
  {
    result = 0;
  }
  return result;
}
// 12D8: using guessed type int dword_12D8;
// 171C: using guessed type int _aeabi_d2f(void);

//----- (000012DC) --------------------------------------------------------
int __fastcall String::ToLower(String *this, int a2)
{
  int v2; // r5@1
  int v3; // r6@1
  int i; // r4@1
  int result; // r0@3

  v2 = String::String((int)this, a2);
  v3 = *(_DWORD *)(v2 + 4);
  for ( i = 0; ; ++i )
  {
    result = *(_DWORD *)(v2 + 8);
    if ( result <= i )
      break;
    *(_BYTE *)(v3 + i) = tolower(*(_BYTE *)(v3 + i));
  }
  return result;
}

//----- (00001308) --------------------------------------------------------
int __fastcall String::ToUpper(String *this, int a2)
{
  int v2; // r5@1
  int v3; // r6@1
  int i; // r4@1
  int result; // r0@3

  v2 = String::String((int)this, a2);
  v3 = *(_DWORD *)(v2 + 4);
  for ( i = 0; ; ++i )
  {
    result = *(_DWORD *)(v2 + 8);
    if ( result <= i )
      break;
    *(_BYTE *)(v3 + i) = toupper(*(_BYTE *)(v3 + i));
  }
  return result;
}

//----- (00001334) --------------------------------------------------------
int __fastcall String::TrimEnd(String *this, int a2, int a3, int a4)
{
  String *v4; // r5@1
  int v5; // r0@1
  int v7; // [sp+0h] [bp-18h]@1

  v7 = a4;
  v4 = this;
  v7 = *(_DWORD *)(a2 + 8);
  v5 = trim(*(_DWORD *)(a2 + 4), &v7, 0, 1);
  return String::String((int)v4, v5, v7);
}

//----- (0000135A) --------------------------------------------------------
int __fastcall String::Contains(String *this, const char *a2)
{
  return (String::IndexOf(this, a2, 0) >> 31) + 1;
}

//----- (00001370) --------------------------------------------------------
int __fastcall String::Contains(String *this, const String *a2)
{
  return (String::IndexOf(this, a2, 0) >> 31) + 1;
}

//----- (00001386) --------------------------------------------------------
int __fastcall String::EndsWith(String *this, const char *a2)
{
  String *v2; // r5@1
  const char *v3; // r6@1
  int result; // r0@3
  signed int v5; // r4@5

  v2 = this;
  v3 = a2;
  if ( this[1] && a2 )
  {
    v5 = strlen(a2);
    if ( v5 )
    {
      if ( (signed int)v2[2] >= v5 )
        result = strncmp((const char *)(*(_QWORD *)(v2 + 1) + (*(_QWORD *)(v2 + 1) >> 32) - v5), v3, v5) == 0;
      else
        result = 0;
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (000013C8) --------------------------------------------------------
int __fastcall String::EndsWith(String *this, const String *a2)
{
  int result; // r0@3

  if ( this[1] && *((_DWORD *)a2 + 1) )
  {
    if ( *((_DWORD *)a2 + 2) )
    {
      if ( *((_DWORD *)a2 + 2) <= (signed int)this[2] )
        result = strncmp(
                   &this[1][(_DWORD)&this[2][-*((_DWORD *)a2 + 2)]],
                   (const char *)*(_QWORD *)(a2 + 1),
                   *(_QWORD *)(a2 + 1) >> 32) == 0;
      else
        result = 0;
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0000140A) --------------------------------------------------------
int __fastcall String::GetBytes(int result, unsigned __int8 *a2, int a3, int a4)
{
  int v4; // r4@1
  int v5; // r5@1
  int v6; // r8@6
  Buffer *v7; // r0@8
  unsigned __int8 *v8; // [sp+0h] [bp-28h]@1
  int v9; // [sp+4h] [bp-24h]@1
  int v10; // [sp+8h] [bp-20h]@1

  v8 = a2;
  v9 = a3;
  v10 = a4;
  v4 = result;
  v5 = a4;
  if ( a3 && a2 )
  {
    if ( *(_DWORD *)(result + 8) > a4 )
    {
      v6 = a3;
      if ( *(_DWORD *)(result + 8) - a4 < a3 )
        v6 = *(_DWORD *)(result + 8) - a4;
      v7 = (Buffer *)Buffer::Buffer(&v8);
      result = Buffer::Copy(v7, 0, (const void *)(*(_DWORD *)(v4 + 4) + v5), v6);
    }
    else
    {
      result = 0;
      *a2 = 0;
    }
  }
  return result;
}
// 16F4: using guessed type int __fastcall Buffer::Buffer(_DWORD);

//----- (00001458) --------------------------------------------------------
int __fastcall String::GetBytes(String *this, int a2)
{
  int v2; // r4@1
  int v3; // r5@1
  int v4; // r0@1

  v2 = a2;
  v3 = ByteArray::ByteArray((ByteArray *)this, 0);
  Array::SetLength((Array *)v3, *(_DWORD *)(v2 + 8));
  v4 = Buffer::Length((Buffer *)v3);
  return String::GetBytes(v2, *(unsigned __int8 **)(v3 + 4), v4, 0);
}

//----- (00001490) --------------------------------------------------------
int __fastcall String::ToDouble(String *this)
{
  String *v1; // r4@1
  int result; // r0@2
  int v3; // r3@6
  int v4; // r5@6
  String v5; // [sp+4h] [bp-64h]@6

  v1 = this;
  if ( this[2] )
  {
    if ( *(_BYTE *)(*(_QWORD *)(this + 1) + (*(_QWORD *)(this + 1) >> 32)) )
    {
      String::String((int)&v5, (const char *)&dword_14DC);
      String::copy((int)&v5, (const char *)*(_QWORD *)(v1 + 1), *(_QWORD *)(v1 + 1) >> 32, v3);
      v4 = String::ToDouble(&v5);
      String::~String(&v5);
      result = v4;
    }
    else
    {
      result = COERCE_UNSIGNED_INT64(atof(this[1]));
    }
  }
  else
  {
    result = 0;
  }
  return result;
}
// 14DC: using guessed type int dword_14DC;

//----- (000014E0) --------------------------------------------------------
int __fastcall String::ToString(String *this, int a2)
{
  return String::String((int)this, a2);
}

//----- (000014F2) --------------------------------------------------------
int __fastcall String::CompareTo(String *this, const char *a2, int a3, int a4)
{
  String *v4; // r4@1
  const char *v5; // r5@1
  signed int v6; // r6@1
  int v7; // r7@1
  int result; // r0@6

  v4 = this;
  v5 = a2;
  v6 = a3;
  v7 = a4;
  if ( a3 < 0 )
    v6 = strlen(a2);
  if ( v4[1] )
  {
    if ( v5 || !v4[1] || (signed int)v4[2] <= 0 )
    {
      if ( v7 )
        result = strncasecmp((const char *)*(_QWORD *)(v4 + 1), v5, *(_QWORD *)(v4 + 1) >> 32);
      else
        result = strncmp((const char *)*(_QWORD *)(v4 + 1), v5, *(_QWORD *)(v4 + 1) >> 32);
    }
    else
    {
      result = 1;
    }
  }
  else if ( v5 && v6 > 0 )
  {
    result = -1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0000154A) --------------------------------------------------------
int __fastcall String::CompareTo(String *this, const String *a2)
{
  return String::CompareTo(this, (const char *)*(_QWORD *)(a2 + 1), *(_QWORD *)(a2 + 1) >> 32, 0);
}

//----- (0000155E) --------------------------------------------------------
int __fastcall String::Substring(String *this, int a2, int a3, int a4)
{
  int v4; // r6@1

  v4 = a4;
  if ( a4 < 0 )
    v4 = *(_DWORD *)(a2 + 8) - a3;
  if ( *(_DWORD *)(a2 + 8) - a3 < v4 )
    v4 = *(_DWORD *)(a2 + 8) - a3;
  return String::String((int)this, *(_DWORD *)(a2 + 4) + a3, v4);
}

//----- (00001590) --------------------------------------------------------
int __fastcall String::TrimStart(String *this, int a2, int a3, int a4)
{
  String *v4; // r5@1
  int v5; // r0@1
  int v7; // [sp+0h] [bp-18h]@1

  v7 = a4;
  v4 = this;
  v7 = *(_DWORD *)(a2 + 8);
  v5 = trim(*(_DWORD *)(a2 + 4), &v7, 1, 0);
  return String::String((int)v4, v5, v7);
}

//----- (000015B6) --------------------------------------------------------
int __fastcall String::operator>=(String *this, String *a2)
{
  return (String::CompareTo(this, a2) >> 31) + 1;
}

//----- (000015CA) --------------------------------------------------------
_BOOL4 __fastcall String::operator>(String *this, String *a2)
{
  return String::CompareTo(this, a2) > 0;
}

//----- (000015E4) --------------------------------------------------------
int __fastcall String::operator[](int a1, int a2)
{
  int result; // r0@3

  if ( *(_DWORD *)(a1 + 8) > a2 && *(_DWORD *)(a1 + 4) )
    result = *(_BYTE *)(*(_DWORD *)(a1 + 4) + a2);
  else
    result = 0;
  return result;
}

//----- (000015FA) --------------------------------------------------------
_BOOL4 __fastcall String::operator<=(String *this, String *a2)
{
  return String::CompareTo(this, a2) <= 0;
}

//----- (00001614) --------------------------------------------------------
unsigned int __fastcall String::operator<(String *this, String *a2)
{
  return (unsigned int)String::CompareTo(this, a2) >> 31;
}

//----- (00001668) --------------------------------------------------------
int __fastcall String::init(int result)
{
  *(_DWORD *)(result + 4) = result + 20;
  *(_DWORD *)(result + 16) = 63;
  *(_DWORD *)(result + 8) = 0;
  **(_BYTE **)(result + 4) = 0;
  return result;
}

//----- (0000167C) --------------------------------------------------------
int __fastcall String::~String(String *this)
{
  return Array::~Array((Array *)this);
}

//----- (0000168A) --------------------------------------------------------
int __fastcall isspace(int a1)
{
  return (*_rt_ctype_table())[a1] & 1;
}

//----- (0000169C) --------------------------------------------------------
int __fastcall Buffer::Length(Buffer *this)
{
  return *((_DWORD *)this + 2);
}

//----- (000016A2) --------------------------------------------------------
void __fastcall String::~String(String *this)
{
  void *v1; // r0@1

  v1 = (void *)String::~String(this);
  operator delete(v1);
}
#endif
static byte buftmp[10];

String::String(cstring cstr): Array(cstr, ArrayLength(cstr))
{
    this->_Length = ArrayLength(cstr);
    this->Copy(0, cstr, this->_Length);
}

String::String(const String &str): Array(str.GetBuffer(), str.Length()){

}
String::String(String && rval): Array(rval.GetBuffer(), rval.Length()){

}
String::String(char *str, int length): Array(str, length){

}
String::String(char *str, int length, bool expand): Array(str, length)
{
    this->Expand = expand;
}

// 包装静态字符串，直接使用，修改时扩容
String::String(cstring str, int length): Array(str, length){}
String::String(bool value): Array(value ? "true" : "false", value ? 5 : 6){

}
String::String(char c): Array(buftmp, 10){}
String::String(byte value, int radix): Array(buftmp, 10){}
String::String(short value, int radix): Array(buftmp, 10){}
String::String(ushort value, int radix): Array(buftmp, 10){}
String::String(int value, int radix): Array(buftmp, 10){}
String::String(uint value, int radix): Array(buftmp, 10){}
String::String(Int64 value, int radix): Array(buftmp, 10){}
String::String(UInt64 value, int radix): Array(buftmp, 10){}
String::String(float value, int decimalPlaces): Array(buftmp, 10){}
String::String(double value, int decimalPlaces): Array(buftmp, 10){}

// 设置数组长度。改变长度后，确保最后以0结尾
bool String::SetLength(int length, bool bak)
{
	return false;
}
// 拷贝数据，默认-1长度表示当前长度
int String::Copy(int destIndex, const void *src, int len)
{
    return 0;
}
	
// 把数据复制到目标缓冲区，默认-1长度表示当前长度
int String::CopyTo(int srcIndex, void *dest, int len)const
{
    return 0;
}

// 为被赋值对象建立一个备份。
// 如果值为空或无效，或者内存分配失败，字符串将会被标记为无效
String &String::operator = (const String &rhs)
{
    return  *this;
}

String &String::operator = (cstring cstr)
{
    return  *this;
}

String &String::operator = (String && rval)
{
    return  *this;
}

// 连接内建类型。如果参数无效则认为连接失败
bool String::Concat(const Object &obj)
{
    return false;
}

bool String::Concat(const String &str)
{
    return false;
}

bool String::Concat(cstring cstr)
{
    return false;
}

bool String::Concat(bool value)
{
    return false;
}

bool String::Concat(char c)
{
    return false;
}

bool String::Concat(byte c, int radix)
{
    return false;
}

bool String::Concat(short num, int radix)
{
    return false;
}

bool String::Concat(ushort num, int radix)
{
    return false;
}

bool String::Concat(int num, int radix)
{
    return false;
}

bool String::Concat(uint num, int radix)
{
    return false;
}

bool String::Concat(Int64 num, int radix)
{
    return false;
}

bool String::Concat(UInt64 num, int radix)
{
    return false;
}

bool String::Concat(float num, int decimalPlaces)
{
    return false;
}

bool String::Concat(double num, int decimalPlaces)
{
    return false;
}

int String::CompareTo(const String &s)const
{
    return 0;
}

int String::CompareTo(cstring cstr, int len, bool ignoreCase)const
{
    return 0;
}

bool String::Equals(const String &s)const
{
    return false;
}

bool String::Equals(cstring cstr)const
{
    return false;
}

bool String::EqualsIgnoreCase(const String &s)const
{
    return false;
}

bool String::EqualsIgnoreCase(cstring cstr)const
{
    return false;
}

bool String::operator < (const String &rhs)const
{
    return false;
}

bool String::operator > (const String &rhs)const
{
    return false;
}

bool String::operator <= (const String &rhs)const
{
    return false;
}

bool String::operator >= (const String &rhs)const
{
    return false;
}

char String::operator[](int index)const
{
    return '0';
}

char &String::operator[](int index)
{
    char ret = '0';
    return ret;
}

void String::GetBytes(byte *buf, int bufsize, int index)const{}
ByteArray String::GetBytes()const{}
ByteArray String::ToHex()const{}
int String::ToInt()const
{
    return 0;
}

float String::ToFloat()const
{
    return 0;
}

double String::ToDouble()const
{
    return 0;
}

// 输出对象的字符串表示方式
String &String::ToStr(String &str)const
{
    return str;
}

// 输出对象的字符串表示方式
String String::ToString()const
{
    return  *this;
}

// 调试输出字符串
void String::Show(bool newLine)const{}

// 格式化字符串，输出到现有字符串后面。方便我们连续格式化多个字符串
String &String::Format(cstring format, ...)
{
    return  *this;
}

int String::IndexOf(const char ch, int startIndex)const
{
    return 0;
}

int String::IndexOf(const String &str, int startIndex)const
{
    return 0;
}

int String::IndexOf(cstring str, int startIndex)const
{
    return 0;
}

int String::LastIndexOf(const char ch, int startIndex)const
{
    return 0;
}

int String::LastIndexOf(const String &str, int startIndex)const
{
    return 0;
}

int String::LastIndexOf(cstring str, int startIndex)const
{
    return 0;
}

bool String::Contains(const String &str)const
{
    return false;
}

bool String::Contains(cstring str)const
{
    return false;
}

bool String::StartsWith(const String &str, int startIndex)const
{
    return false;
}

bool String::StartsWith(cstring str, int startIndex)const
{
    return false;
}

bool String::EndsWith(const String &str)const
{
    return false;
}

bool String::EndsWith(cstring str)const
{
    return false;
}

StringSplit String::Split(const String &sep)const{}
StringSplit String::Split(cstring sep)const{}

String String::Substring(int start, int len)const
{
    return  *this;
}

String String::TrimStart()const
{
    return  *this;
}

String String::TrimEnd()const
{
    return  *this;
}

String String::Trim()const
{
    return  *this;
}

String String::Replace(char find, char replace)const
{
    return  *this;
}

String String::Replace(const String &find, const String &replace)const
{
    return  *this;
}

String String::Remove(int index)const
{
    return  *this;
}

String String::Remove(int index, int count)const
{
    return  *this;
}

String String::ToLower()const
{
    return  *this;
}

String String::ToUpper()const
{
    return  *this;
}

// 静态比较器。比较两个字符串指针
int String::Compare(const void *v1, const void *v2)
{
    return 0;
}

#if DEBUG
    void String::Test(){}
#endif 
void String::init(){}
void String::release(){}
bool String::Concat(cstring cstr, int length)
{
    return false;
}

String &String::copy(cstring cstr, int length)
{
    return  *this;
}

void String::move(String &rhs){}
bool String::CopyOrWrite()
{
    return false;
}


void *String::Alloc(int len)
{
    return this;
}

int String::Search(cstring str, int len, int startIndex, bool rev)const
{
    return 0;
}

StringSplit::StringSplit(const String &str, cstring sep): _Str(str){

}

const String StringSplit::Next(){}





















//String::String(char ch)
//{
////    this->initCapacity();
//    ((byte*)(this->_Arr))[0] = ch;
////    this->jsLength();
//}

//进制字符串
//String::String(byte value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
//            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
//            break;
//        case 16:
//            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
// //   this->jsLength();
//}

//进制字符串
//String::String(short value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
//            break;
//        case 16:
//    //        snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
////    this->jsLength();
//}

//进制字符串
//String::String(int value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
//            break;
//        case 16:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
// //   this->jsLength();
//}

//进制字符串
//String::String(uint value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%u", value);
//            break;
//        case 16:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
//            break;
//    }
////    this->jsLength();
//}

//进制字符串
//String::String(Int64 value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%lld", value);
//            break;
//        case 16:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llX", value);
//            break;
//    }
////    this->jsLength();
//}

//进制字符串
//String::String(UInt64 value, int radix)
//{
////    this->initCapacity();

//    switch (radix)
//    {
//        case 10:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llu", value);
//            break;
//        case 16:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llX", value);
//            break;
//    }
////    this->jsLength();
//}

//浮点数
//String::String(float value, int dot)
//{
////    this->initCapacity();
//    switch (dot)
//    {
//        case 0:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.0f", value);
//            break;
//        case 1:
//   //         snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.1f", value);
//            break;
//        case 2:
//  //          snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.2f", value);
//            break;
//        case 3:
// //           snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.3f", value);
//            break;
//        case 4:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.4f", value);
//            break;
//        case 5:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.5f", value);
//            break;
//        default:
//            break;
//    }
////    this->jsLength();
//}

//浮点数
//String::String(double value, int dot)
//{
////    this->initCapacity();
//    switch (dot)
//    {
//        case 0:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.0lf", value);
//            break;
//        case 1:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.1lf", value);
//            break;
//        case 2:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.2lf", value);
//            break;
//        case 3:
// //           snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.3lf", value);
//            break;
//        case 4:
// //           snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.4lf", value);
//            break;
//        case 5:
////            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.5lf", value);
//            break;
//        default:
//            break;
//    }
////    this->jsLength();
//}

//String::String(const String &str1)
//{
//    #if 1
//  //      this->initCapacity();
//    #endif 
//    #if 1
//        for (int i = 0; i < str1.Length(); i++)
//        {
//            ((byte*)(this->_Arr))[i] = str1.GetBuffer()[i];
//        }
//  //      this->jsLength();
//    #endif 
//}
#if 0
    String &String::operator = (const char *str)
    {
        //    if (strlen(str) <= this->mcapacity)
        {
            //        this->_Length = strlen(str);
        }
        //   else
        {
            //        this->_Length = this->mcapacity;
        }
        for (int i = 0; i < this->_Length; i++)
        {
            ((byte*)(this->_Arr))[i] = str[i];
        }
        ((byte*)(this->_Arr))[this->_Length] = 0;
        return  *this;
    }
#endif 
#if 0
    String &String::operator = (const String &str)
    {
        //    if (str.Length() <= this->mcapacity)
        {
            //        this->_Length = str.Length();
        }
        //   else
        {
            //        this->_Length = this->mcapacity;
        }
        for (int i = 0; i < this->_Length; i++)
        {
            ((byte*)(this->_Arr))[i] = str.GetBuffer()[i];
        }
        ((byte*)(this->_Arr))[this->_Length] = 0;
        return  *this;
    }
#endif 
#if 0
    bool String::operator == (const char *str)
    {
        if (strlen(str) != this->_Length)
        {
            return false;
        }
        for (int i = 0; i < this->_Length; i++)
        {
            if (((byte*)(this->_Arr))[i] != str[i])
            {
                return false;
            }
        }
        return true;
    }
#endif 
#if 0	
    int String::Copy(int destIndex, const void *src, int len)
    {
        return len;
    }
#endif 
#if 0
    bool String::Equals(const String &str1)const
    {
        if (this->Length() != str1._Length)
        {
            return false;
        }
        for (int i = 0; i < this->_Length; i++)
        {
            if (((byte*)(this->_Arr))[i] != ((byte*)str1._Arr)[i])
            {
                return false;
            }
        }
        return true;
    }
#endif 
#if 0
    bool String::Equals(cstring cstr)const
    {
        return true;
    }
#endif 
#if 0
    String &String::operator += (const DateTime &dt)
    {
        if ((this->_Length + 19) < this->Capacity())
        {
            this->_Length += 19;
            snprintf((char*) &((byte*)(this->_Arr))[this->_Length - 1-18], 20, "%04d-%02d-%02d %02d:%02d:%02d", dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);
        }
        return  *this;
    }
#endif 
#if 0
    String &String::operator += (const String &str)
    {
        if ((str.Length() + this->_Length) < this->mcapacity)
        {
            this->_Length += str.Length();
            this->Copy(this->_Length - str.Length(), str.GetBuffer(), str.Length());
        }

        return  *this;
    }
#endif 
#if 0
    String &String::operator += (const float da)
    {
        byte buf[100] = 
        {
            0
        };
        snprintf((char*)buf, 100, "%2.2f", da);
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(byte value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%d", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(short value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%d", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(int value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%d", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(uint value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%u", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%X", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(Int64 value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%lld", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%llX", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(UInt64 value, byte radix)
    {
        byte buf[100] = 
        {
            0
        };
        switch (radix)
        {
            case 10:
                snprintf((char*)buf, 100, "%llu", value);
                break;
            case 16:
                snprintf((char*)buf, 100, "%llX", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    String &String::Concat(float value, byte dot)
    {
        byte buf[100] = 
        {
            0
        };
        switch (dot)
        {
            case 0:
                snprintf((char*)buf, 100, "%0.0f", value);
                break;
            case 1:
                snprintf((char*)buf, 100, "%1.1f", value);
                break;
            case 2:
                snprintf((char*)buf, 100, "%2.2f", value);
                break;
            case 3:
                snprintf((char*)buf, 100, "%3.3f", value);
                break;
            case 4:
                snprintf((char*)buf, 100, "%4.4f", value);
                break;
            default:
                break;
        }

        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 

#if 0

    String &String::Concat(double value, byte dot)
    {
        byte buf[100] = 
        {
            0
        };
        switch (dot)
        {
            case 0:
                snprintf((char*)buf, 100, "%0.0f", value);
                break;
            case 1:
                snprintf((char*)buf, 100, "%1.1f", value);
                break;
            case 2:
                snprintf((char*)buf, 100, "%2.2f", value);
                break;
            case 3:
                snprintf((char*)buf, 100, "%3.3f", value);
                break;
            case 4:
                snprintf((char*)buf, 100, "%4.4f", value);
                break;
            default:
                break;
        }
        if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
        {
            this->Copy(this->_Length, buf, strlen((char*)buf));
        }
        this->_Length += strlen((char*)buf);
        return  *this;
    }
#endif 
#if 0
    byte ToInt(char ch)
    {
        ch = toupper(ch);
        if (ch <= '9')
        {
            return ch - '0';
        }
        return ch - 'A' + 10;
    }
#endif 
#if 0
    String String::ToHex()
    {
        byte buf[100];
        byte bi = 0;
        byte da;
        int j = 0;
        for (int i = 0; i < 100; i++)
        {
            buf[i] = 0;
        }
        for (int i = 0; i < this->_Length; i++)
        {

            if (isxdigit(((byte*)(this->_Arr))[i]))
            {
                if (bi == 0)
                {
                    da = ToInt(((byte*)(this->_Arr))[i]);
                    da <<= 4;
                    bi = 1;
                }
                else
                {
                    da |= ToInt(((byte*)(this->_Arr))[i]);
                    buf[j] = da;
                    j++;
                    bi = 0;
                }
            }
            else
            {
                bi = 0;
            }
        }

        return String((const char*)buf);
    }
#endif 
#if 0
    //数组容量
    uint String::Capacity()const
    {
        return this->mcapacity;
    }
#endif 
#if 0
    void String::Show(bool newLine)const
    {
        printf("%s", ((byte*)(this->_Arr)));
        newLine ? printf("\n"): 0;
    }
#endif 
//初始化容器
//void String::initCapacity()
//{
//    this->_Arr = new char[0x40];
////    this->mcapacity = 0x40 - 1;
//    memset(((byte*)(this->_Arr)), 0x40, 0);
//}

//计算字符串长度
//void String::jsLength()
//{
//    this->_Length = strlen((char*)((byte*)(this->_Arr)));
//}

#ifdef DEBUG
    #if 0
        void String::Test()
        {
            char err[] = "String Err\r\n";
            //默认空字符串，使用内部数据区
            String str;
            assert(str.Length() == 0, err);
            assert(str.Capacity() == 0x40 - 1, err);

            String str1("456");
            assert(str1 == "456", err);
            assert(str1.GetBuffer() == "456", err);

            //		char err1[] = "String(const String& str)";


        }
    #endif 
#endif
