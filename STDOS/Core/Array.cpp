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
	return *this;
}
Array& Array::operator = (const void* p)
{
	return *this;
}
Array& Array::operator = (Array&& rval)
{
	return *this;
}
// �������鳤�ȡ������㹻����СLength������������ȷ�����������㹻������η����ڴ�
bool Array::SetLength(int len)
{
	return this->CheckCapacity(len,false);
}
bool Array::SetLength(int len, bool bak)
{
	int v5=this->_Length;
	if(this->_Length<len)
	{
		int v6;
		if(len)
		{
			v6=this->_Length;
		}
		else
		{
			v6=0;
		}
		if(!this->CheckCapacity(v6,bak))
			return false;
		if(this->_Length<v5)
			this->_Length=v5;
	}
	else
	{
		this->_Length=v5;
	}
	return false;
}
// �������ݣ�Ĭ��-1���ȱ�ʾʹ���ұ���󳤶ȣ���߲���ʱ�Զ�����
int Array::Copy(int destIndex, const Buffer& src, int srcIndex, int len)
{
  int srclen =src.Length();
  int v11 = srclen - srcIndex;
  if ( len >= 0 )
  {
    if ( len > v11 )
      len = srclen - srcIndex;
  }
  else
  {
    len = srclen - srcIndex;
    if ( v11 <= 0 )
      return 0;
  }
  if (this->Length() < destIndex + len )
	this->CheckCapacity(destIndex + len,1);
  //return Buffer::Copy(destIndex, src, srcIndex, len);
  return len;
}

// ��������Ԫ��Ϊָ��ֵ���Զ�����
bool Array::SetItem(const void* data, int index, int count)
{
}
// �������顣ֱ��ʹ��ָ�룬����������
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
// �������顣ֱ��ʹ��ָ�룬����������
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
// ����Ѵ洢���ݡ�
void Array::Clear()
{
}
// ����ָ��λ�õ�ֵ������ʱ�Զ�����
void Array::SetItemAt(int i, const void* item)
{
}

// �������������[]������ָ��Ԫ�صĵ�һ���ֽ�
byte Array::operator[](int i) const
{
}
byte& Array::operator[](int i)
{
}

bool operator==(const Array& bs1, const Array& bs2)
{
	return bs2.CompareTo(bs2)==0;
}
bool operator!=(const Array& bs1, const Array& bs2)
{
	return bs2.CompareTo(bs2)!=0;
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

// ���������������������󣬲�����ָ�����ȵ�����
bool Array::CheckCapacity(int len, int bak)
{
}
void *Array::Alloc(int len)
{
    return 0;
}
// �ͷ���ռ���ڴ�
bool Array::Release()
{
    return true;
}
