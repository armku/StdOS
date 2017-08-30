#include "Array.h"

Array::Array(void *data, int len): Buffer(data, len)
{

}
Array::Array(const void *data, int len): Buffer((void*)data, len)
{

}
//Array::Array(Array&& rval)
//{
//}
//Array::Array(const Buffer& rhs)
//{
//}
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
// 拷贝数据，默认-1长度表示使用右边最大长度，左边不足时自动扩容
int Array::Copy(int destIndex, const Buffer& src, int srcIndex, int len)
{
	Array *v5; 
  int destIndex111;
  const Buffer *src11;
  int srcIndex111; 
  int len11; 
  int v10; 
  int v11; 

  v5 = this;
  destIndex111 = destIndex;
  src11 = &src;
  srcIndex111 = srcIndex;
  len11 = len;
  v10 =src.Length();
  v11 = v10 - srcIndex111;
  if ( len >= 0 )
  {
    if ( len > v11 )
      len11 = v10 - srcIndex111;
  }
  else
  {
    len11 = v10 - srcIndex111;
    if ( v11 <= 0 )
      return 0;
  }
  if (this->Length() < destIndex111 + len11 )
	this->CheckCapacity(destIndex111 + len11,1);
  //return Buffer::Copy(destIndex111, src11, srcIndex111, len11);
}

// 设置数组元素为指定值，自动扩容
bool Array::SetItem(const void* data, int index, int count)
{
}
// 设置数组。直接使用指针，不拷贝数据
bool Array::Set(void* data, int len)
{
	if(this->Set((const void*)data,len))
	{
		this->_Size=1;
		return true;
	}
	else
	{
		return false;
	}
}
// 设置数组。直接使用指针，不拷贝数据
bool Array::Set(const void* data, int len)
{
	if((this->_needFree)&&(this->_Arr!=data))
	{
		//this->Alloc(this->_Length);
	}
	
	this->_Arr=(char*)data;
	this->_Length=len;
	this->_Size=len;	
	this->_needFree=false;
	this->_canWrite=false;
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
void *Array::Alloc(int len)
{
    return 0;
}
// 释放已占用内存
bool Array::Release()
{
    return true;
}
