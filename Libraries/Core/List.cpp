#include <string.h>
#include "List.h"

static char off_328[4*30];

IList::IList()
{
	this->_Arr=(void**)&off_328;
	this->Init();
}

// 添加单个元素
void IList::Add(void *item)
{
	this->CheckCapacity(this->_Count+1);
	this->_Arr[this->_Count++]=item;
}

// 添加多个元素
void IList::Add(void **items, int count)
{
   if ( items && count )
   {
	   this->CheckCapacity(this->_Count+count);
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
	this->_Arr=this->Arr;	
    this->_Count=0;
	this->_Capacity=4;
}

bool IList::CheckCapacity(int count)
{
	int i;
	if(this->_Arr&&this->_Capacity>=count)
	{
		return true;
	}
	else
	{
		for(i=16;i<count;i*=2);
		void* parrnew=operator new[](4 * i);
		if(parrnew)
		{
			if(this->_Count>0 &&this->_Arr)
			{
#if 0
				Buffer *buf=new Buffer(parrnew,4*i);
				buf->Copy(0,this->_Arr,4*this->_Count);
#else
				memcpy(parrnew,this->_Arr,4*this->_Count);
#endif
			}
			if(this->_Arr)
			{
				if(this->_Arr!=this->Arr)
					 operator delete[](this->_Arr);
			}
			this->_Arr=(void**)parrnew;
			this->_Capacity=i;			
			
			return true;
		}
		else
		{
			return false;
		}
	}
}
