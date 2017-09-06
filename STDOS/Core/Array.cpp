#include "Array.h"
#include <stdio.h>

Array::Array(void *data, int len): Buffer(data, len)
{
	this->Init();
}
Array::Array(const void *data, int len): Buffer((void*)data, len)
{
	this->Init();
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
	return this->SetLength(len,false);
}
bool Array::SetLength(int len, bool bak)
{	  
  int baklen; 
   
  if ( this->_Length < len )
  {
    if ( bak )
      baklen = _Capacity ;
    else
      baklen = 0;
    if ( CheckCapacity(len, baklen) )
      return 0;
    if( _Capacity  < len )
      _Capacity = len;
  }
  else
  {
   _Capacity = len;
  }
  
  return true;	
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
	this->Expand=true;
	this->_needFree=false;
	this->_canWrite=true;
	this->_Size=1;
	this->_Capacity=1;
}
void Array::move(Array& rval)
{
}

// ���������������������󣬲�����ָ�����ȵ�����
bool Array::CheckCapacity(int len, int bak)
{
    bool ret;
    int i;
    
    if (this->_Arr&& _Capacity >= len &&this->_canWrite)
    {
        ret = true;
    }
    else if (Expand)
    {
        for (i = 64; i < this->_Length; i *= 2)
            ;        
        void *bufAlloc = this->Alloc(i);
        if (bufAlloc)
        {
            if (this->_Length < len)
                len = this->_Length;
            if (len > 0 && this->_Arr)
            {
                Buffer bufNew(bufAlloc, i);
                bufNew.Copy(0, _Arr, len);
            }
           						
            if (this->_needFree)
            {
                if (_Arr != bufAlloc)
                    delete (_Arr);
            }
            this->_Arr = (char*)bufAlloc;
            this->_Capacity = i;   
			this->_Length=i;
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}
void *Array::Alloc(int len)
{
	this->_needFree=true;
    return new char(this->_Size * len);
}
// �ͷ���ռ���ڴ�
bool Array::Release()
{
	bool ret;
	if(this->_canWrite&&this->_Arr)
	{
		delete[](this->_Arr);
		ret=true;
	}
	else
	{
		ret=false;
	}
	this->_Arr=0;
	this->_Length=0;
	this->_canWrite=0;	
	this->_Size=0;
	this->_Capacity=1;
	
	
    return ret;
}
