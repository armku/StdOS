#include "Sys.h"

Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = (char*)ptr;
    this->_Length = len;   

// int v3; // r5@1
//  int v4; // r6@1
//  int result; // r0@1

//  v3 = a2;
//  v4 = a3;
//  result = Object::Object();
//  *(_DWORD *)result = &off_934;
//  *(_DWORD *)(result + 4) = v3;
//  *(_DWORD *)(result + 8) = v4;
//  return result;	
}

Buffer &Buffer::operator = (const Buffer &rhs)
{
    if (this->_Length < rhs._Length)
    {
        debug_printf("Error: Buffer copy: Buffer length mismath src: %d ,dst: %d \n", rhs._Length, this->_Length);
    }
    else
    {
        for (int i = 0; i < rhs.Length(); i++)
        {
            ((byte*)(this->_Arr))[i] = rhs.GetBuffer()[i];
        }
        this->_Length = rhs._Length;
    }
    return  *this;
	
	
//	int v2; // r3@1

//  Buffer::move(a1, a2);
//  return v2;
}
//Buffer *__fastcall Buffer::operator=(Buffer *a1, Buffer *a2)
//{
//  Buffer *v2; // r4@1
//  const Buffer *v3; // r5@1
//  int v4; // r0@1

//  v2 = a1;
//  v3 = a2;
//  v4 = Buffer::Length(a2);
//  if ( !(*(int (__fastcall **)(Buffer *, int))(*(_DWORD *)v2 + 12))(v2, v4) )
//    assert_failed2((const char *)dword_44C, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x29u);
//  Buffer::Copy(v2, v3, 0);
//  return v2;
//}
// 对象mov操作，指针和长度归我，清空对方
Buffer::Buffer(Buffer&& rval)
{
//	int v2; // r5@1
//  _DWORD *v3; // r0@1
//  _DWORD *v4; // r4@1

//  v2 = a2;
//  v3 = (_DWORD *)Object::Object();
//  v4 = v3;
//  *v3 = &off_934;
//  Buffer::move((int)v3, v2);
//  return v4;
}

Buffer &Buffer::operator = (const void *prt)
{
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = ((byte*)prt)[i];
    }
    return  *this;
	
	
//	int v2; // r5@1

//  v2 = a1;
//  if ( a2 )
//    (*(void (**)(void))(*(_DWORD *)a1 + 16))();
//  return v2;
}

//设置长度，可自动扩容 
bool Buffer::SetLength(int len)
{
    
    {
        this->_Length = len;
    }
    return true;
	
//	signed int result; // r0@2

//  if ( *((_DWORD *)this + 2) >= a2 )
//  {
//    *((_DWORD *)this + 2) = a2;
//    result = 1;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}

//设置指定位置的值，长度不足时报错
bool Buffer::SetAt(int index, byte value)
{
    if (index >= this->_Length)
    {
        return false;
    }
    ((byte*)this->_Arr)[index] = value;
    return true;
	
	
//	Buffer *v3; // r4@1
//  int v4; // r5@1
//  unsigned __int8 v5; // r6@1
//  signed int result; // r0@3

//  v3 = this;
//  v4 = a2;
//  v5 = a3;
//  if ( *((_DWORD *)this + 2) > a2 || (*(int (__fastcall **)(_DWORD, _DWORD))(*(_DWORD *)this + 12))(this, a2 + 1) )
//  {
//    *(_BYTE *)(*((_DWORD *)v3 + 1) + v4) = v5;
//    result = 1;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}

//自我索引运算符[] 返回指定元素的第一个字节
byte Buffer::operator[](int i)const
{
    return ((byte*)this->_Arr)[i];
	
//	int v2; // r5@1
//  int v3; // r4@1

//  v2 = a1;
//  v3 = a2;
//  if ( a2 < 0 || *(_DWORD *)(a1 + 8) <= a2 )
//    assert_failed2((const char *)dword_4B0, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x54u);
//  return *(_DWORD *)(v2 + 4) + v3;
}

//支持buf[i]=0x33的语法
byte &Buffer::operator[](int i)
{
    if ((i < 0) || (i > this->_Length))
    {
        debug_printf("Error: [] length error");
        return ((byte*)(this->_Arr))[0];
    }
    return ((byte*)(this->_Arr))[i];
}

//原始拷贝 清零 不检查边界
void Buffer::Copy(void *dest, const void *source, int len)
{
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

void Buffer::Zero(void *dest, int len)
{
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[i] = 0;
    }
//	return _aeabi_memclr();
}

//拷贝数据，默认-1长度表示当前长度 
int Buffer::Copy(int destIndex, const void *src, int len)
{
    if (len ==  - 1)
    {
        len = this->_Length;
    }
    if (len <= 0)
    {
        return 0;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)(this->_Arr))[destIndex + i] = ((byte*)src)[i];
    }
	return len;
	
	
//	Buffer *v4; // r4@1
//  int v5; // r6@1
//  const void *v6; // r7@1
//  int v7; // r5@1
//  int result; // r0@2
//  int v9; // r8@7

//  v4 = this;
//  v5 = a2;
//  v6 = a3;
//  v7 = a4;
//  if ( !a3 )
//    return 0;
//  if ( !*((_DWORD *)this + 1) || !*((_DWORD *)this + 2) )
//    return 0;
//  v9 = *((_DWORD *)this + 2) - a2;
//  if ( a4 >= 0 )
//  {
//    if ( a4 > v9 && !(*(int (__fastcall **)(Buffer *, int))(*(_DWORD *)this + 12))(this, a2 + a4) )
//    {
//      SmartOS_printf("Buffer::Copy (0x%p, %d) <= (%d, 0x%p, %d) \r\n", *(_QWORD *)((char *)v4 + 4), v5);
//      assert_failed2((const char *)dword_24C, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x95u);
//      v7 = v9;
//    }
//  }
//  else
//  {
//    if ( v9 <= 0 )
//    {
//      SmartOS_printf("Buffer::Copy (0x%p, %d) <= (%d, 0x%p, %d) \r\n", *(_QWORD *)((char *)this + 4), a2, a3, a4);
//      assert_failed2((const char *)dword_220, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x87u);
//      return 0;
//    }
//    v7 = *((_DWORD *)this + 2) - a2;
//  }
//  if ( *((_DWORD *)v4 + 1) )
//  {
//    if ( (const void *)(*((_DWORD *)v4 + 1) + v5) == v6 )
//    {
//      result = v7;
//    }
//    else
//    {
//      if ( v7 )
//        _aeabi_memmove(*((_DWORD *)v4 + 1) + v5, v6, v7);
//      result = v7;
//    }
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}

//把数据复制到目标缓冲区，默认-1长度表示当前长度
int Buffer::CopyTo(int destIndex, void *dest, int len)const
{
    if (len ==  - 1)
    {
        len = this->_Length;
    }
    if (len <= 0)
    {
        return 0;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[destIndex + i] = ((byte*)(this->_Arr))[i];
    }
	return len;
}

//拷贝数据，默认-1长度表示两者最小长度 
int Buffer::Copy(int destIndex, const Buffer &src, int srcIndex, int len)
{
    if (len ==  - 1)
    {
        len = this->_Length;
        if (len < src._Length)
        {
            this->SetLength(src._Length);
            len = this->_Length;
        }
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)this->_Arr)[destIndex + i] = ((byte*)(src._Arr))[srcIndex + i];
    }
	return len;
	
	
	
//	int v5; // r4@1
//  int v6; // r7@2
//  int v7; // r0@3

//  v5 = a1;
//  if ( a5 < 0 )
//    v6 = *(_DWORD *)(a1 + 8) - a2;
//  *(_DWORD *)(a3 + 8);
//  v7 = *(_DWORD *)(a3 + 4);
//  return (*(int (__fastcall **)(int))(*(_DWORD *)v5 + 16))(v5);
}

int Buffer::Copy(const Buffer &src, int destIndex)
{
    int len = this->Length();
    if ((destIndex + this->Length()) > src.Length())
    {
        len = src.Length();
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)this->_Arr)[i + destIndex] = src[i];
    }
    return len;
	
//	Buffer *v3; // r5@1
//  const Buffer *v4; // r4@1
//  int v5; // r6@1
//  int v6; // r0@1

//  v3 = this;
//  v4 = a2;
//  v5 = a3;
//  v6 = Buffer::Length(a2);
//  return (*(int (__fastcall **)(Buffer *, int, _DWORD, int))(*(_DWORD *)v3 + 16))(v3, v5, *((_DWORD *)v4 + 1), v6);
}
//int __fastcall Buffer::CopyTo(Buffer *this, int a2, void *a3, int a4)
//{
//  Buffer *v4; // r5@1
//  int v5; // r4@1
//  int result; // r0@3
//  int v7; // r0@5

//  v4 = this;
//  v5 = a4;
//  if ( *((_DWORD *)this + 1) && a3 )
//  {
//    v7 = *((_DWORD *)this + 2);
//    if ( v7 - a2 > 0 )
//    {
//      if ( a4 < 0 || a4 > v7 - a2 )
//        v5 = v7 - a2;
//      if ( v5 )
//      {
//        if ( (void *)(*((_DWORD *)v4 + 1) + a2) != a3 )
//          _aeabi_memmove(a3, *((_DWORD *)v4 + 1) + a2, v5);
//      }
//      result = v5;
//    }
//    else
//    {
//      result = 0;
//    }
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
//}
//用指定字节设置初始化一个区域
int Buffer::Set(byte item, int index, int len)
{
    if (index + this->Length() < len)
    {
        len = this->Length() - index;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)this->_Arr)[index + i] = item;
    }
    return len;
	
	
	Buffer *v4; // r5@1
  int v5; // r4@1
  int result; // r0@3
  int v7; // r0@5

//  v4 = this;
//  v5 = a4;
//  if ( *((_DWORD *)this + 1) && a4 )
//  {
//    v7 = *((_DWORD *)this + 2);
//    if ( v7 - a3 > 0 )
//    {
//      if ( a4 < 0 || a4 > v7 - a3 )
//        v5 = v7 - a3;
//      if ( v5 )
//        _aeabi_memset(*((_DWORD *)v4 + 1) + a3, v5, a2);
//      result = v5;
//    }
//    else
//    {
//      result = 0;
//    }
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}
//int __fastcall BufferRef::Set(int result, void *a2, int a3)
//{
//  *(_DWORD *)(result + 4) = a2;
//  *(_DWORD *)(result + 8) = a3;
//  return result;
//}
void Buffer::Clear(byte item)
{
    for (int i = 0; i < this->Length(); i++)
    {
        ((byte*)this->_Arr)[i] = item;
    }
}

//截取一个子缓冲区，默认-1长度表示剩余全部		
Buffer Buffer::Sub(int index, int length)
{
    if (length < 0)
    {
        return Buffer(((byte*)this->_Arr) + index, this->Length() - index);
    }
    if (index + length > this->_Length)
    {
        length = this->_Length - index;
    }
    return Buffer(((byte*)this->_Arr) + index, length);
	
	
//	 Buffer *v4; // r7@1
//  int v5; // r5@1
//  int v6; // r4@1
//  int v7; // r6@1

//  v4 = this;
//  v5 = a2;
//  v6 = a3;
//  v7 = a4;
//  if ( a3 < 0 )
//    assert_failed2("index >= 0", "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0xE6u);
//  if ( *(_DWORD *)(v5 + 8) <= v6 )
//    assert_failed2("index < _Length", "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0xE7u);
//  if ( v7 < 0 )
//    v7 = *(_DWORD *)(v5 + 8) - v6;
//  if ( v6 + v7 > *(_DWORD *)(v5 + 8) )
//    assert_failed2("len <= _Length", "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0xE9u);
//  return Buffer::Buffer(v4, *(_DWORD *)(v5 + 4) + v6, v7);
}

const Buffer Buffer::Sub(int index, int len)const
{
    if (len < 0)
    {
        return Buffer(((byte*)this->_Arr) + index, this->Length() - index);
    }
    if (index + len > this->_Length)
    {
        len = this->_Length - index;
    }
    return Buffer(((byte*)this->_Arr) + index, len);
	
//	Buffer *v4; // r7@1
//  int v5; // r5@1
//  int v6; // r4@1
//  int v7; // r6@1

//  v4 = this;
//  v5 = a2;
//  v6 = a3;
//  v7 = a4;
//  if ( a3 < 0 )
//    assert_failed2("index >= 0", "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0xF0u);
//  if ( v7 < 0 )
//    v7 = *(_DWORD *)(v5 + 8) - v6;
//  if ( v6 + v7 > *(_DWORD *)(v5 + 8) )
//    SmartOS_printf("Buffer::Sub (%d, %d) > %d \r\n", v6, v7, *(_DWORD *)(v5 + 8));
//  if ( v6 + v7 > *(_DWORD *)(v5 + 8) )
//    assert_failed2("index + len <= _Length", "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0xF6u);
//  return Buffer::Buffer((int)v4, *(_DWORD *)(v5 + 4) + v6, v7);
}

#if 0
    //显示16禁止数据，指定分割字符和换行长度
    String &Buffer::ToHex(String &str, char sep, int newLine)const
    {
        return  *String("Fuck", 5);
		
		Buffer *v4; // r9@1
  String *v5; // r4@1
  int v6; // r6@1
  unsigned __int8 *v7; // r7@1
  signed int v8; // r5@1
  char v10; // [sp+8h] [bp-28h]@1

  v10 = a3;
  v4 = this;
  v5 = a2;
  v6 = a4;
  v7 = (unsigned __int8 *)Buffer::GetBuffer(this);
  v8 = 0;
  while ( Buffer::Length(v4) > v8 )
  {
    if ( v8 )
    {
      if ( v6 <= 0 || v8 != v6 * (v8 / v6) )
      {
        if ( v10 )
          String::Concat(v5, v10);
      }
      else
      {
        String::Concat(v5, (const char *)&dword_600);
      }
    }
    String::Concat(v5, *v7, -16);
    ++v8;
    ++v7;
  }
  return v5;
    }

    //显示16禁止数据，指定分割字符和换行长度
    String Buffer::ToHex(char sep, int newLine)
    {
        return  *String("Fuck", 5);
		Buffer *v4; // r5@1
  char v5; // r6@1
  int v6; // r7@1
  String *v7; // r0@1

  v4 = (Buffer *)a2;
  v5 = a3;
  v6 = a4;
  v7 = (String *)String::String();
  return Buffer::ToHex(v4, v7, v5, v6);
    }
#endif 
#if 0
    ushort Buffer::ToUInt16()const{}
    uint Buffer::ToUInt32()const{}

    UInt64 Buffer::ToUInt64()const{}
#endif 
void Buffer::Write(ushort value, int index)
{
//int v4; // [sp+4h] [bp-1Ch]@1
//  int v5; // [sp+8h] [bp-18h]@1

//  v4 = a2;
//  v5 = a3;
//  return (*(int (__cdecl **)(Buffer *, int, int *))(*(_DWORD *)this + 16))(this, a3, &v4);
}
void Buffer::Write(short value, int index)
{
//	int v4; // [sp+4h] [bp-1Ch]@1
//  int v5; // [sp+8h] [bp-18h]@1

//  v4 = a2;
//  v5 = a3;
//  return (*(int (__cdecl **)(Buffer *, int, int *))(*(_DWORD *)this + 16))(this, a3, &v4);
}
void Buffer::Write(uint value, int index)
{
//unsigned int v4; // [sp+4h] [bp-1Ch]@1
//  int v5; // [sp+8h] [bp-18h]@1

//  v4 = a2;
//  v5 = a3;
//  return (*(int (__cdecl **)(_DWORD, _DWORD, _DWORD))(*(_DWORD *)this + 16))(this, a3, &v4);
}
void Buffer::Write(int value, int index)
{
//	int v4; // [sp+4h] [bp-1Ch]@1
//  int v5; // [sp+8h] [bp-18h]@1

//  v4 = a2;
//  v5 = a3;
//  return (*(int (__cdecl **)(Buffer *, int, int *))(*(_DWORD *)this + 16))(this, a3, &v4);
}
void Buffer::Write(UInt64 value, int index)
{
//	unsigned __int64 v4; // [sp+8h] [bp-18h]@1

//  v4 = a3;
//  (*(void (__cdecl **)(Buffer *, int, unsigned __int64 *, signed int))(*(_DWORD *)this + 16))(this, a4, &v4, 8);
}
#if 0
    //输出对象的字符串表示方式
    String &Buffer::ToStr(String &str)const
{
	return Buffer::ToHex(this, a2, 45, 32);
}
    //包装为字符串对象
    String Buffer::AsString()const{}
#endif 
int Buffer::CompareTo(const Buffer &bs)const
{
    for (int i = 0; i < this->_Length; i++)
    {
        if (this->GetBuffer()[i] > bs[i])
        {
            return 1;
        }
        if (this->GetBuffer()[i] < bs[i])
        {
            return  - 1;
        }
    }
    return 0;
}

int Buffer::CompareTo(const void *ptr, int len)const
{
    if (len > this->_Length)
    {
        len = this->_Length;
    }
    int ret = 0;
    for (int i = 0; i < this->_Length; i++)
    {
        if (this->GetBuffer()[i] > ((byte*)ptr)[i])
        {
            return 1;
        }
        if (this->GetBuffer()[i] < ((byte*)ptr)[i])
        {
            return  - 1;
        }
    }
    return ret;
}


bool operator == (const Buffer &bs1, const Buffer &bs2)
{
    if (bs1.Length() != bs2.Length())
    {
        return false;
    }
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != bs2[i])
        {
            return false;
        }
    }
    return true;
}


bool operator == (const Buffer &bs1, const void *ptr)
{
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != ((byte*)ptr)[i])
        {
            return false;
        }
    }
    return true;
}

bool operator != (const Buffer &bs1, const Buffer &bs2)
{
    if (bs1.Length() != bs2.Length())
    {
        return true;
    }
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != bs2[i])
        {
            return true;
        }
    }
    return false;
}

bool operator != (const Buffer &bs1, const void *ptr)
{
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != ((byte*)ptr)[i])
        {
            return true;
        }
    }
    return false;
}















//void Buffer::Show(bool newLine)const
//{
//    if (newLine)
//    {
//        for (int i = 0; i < this->_Length - 1; i++)
//        {
//            printf("%02X ", ((byte*)(this->_Arr))[i]);
//        }
//        printf("%02X", ((byte*)(this->_Arr))[this->_Length - 1]);
//    }
//    else
//    {
//        printf("%s", ((byte*)(this->_Arr)));
//    }    
//    Object::Show(true);
//}


char chartmp[512];
//Buffer::Buffer(){}
String &Buffer::ToStr(String &str)const
{	
    String *ret = new String();
    return  *ret;
}

//int __fastcall Buffer::AsString(Buffer *this, int a2)
//{
//  __int64 v2; // r1@1

//  v2 = *(_QWORD *)(a2 + 4);
//  return String::String();
//}
//int __fastcall Buffer::ToUInt16(Buffer *this, int a2, int a3)
//{
//  int v3; // r6@1
//  int v4; // r4@1
//  int result; // r0@3

//  v3 = a3;
//  v4 = Buffer::GetBuffer(this) + a2;
//  if ( v3 )
//  {
//    if ( v4 & 1 )
//      result = *(_BYTE *)v4 | (*(_BYTE *)(v4 + 1) << 8);
//    else
//      result = *(_WORD *)v4;
//  }
//  else
//  {
//    result = *(_BYTE *)(v4 + 1) | (*(_BYTE *)v4 << 8);
//  }
//  return result;
//}
//int __fastcall Buffer::ToUInt32(Buffer *this, int a2, int a3)
//{
//  int v3; // r6@1
//  int v4; // r4@1
//  int result; // r0@3

//  v3 = a3;
//  v4 = Buffer::GetBuffer(this) + a2;
//  if ( v3 )
//  {
//    if ( v4 & 3 )
//      result = *(_BYTE *)v4 | (*(_BYTE *)(v4 + 1) << 8) | (*(_BYTE *)(v4 + 2) << 16) | (*(_BYTE *)(v4 + 3) << 24);
//    else
//      result = *(_DWORD *)v4;
//  }
//  else
//  {
//    result = _byteswap_ulong(*(_DWORD *)v4);
//  }
//  return result;
//}
//int __fastcall Buffer::ToUInt64(Buffer *this, int a2, int a3)
//{
//  Buffer *v3; // r9@1
//  int v4; // r5@1
//  int v5; // r7@1
//  int v6; // r8@2
//  __int64 v7; // r0@3
//  int v8; // r6@5

//  v3 = this;
//  v4 = a2;
//  v5 = a3;
//  if ( a3 )
//  {
//    v6 = Buffer::GetBuffer(this) + a2;
//    if ( v6 & 7 )
//    {
//      v8 = Buffer::ToUInt32(v3, v4, v5);
//      Buffer::ToUInt32(v3, v4 + 4, v5);
//      LODWORD(v7) = v8;
//    }
//    else
//    {
//      v7 = *(_QWORD *)v6;
//    }
//  }
//  else
//  {
//    Buffer::ToUInt32(this, a2, 0);
//    LODWORD(v7) = Buffer::ToUInt32(v3, v4 + 4, v5);
//  }
//  return v7;
//}
//int __fastcall Buffer::CompareTo(Buffer *this, const void *a2, int a3)
//{
//  Buffer *v3; // r3@1
//  _BYTE *v4; // r4@1
//  int result; // r0@4
//  int v6; // r1@6
//  _BYTE *v7; // r5@8
//  _BYTE *v8; // r6@8
//  int v9; // r7@9
//  int v10; // r12@9

//  v3 = this;
//  v4 = a2;
//  if ( a3 < 0 )
//    a3 = *((_DWORD *)this + 2);
//  if ( *((const void **)this + 1) == a2 )
//  {
//    result = *((_DWORD *)this + 2) - a3;
//  }
//  else
//  {
//    v6 = *((_DWORD *)this + 2);
//    if ( v6 > a3 )
//      v6 = a3;
//    v7 = (_BYTE *)*((_DWORD *)this + 1);
//    v8 = v4;
//    while ( v6 > 0 )
//    {
//      v9 = *v7;
//      v10 = *v8;
//      result = v9 - v10;
//      if ( v9 != v10 )
//        return result;
//      --v6;
//      ++v7;
//      ++v8;
//    }
//    result = *((_DWORD *)v3 + 2) - a3;
//  }
//  return result;
//}
//int __fastcall Buffer::CompareTo(Buffer *this, const Buffer *a2)
//{
//  return Buffer::CompareTo(this, (const void *)*(_QWORD *)((char *)a2 + 4), *(_QWORD *)((char *)a2 + 4) >> 32);
//}
//int __fastcall Buffer::operator[](int a1, int a2)
//{
//  int v2; // r5@1
//  int v3; // r4@1

//  v2 = a1;
//  v3 = a2;
//  if ( a2 < 0 || *(_DWORD *)(a1 + 8) <= a2 )
//    assert_failed2((const char *)dword_830, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x4Bu);
//  return *(_BYTE *)(*(_DWORD *)(v2 + 4) + v3);
//}
//int __fastcall operator==(Buffer *this, void *a2)
//{
//  int result; // r0@2

//  if ( *((void **)this + 1) == a2 )
//  {
//    result = 1;
//  }
//  else if ( *((_DWORD *)this + 1) && a2 )
//  {
//    result = Buffer::CompareTo(this, a2, -1) == 0;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
//}
//int __fastcall operator==(Buffer *this, int a2)
//{
//  Buffer *v2; // r5@1
//  const Buffer *v3; // r4@1
//  int result; // r0@2
//  int v5; // r6@7

//  v2 = this;
//  v3 = (const Buffer *)a2;
//  if ( *((_DWORD *)this + 1) == *(_DWORD *)(a2 + 4) )
//  {
//    result = 1;
//  }
//  else if ( *((_DWORD *)this + 1) && *(_DWORD *)(a2 + 4) )
//  {
//    v5 = Buffer::Length(this);
//    if ( v5 == Buffer::Length(v3) )
//      result = Buffer::CompareTo(v2, v3) == 0;
//    else
//      result = 0;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
//}
//int __fastcall operator!=(Buffer *this, void *a2)
//{
//  int result; // r0@2

//  if ( *((void **)this + 1) == a2 )
//  {
//    result = 0;
//  }
//  else if ( *((_DWORD *)this + 1) && a2 )
//  {
//    result = Buffer::CompareTo(this, a2, -1) != 0;
//  }
//  else
//  {
//    result = 1;
//  }
//  return result;
//}
//int __fastcall operator!=(Buffer *this, int a2)
//{
//  Buffer *v2; // r5@1
//  const Buffer *v3; // r4@1
//  int result; // r0@2
//  int v5; // r6@7

//  v2 = this;
//  v3 = (const Buffer *)a2;
//  if ( *((_DWORD *)this + 1) == *(_DWORD *)(a2 + 4) )
//  {
//    result = 0;
//  }
//  else if ( *((_DWORD *)this + 1) && *(_DWORD *)(a2 + 4) )
//  {
//    v5 = Buffer::Length(this);
//    if ( v5 == Buffer::Length(v3) )
//      result = Buffer::CompareTo(v2, v3) != 0;
//    else
//      result = 1;
//  }
//  else
//  {
//    result = 1;
//  }
//  return result;
//}
//int __fastcall Buffer::Length(Buffer *this)
//{
//  return *((_DWORD *)this + 2);
//}
//_DWORD *__fastcall Object::Object(_DWORD *result)
//{
//  *result = (char *)&`vtable for'Object + 8;
//  return result;
//}
//int __fastcall Buffer::GetBuffer(Buffer *this)
//{
//  return *((_DWORD *)this + 1);
//}
void Buffer::move(Buffer& rval)
{
//	*(_DWORD *)(result + 4) = *(_DWORD *)(a2 + 4);
//  *(_DWORD *)(result + 8) = *(_DWORD *)(a2 + 8);
//  *(_DWORD *)(a2 + 4) = 0;
//  *(_DWORD *)(a2 + 8) = 0;
//  return result;
}
String  Buffer::ToString()const
{
	for(int i=0;i<this->_Length;i++)
	{
		chartmp[i]=this->_Arr[i];
	}
	chartmp[this->_Length]=0;
    return String(chartmp);
}


#if DEBUG
    void Buffer::Test()
    {
        #if 0
            //使用指针和长度构造一个内存区
            char cs[] = "This is Buffer Test.";
            Buffer bs(cs, sizeof(cs));
            debug_printf("Buffer bs (cs,strlen(cs)) => %s\r\n", cs);
            assert(bs.GetBuffer() == (byte*)cs, "GetBuffer()");
            assert(bs == cs, "Buffer(void* p = nullptr, int len = 0)");
        #endif 
        #if 0
            byte buf[] = 
            {
                1, 2, 3, 4
            };
            byte bts[] = 
            {
                5, 6, 7, 8, 9, 10
            };
            Buffer bs1(buf, sizeof(buf));

            char err[] = "Buffer& operator = (const void* ptr)";

            //从指针拷贝，使用我的长度
            bs1 = bts;
            assert(bs1.GetBuffer() == buf, err);
            assert(bs1.GetBuffer() != bts, err);
            assert(bs1.Length() == sizeof(buf), err);
            assert(buf[0] == bts[0] && buf[3] == bts[3], err);
        #endif 
        #if 0
            byte buf[] = 
            {
                1, 2, 3, 4
            };
            byte bts[] = 
            {
                5, 6, 7
            };
            Buffer bs(buf, sizeof(buf));
            Buffer bs2(bts, sizeof(bts));

            char err[] = "Buffer& operator = (const Buffer& rhs)";

            //从另一个对象拷贝数据和长度 长度不足且扩容失败时报错
            //Buffer 无法自动扩容，Array可以
            //bs2 = bs;
            bs = bs2;
            assert(bs.GetBuffer() == buf, err);
            assert(bs.GetBuffer() != bts, err);
            assert(bs.Length() == sizeof(bts), err);
            assert(bs.Length() != sizeof(buf), err);
            assert(buf[0] == bts[0] && buf[2] == bts[2], err);
            assert(buf[3] == 4, err);
        #endif 

    }
#endif
