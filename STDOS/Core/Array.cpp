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
// �������鳤�ȡ������㹻����СLength������������ȷ�����������㹻������η����ڴ�
bool Array::SetLength(int len)
{
}
bool Array::SetLength(int len, bool bak)
{
}
// �������ݣ�Ĭ��-1���ȱ�ʾʹ���ұ���󳤶ȣ���߲���ʱ�Զ�����
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
