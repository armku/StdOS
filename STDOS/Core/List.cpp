#include "List.h"
#include "Buffer.h"
#include "stdio.h"

char off_328[8];

IList::IList()
{
	this->_Arr=(void**)&off_328;
	this->Init();
}
IList::IList(const IList &list)
{
	this->_Arr=(void**)&off_328;
	this->Init();
	*this=list;
}
IList::IList(IList && list)
{
	this->_Arr=(void**)&off_328;
	this->Init();
	this->move(list);
}
IList::~IList()
{
	delete((void *)this->_Arr);
}
IList &IList::operator = (const IList &list)
{
	if(this!=&list)
	{
		
	}
	return *this;
}
IList &IList::operator = (IList && list)
{
	if(this!=&list)
	{
		this->move(list);
	}
	return *this;
}
// ��ӵ���Ԫ��
void IList::Add(void *item)
{
	this->CheckCapacity(this->_Capacity+1);
	this->_Capacity++;
	this->_Arr[this->_Count]=item;
}

// ��Ӷ��Ԫ��
void IList::Add(void **items, int count)
{
    return ;
}

// ɾ��ָ��λ��Ԫ��
void IList::RemoveAt(int index)
{
    if (this->_Capacity > 0 && index < this->_Capacity)
    {
        int v7 = _Capacity - 1-index;
        if (_Capacity - 1 != index)
        {
			Buffer v88(this->Arr[4 * index], 4 * v7); 
			v88.Copy(0,this->Arr[4 * (index+1)],4*v7);
           //Buffer* v8 = (Buffer *)Buffer::Buffer(&pindex, *(_DWORD *)(this + 8) + 4 * index, 4 * v7);         
			// Buffer::Copy(v8, 0, (const void*)(*(_DWORD*)(pthis + 8) + 4 *(v5 + 1)), 4 *v7);
        }
        this->_Capacity--;
    }
}

// ɾ��ָ��Ԫ��
int IList::Remove(const void *item)
{
	int idx=this->FindIndex(item);
	if(idx>=0)
	this->RemoveAt(idx);
    return idx;
}

void IList::Clear()
{
   this->_Capacity=0;
}

// ����ָ���������ʱ����-1
int IList::FindIndex(const void *item)const
{
//    for (int i = 0; *(*this->_Arr) > i; ++i)
//    {
//        if (*(const void **)this->_Arr[4 *i] == item)
//            return i;
//        if (this->_Arr && !(*((this->_Arr[4 *i], item))
//        {
//            return i; 
//        }
//    }

    return  - 1; 
}

// �ͷ�����ָ��ָ����ڴ�
IList &IList::DeleteAll()
{
	
}

// �������������[]������ָ��Ԫ�صĵ�һ��
void *IList::operator[](int i)const
{
	if(i>0&&this->_Count>i)
	{
		return this->_Arr[4*i];
	}
	else
	{
		return nullptr;
	}
}
void * &IList::operator[](int i)
{
	if(i>0&&this->_Count>i)
	{
		return this->_Arr[4*i];
	}
	else
	{
//		return nullptr;
	}
}

void IList::Init()
{
	this->_Arr=nullptr;	
    this->_Count=this->_Capacity+20;
	this->_Capacity=0;
	//this->Arr=4;	
}

bool IList::CheckCapacity(int count)
{
    if (count <= this->_Capacity)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void IList::move(IList &list)
{
    return ;
}
