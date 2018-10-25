#include <string.h>
#include "List.h"

IList::IList()
{
	this->_Arr=(void**)&tmpbuf;
	this->Init();
}

// ��ӵ���Ԫ��
void IList::Add(void *item)
{
	this->_Arr[this->_Count++]=item;
}

// ��Ӷ��Ԫ��
void IList::Add(void **items, int count)
{
   if ( items && count )
   {
	   while(true)
	   {
		   if(count==0)
			   break;
		   count--;
		   int pos=this->_Count;
		   this->_Count++;
		   this->_Arr[pos]=items;
		   items++;
	   }
   }
}

// ɾ��ָ��λ��Ԫ��
void IList::RemoveAt(int index)
{
	return;
}

// ɾ��ָ��Ԫ��
int IList::Remove(const void *item)
{
	int idx=this->FindIndex(item);
	if(idx>=0)
	this->RemoveAt(idx);
    return idx;
}

// ����ָ���������ʱ����-1
int IList::FindIndex(const void *item)const
{
    for (int i = 0; this->_Count > i; ++i)
    {
        if ((const void*)(this->_Arr[i]) == item)
            return i;
        if (this->Comparer(this->_Arr[i], item))
        {
            return i;
        }
    }
    return  - 1;
}

// �������������[]������ָ��Ԫ�صĵ�һ��
void *IList::operator[](int i)const
{
	if(i>=0&&this->_Count>i)
	{
		return this->_Arr[i];
	}
	else
	{
		return nullptr;
	}
}

void IList::Init()
{
	this->_Count=0;
	this->_Capacity=4;
}
