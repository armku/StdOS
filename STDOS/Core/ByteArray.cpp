#include "ByteArray.h"
#if 0
_DWORD (__cdecl *off_2A0)(Buffer *__hidden this, String *) = &Buffer::ToStr; // weak


//----- (00000000) --------------------------------------------------------
int __fastcall ByteArray::Load(ByteArray *this, const void *a2, int a3)
{
  ByteArray *v3; // r4@1
  int v4; // r0@2

  v3 = this;
  if ( *(_BYTE *)a2 > a3 )
    v4 = a3;
  else
    v4 = *(_BYTE *)a2;
  *((_DWORD *)v3 + 2) = v4;
  return (*(int (__fastcall **)(_DWORD, _DWORD, _DWORD, _DWORD))(*(_DWORD *)v3 + 16))(
           v3,
           0,
           (char *)a2 + 1,
           *((_DWORD *)v3 + 2));
}

//----- (0000002E) --------------------------------------------------------
int __fastcall ByteArray::move(ByteArray *this, ByteArray *a2)
{
  ByteArray *v2; // r5@1
  ByteArray *v3; // r4@1
  int result; // r0@3

  v2 = this;
  v3 = a2;
  if ( *((ByteArray **)a2 + 1) != (ByteArray *)((char *)a2 + 20) && *((_BYTE *)a2 + 13) )
  {
    result = Array::move(this, a2);
  }
  else
  {
    (*(void (__fastcall **)(_DWORD, _DWORD))(*(_DWORD *)this + 12))(this, *((_DWORD *)a2 + 2));
    result = (*(int (__fastcall **)(_DWORD, _DWORD, _DWORD, _DWORD))(*(_DWORD *)v2 + 16))(
               v2,
               0,
               *(_QWORD *)((char *)v3 + 4),
               *(_QWORD *)((char *)v3 + 4) >> 32);
  }
  return result;
}

//----- (0000006E) --------------------------------------------------------
char *__fastcall ByteArray::Alloc(ByteArray *this, int a2)
{
  char *result; // r0@2

  if ( a2 > 64 )
  {
    *((_BYTE *)this + 13) = 1;
    result = (char *)operator new[](a2);
  }
  else
  {
    *((_BYTE *)this + 13) = 0;
    result = (char *)this + 20;
  }
  return result;
}

//----- (00000090) --------------------------------------------------------
ByteArray *__fastcall ByteArray::ByteArray(int a1, ByteArray *a2)
{
  ByteArray *v2; // r5@1
  ByteArray *v3; // r0@1
  ByteArray *v4; // r4@1

  v2 = a2;
  v3 = (ByteArray *)Array::Array();
  v4 = v3;
  *(_DWORD *)v3 = &off_2A0;
  ByteArray::move(v3, v2);
  return v4;
}
// 2A0: using guessed type _DWORD (__cdecl *off_2A0)(Buffer *__hidden this, String *);
// 338: using guessed type int Array::Array(void);

//----- (000000B8) --------------------------------------------------------
int __fastcall ByteArray::ByteArray(int a1, const void *a2, int a3, int a4)
{
  const void *v4; // r6@1
  int v5; // r5@1
  int v6; // r7@1
  int v7; // r4@1

  v4 = a2;
  v5 = a3;
  v6 = a4;
  v7 = Array::Array(a1);
  *(_DWORD *)v7 = &off_2A0;
  if ( v6 )
  {
    *(_DWORD *)(v7 + 8) = v5;
    Buffer::Copy((Buffer *)v7, 0, v4, v5);
  }
  else
  {
    Array::Set((Array *)v7, v4, v5);
  }
  return v7;
}
// 2A0: using guessed type _DWORD (__cdecl *off_2A0)(Buffer *__hidden this, String *);
// 33C: using guessed type int __cdecl Array::Array(_DWORD);

//----- (000000FC) --------------------------------------------------------
int __fastcall ByteArray::ByteArray(int a1, const void *a2, int a3, int a4)
{
  const void *v4; // r6@1
  int v5; // r5@1
  int v6; // r7@1
  int v7; // r4@1

  v4 = a2;
  v5 = a3;
  v6 = a4;
  v7 = Array::Array(a1);
  *(_DWORD *)v7 = &off_2A0;
  if ( v6 )
  {
    *(_DWORD *)(v7 + 8) = v5;
    Buffer::Copy((Buffer *)v7, 0, v4, v5);
  }
  else
  {
    Array::Set((Array *)v7, (void *)v4, v5);
  }
  return v7;
}
// 2A0: using guessed type _DWORD (__cdecl *off_2A0)(Buffer *__hidden this, String *);
// 33C: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000140) --------------------------------------------------------
Array *__fastcall ByteArray::ByteArray(int a1, int a2)
{
  const Buffer *v2; // r5@1
  int v3; // r2@1
  Array *v4; // r0@1
  Array *v5; // r4@1

  v2 = (const Buffer *)a2;
  v3 = *(_DWORD *)(a2 + 8);
  v4 = (Array *)Array::Array(a1);
  v5 = v4;
  *(_DWORD *)v4 = &off_2A0;
  Array::Copy(v4, 0, v2, 0, -1);
  return v5;
}
// 2A0: using guessed type _DWORD (__cdecl *off_2A0)(Buffer *__hidden this, String *);
// 31C: using guessed type _DWORD __cdecl Array::Copy(Array *__hidden this, int, const Buffer *, int, int);
// 33C: using guessed type int __cdecl Array::Array(_DWORD);

//----- (00000178) --------------------------------------------------------
Buffer *__fastcall ByteArray::ByteArray(int a1, unsigned __int8 a2, int a3)
{
  unsigned __int8 v3; // r6@1
  int v4; // r5@1
  Array *v5; // r0@1
  Buffer *v6; // r4@1

  v3 = a2;
  v4 = a3;
  v5 = (Array *)Array::Array(a1);
  v6 = v5;
  *(_DWORD *)v5 = &off_2A0;
  Array::SetLength(v5, v4);
  Buffer::Set(v6, v3, 0, v4);
  return v6;
}
// 2A0: using guessed type _DWORD (__cdecl *off_2A0)(Buffer *__hidden this, String *);
// 33C: using guessed type int __cdecl Array::Array(_DWORD);

//----- (000001B0) --------------------------------------------------------
Array *__fastcall ByteArray::ByteArray(int a1, int a2)
{
  int v2; // r5@1
  Array *v3; // r0@1
  Array *v4; // r4@1

  v2 = a2;
  v3 = (Array *)Array::Array(a1);
  v4 = v3;
  *(_DWORD *)v3 = &off_2A0;
  Array::SetLength(v3, v2);
  return v4;
}
// 2A0: using guessed type _DWORD (__cdecl *off_2A0)(Buffer *__hidden this, String *);
// 33C: using guessed type int __cdecl Array::Array(_DWORD);

//----- (000001D8) --------------------------------------------------------
ByteArray *__fastcall ByteArray::operator=(ByteArray *this, ByteArray *a2)
{
  ByteArray *v2; // r4@1

  v2 = this;
  ByteArray::move(this, a2);
  return v2;
}

//----- (000001EA) --------------------------------------------------------
int __fastcall ByteArray::operator=(int a1)
{
  int v1; // r4@1

  v1 = a1;
  Buffer::operator=();
  return v1;
}
// 34C: using guessed type int Buffer::operator=(void);

//----- (000001FC) --------------------------------------------------------
int __fastcall ByteArray::operator=(int a1)
{
  int v1; // r4@1

  v1 = a1;
  Buffer::operator=();
  return v1;
}
// 350: using guessed type int Buffer::operator=(void);

//----- (0000020E) --------------------------------------------------------
int __fastcall ByteArray::operator=(int a1)
{
  int v1; // r4@1

  v1 = a1;
  Buffer::operator=();
  return v1;
}
// 350: using guessed type int Buffer::operator=(void);

//----- (00000220) --------------------------------------------------------
int __fastcall ByteArray::Save(ByteArray *this, void *a2, int a3)
{
  ByteArray *v3; // r4@1
  _BYTE *v4; // r6@1
  int v5; // r7@1
  int v6; // r0@4

  v3 = this;
  v4 = a2;
  v5 = a3;
  if ( *((_DWORD *)this + 2) > 255 )
    assert_failed2("_Length <= 0xFF", "E:\\Smart\\SmartOS\\Core\\ByteArray.cpp", 0x92u);
  if ( *((_DWORD *)v3 + 2) > v5 )
    v6 = v5;
  else
    v6 = *((_DWORD *)v3 + 2);
  *v4 = v6;
  return (*(int (__fastcall **)(ByteArray *, _DWORD, _BYTE *, int))(*(_DWORD *)v3 + 20))(v3, 0, v4 + 1, v6);
}

//----- (000002D8) --------------------------------------------------------
int __fastcall ByteArray::~ByteArray(ByteArray *this)
{
  return Array::~Array(this);
}

//----- (000002E6) --------------------------------------------------------
void __fastcall ByteArray::~ByteArray(ByteArray *this)
{
  void *v1; // r0@1

  v1 = (void *)ByteArray::~ByteArray(this);
  operator delete(v1);
}
#endif
#if 0
ByteArray::ByteArray(int length)
{
	
}
ByteArray::ByteArray(byte item, int length)
{
	
}
// 因为使用外部指针，这里初始化时没必要分配内存造成浪费
ByteArray::ByteArray(const void* data, int length, bool copy)
{
	
}
ByteArray::ByteArray(void* data, int length, bool copy)
{
	
}
explicit ByteArray::ByteArray(const Buffer& arr)
{
	
}
ByteArray::ByteArray(ByteArray&& rval)
{
	
}

ByteArray& ByteArray::operator = (const Buffer& rhs)
{
	
}
ByteArray& ByteArray::operator = (const ByteArray& rhs)
{
	
}
ByteArray& ByteArray::operator = (const void* p)
{
	
}
ByteArray& ByteArray::operator = (ByteArray&& rval)
{
	
}

// 重载等号运算符，使用外部指针、内部长度，用户自己注意安全
//ByteArray& ByteArray::operator=(const void* data)
//{
	
//}

// 保存到普通字节数组，首字节为长度
int ByteArray::Load(const void* data, int maxsize)
{
	
}
// 从普通字节数据组加载，首字节为长度
int ByteArray::Save(void* data, int maxsize) const
{
	
}

//bool operator==(const ByteArray& bs1, const ByteArray& bs2)
//{
	
//}
//bool operator!=(const ByteArray& bs1, const ByteArray& bs2)
//{
	
//}

#if DEBUG
	void ByteArray::Test()
{
	
}
#endif

void* ByteArray::Alloc(int len)
{
	
}

void ByteArray::move(ByteArray& rval)
{
	
}
#endif
