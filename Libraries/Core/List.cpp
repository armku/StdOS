#include <string.h>
#include "List.h"

IList::IList()
{
	this->_Arr=(void**)&tmpbuf;
	this->Init();
}

// 添加单个元素
void IList::Add(void *item)
{
	this->_Arr[this->_Count++]=item;
}

// 添加多个元素
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

// 删除指定位置元素
void IList::RemoveAt(int index)
{
	return;
}

// 删除指定元素
int IList::Remove(const void *item)
{
	int idx=this->FindIndex(item);
	if(idx>=0)
	this->RemoveAt(idx);
    return idx;
}

// 查找指定项。不存在时返回-1
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

// 重载索引运算符[]，返回指定元素的第一个
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
