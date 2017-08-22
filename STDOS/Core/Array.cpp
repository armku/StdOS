#include "Array.h"

#include "Array.h"


#if 0
int dword_1F0[5] = { 3867256551, 1816371885, 3832701285, 2498145983, 185 }; // weak
int dword_204[8] =
{
  1634036803,
  2962613874,
  3836644838,
  2213055928,
  3132679357,
  3870992871,
  2464778124,
  136
}; // weak
_UNKNOWN loc_300; // weak
_DWORD (__cdecl *off_658)(Buffer *__hidden this, String *) = &Buffer::ToStr; // weak


//----- (00000000) --------------------------------------------------------
signed int __fastcall Array::CheckCapacity(Array *this, int a2, int a3, int a4)
{
  Array *v4; // r4@1
  int v5; // r7@1
  signed int result; // r0@4
  int i; // r5@8
  int v8; // r9@11
  int v9; // r6@11
  Buffer *v10; // r0@17
  int v11; // r10@18
  int v12; // [sp+0h] [bp-30h]@1
  int v13; // [sp+4h] [bp-2Ch]@1
  int v14; // [sp+8h] [bp-28h]@1

  v12 = a2;
  v13 = a3;
  v14 = a4;
  v4 = this;
  v5 = a3;
  if ( *((_DWORD *)this + 1) && *((_DWORD *)this + 4) >= a2 && *((_BYTE *)this + 14) )
  {
    result = 1;
  }
  else if ( *((_BYTE *)this + 12) )
  {
    for ( i = 64; i < a2; i *= 2 )
      ;
    v8 = *((_BYTE *)this + 13);
    v9 = (*(int (__fastcall **)(Array *, int))(*(_DWORD *)this + 48))(this, i);
    if ( v9 )
    {
      if ( *((_DWORD *)v4 + 2) < v5 )
        v5 = *((_DWORD *)v4 + 2);
      if ( v5 > 0 && *((_DWORD *)v4 + 1) )
      {
        v10 = (Buffer *)Buffer::Buffer(&v12, v9, i);
        Buffer::Copy(v10, 0, *((const void **)v4 + 1), v5);
      }
      v11 = *((_DWORD *)v4 + 2);
      if ( v8 )
      {
        if ( *((_DWORD *)v4 + 1) != v9 )
          operator delete(*((void **)v4 + 1));
      }
      *((_DWORD *)v4 + 1) = v9;
      *((_DWORD *)v4 + 4) = i;
      *((_DWORD *)v4 + 2) = v11;
      result = 1;
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
// 6C0: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);

//----- (00000090) --------------------------------------------------------
signed int __fastcall Array::Set(Array *this, const void *a2, int a3)
{
  Array *v3; // r4@1
  const void *v4; // r5@1
  int v5; // r6@1

  v3 = this;
  v4 = a2;
  v5 = a3;
  if ( *((_BYTE *)this + 13) && *((const void **)this + 1) != a2 )
    (*(void (__fastcall **)(Array *))(*(_DWORD *)this + 52))(this);
  *((_DWORD *)v3 + 1) = v4;
  *((_DWORD *)v3 + 2) = v5;
  *((_DWORD *)v3 + 4) = v5;
  *((_BYTE *)v3 + 13) = 0;
  *((_BYTE *)v3 + 14) = 0;
  return 1;
}

//----- (000000BA) --------------------------------------------------------
signed int __fastcall Array::Set(Array *this, void *a2, int a3)
{
  Array *v3; // r4@1
  signed int result; // r0@2

  v3 = this;
  if ( Array::Set(this, a2, a3) )
  {
    result = 1;
    *((_BYTE *)v3 + 14) = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (000000DA) --------------------------------------------------------
int __fastcall Array::Copy(Array *this, int a2, const Buffer *a3, int a4, int a5)
{
  Array *v5; // r7@1
  int v6; // r6@1
  const Buffer *v7; // r8@1
  int v8; // r9@1
  int v9; // r4@1
  int v10; // r0@1
  int v11; // r5@1

  v5 = this;
  v6 = a2;
  v7 = a3;
  v8 = a4;
  v9 = a5;
  v10 = Buffer::Length(a3);
  v11 = v10 - v8;
  if ( a5 >= 0 )
  {
    if ( a5 > v11 )
      v9 = v10 - v8;
  }
  else
  {
    v9 = v10 - v8;
    if ( v11 <= 0 )
      return 0;
  }
  if ( Buffer::Length(v5) < v6 + v9 )
    (*(void (__fastcall **)(Array *, int, signed int))(*(_DWORD *)v5 + 36))(v5, v6 + v9, 1);
  return Buffer::Copy(v5, v6, v7, v8, v9);
}

//----- (00000130) --------------------------------------------------------
int __fastcall Array::Init(int result)
{
  *(_BYTE *)(result + 12) = 1;
  *(_BYTE *)(result + 15) = 1;
  *(_DWORD *)(result + 16) = *(_DWORD *)(result + 8);
  *(_BYTE *)(result + 13) = 0;
  *(_BYTE *)(result + 14) = 1;
  return result;
}

//----- (00000144) --------------------------------------------------------
int __fastcall Array::move(Array *this, Array *a2)
{
  Array *v2; // r5@1
  Array *v3; // r4@1
  int result; // r0@4

  v2 = this;
  v3 = a2;
  if ( *((_BYTE *)this + 13) && *((_DWORD *)this + 1) != *((_DWORD *)a2 + 1) )
    (*(void (__fastcall **)(Array *))(*(_DWORD *)this + 52))(this);
  Buffer::move(v2, v3);
  *((_DWORD *)v2 + 4) = *((_DWORD *)v3 + 4);
  *((_BYTE *)v2 + 13) = *((_BYTE *)v3 + 13);
  *((_BYTE *)v2 + 14) = *((_BYTE *)v3 + 14);
  *((_BYTE *)v2 + 12) = *((_BYTE *)v3 + 12);
  result = 0;
  *((_DWORD *)v3 + 4) = 0;
  *((_BYTE *)v3 + 13) = 0;
  *((_BYTE *)v3 + 14) = 0;
  return result;
}

//----- (00000180) --------------------------------------------------------
int __fastcall Array::Alloc(Array *this, int a2)
{
  *((_BYTE *)this + 13) = 1;
  return operator new[](*((_BYTE *)this + 15) * a2);
}

//----- (00000198) --------------------------------------------------------
int __fastcall Array::Clear(Array *this)
{
  Array *v1; // r4@1

  v1 = this;
  if ( !*((_BYTE *)this + 14) )
    assert_failed2((const char *)dword_1F0, "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0xECu);
  if ( !*((_DWORD *)v1 + 1) )
    assert_failed2((const char *)dword_204, "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0xEDu);
  return Buffer::Set(v1, 0, 0, *((_BYTE *)v1 + 15) * *((_DWORD *)v1 + 2));
}
// 1F0: using guessed type int dword_1F0[5];
// 204: using guessed type int dword_204[8];

//----- (00000224) --------------------------------------------------------
signed int __fastcall Array::Release(Array *this)
{
  void *v1; // r5@1
  int v2; // r6@1
  signed int result; // r0@3

  v1 = (void *)*((_DWORD *)this + 1);
  v2 = *((_BYTE *)this + 13);
  *((_DWORD *)this + 1) = 0;
  *((_DWORD *)this + 4) = 0;
  *((_DWORD *)this + 2) = 0;
  *((_BYTE *)this + 13) = 0;
  *((_BYTE *)this + 14) = 1;
  if ( v2 && v1 )
  {
    operator delete[](v1);
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

//----- (0000024C) --------------------------------------------------------
signed int __fastcall Array::SetItem(Array *this, const void *a2, int a3, int a4)
{
  Array *v4; // r4@1
  unsigned __int8 *v5; // r7@1
  int v6; // r5@1
  int v7; // r6@1
  bool v8; // zf@13
  bool v9; // nf@13

  v4 = this;
  v5 = (unsigned __int8 *)a2;
  v6 = a3;
  v7 = a4;
  if ( !*((_BYTE *)this + 14) )
    assert_failed2((const char *)&loc_300, "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0xAFu);
  if ( !v5 )
    assert_failed2("Array::SetItem data Error", "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0xB0u);
  if ( v7 <= 0 )
    v7 = *((_DWORD *)v4 + 2) - v6;
  if ( v7 <= 0 )
    assert_failed2("Array::SetItem count Error", "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0xB4u);
  Array::CheckCapacity(v4, v6 + v7, v6, a4);
  if ( *((_DWORD *)v4 + 2) < v6 + v7 )
    *((_DWORD *)v4 + 2) = v6 + v7;
  if ( *((_BYTE *)v4 + 15) == 1 )
  {
    Buffer::Set(v4, *v5, v6, v7);
  }
  else
  {
    while ( 1 )
    {
      v8 = v7 == 0;
      v9 = v7-- < 0;
      if ( v9 || v8 )
        break;
      (*(void (__fastcall **)(_DWORD, _DWORD, _DWORD, _DWORD))(*(_DWORD *)v4 + 16))(v4, v6, v5, *((_BYTE *)v4 + 15));
      v6 += *((_BYTE *)v4 + 15);
    }
  }
  return 1;
}

//----- (0000034C) --------------------------------------------------------
int __fastcall Array::SetItemAt(Array *this, int a2, const void *a3, int a4)
{
  Array *v4; // r4@1
  int v5; // r5@1
  const void *v6; // r6@1

  v4 = this;
  v5 = a2;
  v6 = a3;
  if ( !*((_BYTE *)this + 14) )
    assert_failed2((const char *)sub_3B0, "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0xF6u);
  Array::CheckCapacity(v4, v5 + 1, *((_DWORD *)v4 + 2), a4);
  if ( *((_DWORD *)v4 + 2) <= v5 )
    *((_DWORD *)v4 + 2) = v5 + 1;
  return (*(int (__fastcall **)(Array *, int, const void *))(*(_DWORD *)v4 + 16))(v4, *((_BYTE *)v4 + 15) * v5, v6);
}
// 3B0: using guessed type int sub_3B0();

//----- (000003C8) --------------------------------------------------------
int __fastcall Array::SetLength(Array *this, int a2)
{
  return (*(int (**)(void))(*(_DWORD *)this + 36))();
}

//----- (000003DC) --------------------------------------------------------
signed int __fastcall Array::SetLength(Array *this, int a2, int a3, int a4)
{
  Array *v4; // r4@1
  int v5; // r5@1
  int v6; // r0@4

  v4 = this;
  v5 = a2;
  if ( *((_DWORD *)this + 4) < a2 )
  {
    if ( a3 )
      v6 = *((_DWORD *)this + 2);
    else
      v6 = 0;
    if ( !Array::CheckCapacity(v4, a2, v6, a4) )
      return 0;
    if ( *((_DWORD *)v4 + 2) < v5 )
      *((_DWORD *)v4 + 2) = v5;
  }
  else
  {
    *((_DWORD *)this + 2) = a2;
  }
  return 1;
}

//----- (00000414) --------------------------------------------------------
Array *__fastcall Array::Array(int a1, Array *a2)
{
  Array *v2; // r5@1
  Array *v3; // r0@1
  Array *v4; // r4@1

  v2 = a2;
  v3 = (Array *)Buffer::Buffer(a1, 0, 0);
  v4 = v3;
  *(_DWORD *)v3 = &off_658;
  Array::move(v3, v2);
  return v4;
}
// 658: using guessed type _DWORD (__cdecl *)(Buffer *__hidden this, String *);
// 6C0: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);

//----- (0000043C) --------------------------------------------------------
_DWORD *__fastcall Array::Array(int a1, int a2, int a3)
{
  _DWORD *v3; // r0@1
  _DWORD *v4; // r4@1

  v3 = (_DWORD *)Buffer::Buffer(a1, a2, a3);
  v4 = v3;
  *v3 = &off_658;
  Array::Init((int)v3);
  *((_BYTE *)v4 + 14) = 0;
  return v4;
}
// 658: using guessed type _DWORD (__cdecl *off_658)(Buffer *__hidden this, String *);
// 6C0: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);

//----- (00000468) --------------------------------------------------------
_DWORD *__fastcall Array::Array(int a1, int a2, int a3)
{
  _DWORD *v3; // r0@1
  _DWORD *v4; // r4@1

  v3 = (_DWORD *)Buffer::Buffer(a1, a2, a3);
  v4 = v3;
  *v3 = &off_658;
  Array::Init((int)v3);
  return v4;
}
// 658: using guessed type _DWORD (__cdecl *off_658)(Buffer *__hidden this, String *);
// 6C0: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);

//----- (00000490) --------------------------------------------------------
int __fastcall Array::Array(int a1, const Buffer *a2)
{
  const Buffer *v2; // r5@1
  Array *v3; // r0@1
  int v4; // r4@1

  v2 = a2;
  v3 = (Array *)Buffer::Buffer(a1, 0, 0);
  v4 = (int)v3;
  *(_DWORD *)v3 = &off_658;
  Array::Copy(v3, 0, v2, 0, -1);
  Array::Init(v4);
  return v4;
}
// 658: using guessed type _DWORD (__cdecl *off_658)(Buffer *__hidden this, String *);
// 6C0: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);

//----- (000004C8) --------------------------------------------------------
void __fastcall Array::~Array(Array *this)
{
  Array *v1; // r0@1

  v1 = Array::~Array(this);
  operator delete((void *)v1);
}

//----- (000004DC) --------------------------------------------------------
Array *__fastcall Array::~Array(Array *this)
{
  Array *v1; // r4@1

  v1 = this;
  *(_DWORD *)this = &off_658;
  Array::Release(this);
  return v1;
}
// 658: using guessed type _DWORD (__cdecl *off_658)(Buffer *__hidden this, String *);

//----- (000004F4) --------------------------------------------------------
Array *__fastcall Array::operator=(Array *this, Array *a2)
{
  Array *v2; // r4@1

  v2 = this;
  Array::move(this, a2);
  return v2;
}

//----- (00000506) --------------------------------------------------------
int __fastcall Array::operator=(int a1)
{
  int v1; // r4@1

  v1 = a1;
  Buffer::operator=();
  return v1;
}
// 6C4: using guessed type int Buffer::operator=(void);

//----- (00000518) --------------------------------------------------------
int __fastcall Array::operator=(int a1)
{
  int v1; // r4@1

  v1 = a1;
  Buffer::operator=();
  return v1;
}
// 6C8: using guessed type int Buffer::operator=(void);

//----- (0000052C) --------------------------------------------------------
int __fastcall Array::operator[](int a1, int a2)
{
  int v2; // r4@1
  int v3; // r5@1

  v2 = a1;
  v3 = a2;
  if ( !*(_DWORD *)(a1 + 4) || a2 < 0 || *(_DWORD *)(a1 + 8) <= a2 )
    assert_failed2((const char *)sub_580, "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0x10Eu);
  if ( (signed int)*(_BYTE *)(v2 + 15) > 1 )
    v3 *= *(_BYTE *)(v2 + 15);
  return *(_DWORD *)(v2 + 4) + v3;
}
// 580: using guessed type int sub_580();

//----- (00000590) --------------------------------------------------------
int __fastcall Array::operator[](int a1, int a2)
{
  int v2; // r4@1
  int v3; // r5@1

  v2 = a1;
  v3 = a2;
  if ( !*(_DWORD *)(a1 + 4) || a2 < 0 || *(_DWORD *)(a1 + 8) <= a2 )
    assert_failed2((const char *)sub_5E4, "E:\\Smart\\SmartOS\\Core\\Array.cpp", 0x104u);
  if ( (signed int)*(_BYTE *)(v2 + 15) > 1 )
    v3 *= *(_BYTE *)(v2 + 15);
  return *(_BYTE *)(*(_DWORD *)(v2 + 4) + v3);
}
// 5E4: using guessed type int sub_5E4();

//----- (000005F4) --------------------------------------------------------
int __fastcall operator==(Buffer *this, const Buffer *a2)
{
  Buffer *v2; // r5@1
  const Buffer *v3; // r4@1
  int v4; // r6@1
  int result; // r0@2

  v2 = this;
  v3 = a2;
  v4 = Buffer::Length(this);
  if ( v4 == Buffer::Length(v3) )
    result = Buffer::CompareTo(v2, v3) == 0;
  else
    result = 0;
  return result;
}

//----- (00000622) --------------------------------------------------------
int __fastcall operator!=(Buffer *this, const Buffer *a2)
{
  Buffer *v2; // r5@1
  const Buffer *v3; // r4@1
  int v4; // r6@1
  int result; // r0@2

  v2 = this;
  v3 = a2;
  v4 = Buffer::Length(this);
  if ( v4 == Buffer::Length(v3) )
    result = Buffer::CompareTo(v2, v3) != 0;
  else
    result = 1;
  return result;
}

//----- (00000690) --------------------------------------------------------
int __fastcall Buffer::Length(Buffer *this)
{
  return *((_DWORD *)this + 2);
}
#endif

#if 0
Array::Array(void* data, int len)
{
	
}
Array::Array(const void* data, int len)
{
	
}
Array::Array(Array&& rval)
{
	
}
Array::Array(const Buffer& rhs)
{
	
}

Array::~Array()
{
	
}

Array& Array::operator = (const Buffer& rhs)
{
	
}
Array& Array::operator = (const void* p)
{
	
}
Array& Array::operator = (Array&& rval)
{
	
}

// 设置数组长度。容量足够则缩小Length，否则扩容以确保数组容量足够大避免多次分配内存
bool Array::SetLength(int len)
{
	
}
bool Array::SetLength(int len, bool bak)
{
	
}

//void Array::SetBuffer(void* ptr, int len)
//{
	
//}
//void Array::SetBuffer(const void* ptr, int len)
//{
	
//}
// 拷贝数据，默认-1长度表示使用右边最大长度，左边不足时自动扩容

int Array::Copy(int destIndex, const Buffer& src, int srcIndex, int len)
{
	
}

// 设置数组元素为指定值，自动扩容
bool Array::SetItem(const void* data, int index, int count)
{
	
}
// 设置数组。直接使用指针，不拷贝数据
bool Array::Set(void* data, int len)
{
	
}
// 设置数组。直接使用指针，不拷贝数据
bool Array::Set(const void* data, int len)
{
	
}

// 清空已存储数据。
void Array::Clear()
{
	
}
// 设置指定位置的值，不足时自动扩容
void Array::SetItemAt(int i, const void* item)
{
	
}

// 重载索引运算符[]，返回指定元素的第一个字节
byte Array::operator[](int i) const
{
	
}
byte& Array::operator[](int i)
{
	
}

bool operator==(const Array& bs1, const Array& bs2)
{
	
}
bool operator!=(const Array& bs1, const Array& bs2)
{
	
}

#if DEBUG
	void Array::Test()
{
	
}
#endif

void Array::Init()
{
	
}
void Array::move(Array& rval)
{
	
}

// 检查容量。如果不足则扩大，并备份指定长度的数据
bool Array::CheckCapacity(int len, int bak)
{
	
}
void* Array::Alloc(int len)
{
	
}
// 释放已占用内存
bool Array::Release()
{
	
}
#endif























Array::Array(void* data, int len):Buffer(data,len)
{
	
}
Array::Array(const void* data, int len):Buffer((void*)data ,len)
{
	
}
bool Array::Release()
{
	return true;
}
void* Array::Alloc(int len)
{
	return 0;
}
void Array::Clear()
{
}
void Array::SetItemAt(int i, const void* item)
{
	
}
int Array::Copy(int destIndex, const Buffer& src, int srcIndex, int len)
{
	return true;
}
bool Array::SetLength(int len, bool bak)
{
	return true;
}
bool Array::SetLength(int len)
{
	return true;
}
Array::~Array()
{
}
