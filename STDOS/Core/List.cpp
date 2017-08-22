#include "List.h"
#include "stdio.h"

#if 0

_UNKNOWN IList::operator[](int)::dummy; // weak
void *off_328 = (void *)0x235; // weak


//----- (00000000) --------------------------------------------------------
signed int __fastcall IList::CheckCapacity(IList *this, int a2, int a3, int a4)
{
  IList *v4; // r4@1
  signed int result; // r0@3
  int i; // r5@5
  int v7; // r6@8
  Buffer *v8; // r0@12
  int v9; // [sp+0h] [bp-28h]@1
  int v10; // [sp+4h] [bp-24h]@1
  int v11; // [sp+8h] [bp-20h]@1

  v9 = a2;
  v10 = a3;
  v11 = a4;
  v4 = this;
  if ( *((_DWORD *)this + 2) && *((_DWORD *)this + 4) >= a2 )
  {
    result = 1;
  }
  else
  {
    for ( i = 16; i < a2; i *= 2 )
      ;
    v7 = operator new[](4 * i);
    if ( v7 )
    {
      if ( *((_DWORD *)v4 + 3) > 0 && *((_DWORD *)v4 + 2) )
      {
        v8 = (Buffer *)Buffer::Buffer(&v9, v7, 4 * i);
        Buffer::Copy(v8, 0, *((const void **)v4 + 2), 4 * *((_DWORD *)v4 + 3));
      }
      if ( *((_DWORD *)v4 + 2) )
      {
        if ( *((IList **)v4 + 2) != (IList *)((char *)v4 + 20) )
          operator delete[](*((void **)v4 + 2));
      }
      *((_DWORD *)v4 + 2) = v7;
      *((_DWORD *)v4 + 4) = i;
      result = 1;
    }
    else
    {
      result = 0;
    }
  }
  return result;
}
// 344: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);

//----- (00000072) --------------------------------------------------------
int __fastcall IList::Add(int result, void **a2, int a3, int a4)
{
  int v4; // r4@1
  void **v5; // r5@1
  int v6; // r6@1
  int v7; // r1@6
  bool v8; // zf@5

  v4 = result;
  v5 = a2;
  v6 = a3;
  if ( a2 && a3 )
  {
    IList::CheckCapacity((IList *)result, *(_DWORD *)(result + 12) + a3, a3, a4);
    while ( 1 )
    {
      result = v6;
      v8 = v6-- == 0;
      if ( v8 )
        break;
      v7 = *(_DWORD *)(v4 + 12);
      *(_DWORD *)(v4 + 12) = v7 + 1;
      *(_DWORD *)(*(_DWORD *)(v4 + 8) + 4 * v7) = v5;
      ++v5;
    }
  }
  return result;
}

//----- (000000A6) --------------------------------------------------------
int __fastcall IList::Add(IList *this, void *a2, int a3, int a4)
{
  IList *v4; // r4@1
  void *v5; // r5@1
  int v6; // r1@1
  int result; // r0@1

  v4 = this;
  v5 = a2;
  IList::CheckCapacity(this, *((_DWORD *)this + 3) + 1, a3, a4);
  v6 = *((_DWORD *)v4 + 3);
  *((_DWORD *)v4 + 3) = v6 + 1;
  result = *((_DWORD *)v4 + 2);
  *(_DWORD *)(result + 4 * v6) = v5;
  return result;
}

//----- (000000C4) --------------------------------------------------------
int __fastcall IList::Init(int result)
{
  *(_DWORD *)(result + 8) = result + 20;
  *(_DWORD *)(result + 12) = 0;
  *(_DWORD *)(result + 16) = 4;
  *(_DWORD *)(result + 4) = 0;
  return result;
}

//----- (000000D8) --------------------------------------------------------
int __fastcall IList::move(IList *this, IList *a2, int a3, int a4)
{
  IList *v4; // r4@1
  IList *v5; // r5@1
  int v6; // r1@2
  int result; // r0@2
  IList *v8; // [sp+0h] [bp-20h]@1
  int v9; // [sp+4h] [bp-1Ch]@1
  int v10; // [sp+8h] [bp-18h]@1

  v8 = a2;
  v9 = a3;
  v10 = a4;
  v4 = this;
  v5 = a2;
  *((_DWORD *)this + 3) = *((_DWORD *)a2 + 3);
  *((_DWORD *)this + 4) = *((_DWORD *)a2 + 4);
  *((_DWORD *)this + 1) = *((_DWORD *)a2 + 1);
  if ( *((IList **)a2 + 2) == (IList *)((char *)a2 + 20) )
  {
    *((_DWORD *)this + 2) = (char *)this + 20;
    Buffer::Buffer(&v8, *((_DWORD *)this + 2), 4 * *((_DWORD *)this + 3));
    v6 = *((_DWORD *)v5 + 2);
    result = Buffer::operator=();
  }
  else
  {
    if ( *((_DWORD *)this + 2) )
    {
      if ( *((IList **)this + 2) != (IList *)((char *)this + 20) )
        operator delete[](*((void **)this + 2));
    }
    *((_DWORD *)v4 + 2) = *((_DWORD *)v5 + 2);
    result = IList::Init((int)v5);
  }
  return result;
}
// 344: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);
// 348: using guessed type int Buffer::operator=(void);

//----- (00000130) --------------------------------------------------------
int __fastcall IList::Clear(int result)
{
  *(_DWORD *)(result + 12) = 0;
  return result;
}

//----- (00000136) --------------------------------------------------------
int __fastcall IList::Remove(IList *this, const void *a2)
{
  IList *v2; // r5@1
  int v3; // r0@1
  int v4; // r2@1
  int v5; // r3@1
  int v6; // r4@1

  v2 = this;
  v3 = IList::FindIndex(this, a2);
  v6 = v3;
  if ( v3 >= 0 )
    IList::RemoveAt((int)v2, v3, v4, v5);
  return v6;
}

//----- (00000156) --------------------------------------------------------
int __fastcall IList::RemoveAt(int result, int a2, int a3, int a4)
{
  int v4; // r4@1
  int v5; // r5@1
  int v6; // r6@1
  int v7; // r7@4
  Buffer *v8; // r0@5
  int v9; // [sp+0h] [bp-28h]@1
  int v10; // [sp+4h] [bp-24h]@1
  int v11; // [sp+8h] [bp-20h]@1

  v9 = a2;
  v10 = a3;
  v11 = a4;
  v4 = result;
  v5 = a2;
  v6 = *(_DWORD *)(result + 12);
  if ( v6 > 0 && a2 < v6 )
  {
    v7 = v6 - 1 - a2;
    if ( v6 - 1 != a2 )
    {
      v8 = (Buffer *)Buffer::Buffer(&v9, *(_DWORD *)(result + 8) + 4 * a2, 4 * v7);
      Buffer::Copy(v8, 0, (const void *)(*(_DWORD *)(v4 + 8) + 4 * (v5 + 1)), 4 * v7);
    }
    result = *(_DWORD *)(v4 + 12) - 1;
    *(_DWORD *)(v4 + 12) = result;
  }
  return result;
}
// 344: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);

//----- (000001A2) --------------------------------------------------------
IList *__fastcall IList::DeleteAll(IList *this)
{
  IList *v1; // r4@1
  int i; // r5@1

  v1 = this;
  for ( i = 0; *((_DWORD *)v1 + 3) > i; ++i )
  {
    if ( *(_DWORD *)(*((_DWORD *)v1 + 2) + 4 * i) )
      operator delete(*(void **)(*((_DWORD *)v1 + 2) + 4 * i));
  }
  return v1;
}

//----- (000001C8) --------------------------------------------------------
IList *__fastcall IList::IList(IList *this, IList *a2)
{
  IList *v2; // r4@1
  IList *v3; // r5@1
  int v4; // r2@1
  int v5; // r3@1

  v2 = this;
  v3 = a2;
  *(_DWORD *)this = &off_328;
  IList::Init((int)this);
  IList::move(v2, v3, v4, v5);
  return v2;
}
// 328: using guessed type void *off_328;

//----- (000001E8) --------------------------------------------------------
IList *__fastcall IList::IList(IList *this, int a2)
{
  IList *v2; // r4@1
  int v3; // r5@1
  int v4; // r2@1
  int v5; // r3@1

  v2 = this;
  v3 = a2;
  *(_DWORD *)this = &off_328;
  IList::Init((int)this);
  IList::operator=(v2, v3, v4, v5);
  return v2;
}
// 328: using guessed type void *off_328;

//----- (00000208) --------------------------------------------------------
int __fastcall IList::IList(IList *this)
{
  int v1; // r2@1

  *(_DWORD *)this = &off_328;
  IList::Init((int)this);
  return v1;
}
// 328: using guessed type void *off_328;

//----- (00000220) --------------------------------------------------------
void __fastcall IList::~IList(IList *this)
{
  IList *v1; // r0@1

  v1 = IList::~IList(this);
  operator delete((void *)v1);
}

//----- (00000234) --------------------------------------------------------
IList *__fastcall IList::~IList(IList *this)
{
  IList *v1; // r4@1

  v1 = this;
  *(_DWORD *)this = &off_328;
  if ( *((_DWORD *)this + 2) && *((IList **)this + 2) != (IList *)((char *)this + 20) )
    operator delete[](*((void **)this + 2));
  return v1;
}
// 328: using guessed type void *;

//----- (00000258) --------------------------------------------------------
IList *__fastcall IList::operator=(IList *this, IList *a2, int a3, int a4)
{
  IList *v4; // r5@1

  v4 = this;
  if ( this != a2 )
    IList::move(this, a2, a3, a4);
  return v4;
}

//----- (0000026E) --------------------------------------------------------
IList *__fastcall IList::operator=(IList *this, int a2, int a3, int a4)
{
  IList *v4; // r5@1
  int v5; // r4@1
  int v6; // r1@4
  int v8; // [sp+0h] [bp-20h]@1
  int v9; // [sp+4h] [bp-1Ch]@1
  int v10; // [sp+8h] [bp-18h]@1

  v8 = a2;
  v9 = a3;
  v10 = a4;
  v4 = this;
  v5 = a2;
  if ( this != (IList *)a2 )
  {
    *((_DWORD *)this + 3) = *(_DWORD *)(a2 + 12);
    *((_DWORD *)this + 1) = *(_DWORD *)(a2 + 4);
    if ( *(_DWORD *)(a2 + 8) != a2 + 20 )
      IList::CheckCapacity(this, *(_DWORD *)(a2 + 12), a3, a4);
    Buffer::Buffer(&v8, *((_DWORD *)v4 + 2), 4 * *((_DWORD *)v4 + 3));
    v6 = *(_DWORD *)(v5 + 8);
    Buffer::operator=();
  }
  return v4;
}
// 344: using guessed type int __fastcall Buffer::Buffer(_DWORD, _DWORD, _DWORD);
// 348: using guessed type int Buffer::operator=(void);

//----- (000002AC) --------------------------------------------------------
void *__fastcall IList::operator[](int a1, int a2)
{
  void *result; // r0@3

  if ( a2 >= 0 && *(_DWORD *)(a1 + 12) > a2 )
    result = (void *)(*(_DWORD *)(a1 + 8) + 4 * a2);
  else
    result = &IList::operator[](int)::dummy;
  return result;
}

//----- (000002C8) --------------------------------------------------------
signed int __fastcall IList::FindIndex(IList *this, const void *a2)
{
  IList *v2; // r4@1
  const void *v3; // r6@1
  int i; // r5@1

  v2 = this;
  v3 = a2;
  for ( i = 0; *((_DWORD *)v2 + 3) > i; ++i )
  {
    if ( *(const void **)(*((_DWORD *)v2 + 2) + 4 * i) == v3 )
      return i;
    if ( *((_DWORD *)v2 + 1)
      && !(*((int (__fastcall **)(_DWORD, _DWORD))v2 + 1))(*(_DWORD *)(*((_DWORD *)v2 + 2) + 4 * i), v3) )
    {
      return i;
    }
  }
  return -1;
}

//----- (00000304) --------------------------------------------------------
int __fastcall IList::operator[](int a1, int a2)
{
  int result; // r0@3

  if ( a2 >= 0 && *(_DWORD *)(a1 + 12) > a2 )
    result = *(_DWORD *)(*(_DWORD *)(a1 + 8) + 4 * a2);
  else
    result = 0;
  return result;
}
#endif


































IList::IList(){}
IList::IList(const IList &list){

}
IList::IList(IList && list){}
IList::~IList(){}
IList &IList::operator = (const IList &list){

}
IList &IList::operator = (IList && list){

}
// 添加单个元素
void IList::Add(void *item)
{
    return ;
}

// 添加多个元素
void IList::Add(void **items, int count)
{
    return ;
}

// 删除指定位置元素
void IList::RemoveAt(int index)
{
    return ;
}

// 删除指定元素
int IList::Remove(const void *item)
{
    return 0;
}

void IList::Clear()
{
    return ;
}

// 查找指定项。不存在时返回-1
int IList::FindIndex(const void *item)const
{
    return 0;
}

// 释放所有指针指向的内存
IList &IList::DeleteAll(){}

// 重载索引运算符[]，返回指定元素的第一个
void *IList::operator[](int i)const{}
void * &IList::operator[](int i){

}

#if DEBUG
    void IList::Test()
    {
        return ;
    }
#endif 
void IList::Init()
{
    return ;
}

bool IList::CheckCapacity(int count)
{
	if(count<=this->_Capacity)
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

//#pragma once

//#include "Type.h"
//#include <stdio.h>
//template<typename T> class IList
//{
//	public:
//		IList();
//		void Add(T&);
//		void Add(T*);
//		int Count();
//		int FindIndex(T&);
//		void Remove(T&);
//		void RemoveAt(int pos=-1);
//		bool MoveNext(int pos);//到下一个节点
//		T* Find(T&);
//		T *operator[](int id);
//		void PrintList();
//		~IList();
//	protected:
//		struct Node{
//			Node* pNext;
//			T* pT;
//		};
//		Node *pFirst;//链首节点指针
//		Node *mpCur;//当前节点
//		int count;
//};

//template <typename T> 
//int IList<T>::Count()
//{
//	return this->count;
//}

//template <typename T> 
//IList<T>::IList()
//{
//        this->pFirst = NULL;
//		this->count=0;
//}
//template <typename T> 
//void IList<T>::Add(T &t)
//{
//        Node *temp = new Node();
//        temp->pT = &t;
//        temp->pNext = pFirst;
//        pFirst = temp;
//		mpCur=temp;
//		this->count++;
//}

//template <typename T> 
//void IList<T>::Add(T *t)
//{
//        Node *temp = new Node();
//        temp->pT = t;
//        temp->pNext = pFirst;
//        pFirst = temp;
//		this->count++;
//}

//template <typename T> void IList<T>::Remove(T &t)
//{
//        Node *q = 0; //用来定位待删除的节点
//        if (*(pFirst->pT) == t)
//        {
//                q = pFirst;
//                pFirst = pFirst->pNext; //待删除节点在链首时的脱链
//        }
//        else
//        {
//                for (Node *p = pFirst; p->pNext; p = p->pNext)
//                //顺链查找
//                {
//                        if (*(p->pNext->pT) == t)
//                        {
//                                q = p->pNext;
//                                p->pNext = q->pNext;
//                                break;
//                        }
//                }
//        }
//        if (q)
//        {
//                delete q->pT;
//                delete q;
//        }
//		this->count--;
//}
//template <typename T> void IList<T>::RemoveAt(int pos)
//{
//	if(pos==-1)
//	{
//		this->Remove(*(mpCur->pT));
//	}
//}
//template<typename T>
//T* IList<T>::Find(T& t)
//{
//	for(Node* p=pFirst;p;p=p->pNext)
//	{
//		if(*(p->pT)==t)
//		{
//			return p->pT;
//		}
//	}
//	return 0;
//}
//template<typename T>
//bool IList<T>::MoveNext(int pos)
//{
//	if(pos<0)
//	{	
//		if(mpCur)
//		{
//			mpCur=mpCur->pNext;
//			if(mpCur)
//			{
//				return true;
//			}
//			else
//			{				
//				return false;
//			}
//		}
//		else
//		{
//			mpCur=pFirst;
//			return true;
//		}
//	}
//	
//	
//	int i=0;
//	for(Node* p=pFirst;p;p=p->pNext,i++)
//	{
//		if(p&&(i==pos))
//		{
//			return true;
//		}
//	}
//	return false;
//}
//template<typename T>
//T* IList<T>::operator[](int taskid)
//{	
//	if(taskid<0)
//	{
//		return this->mpCur->pT;
//	}
//	int i=0;
//	for(Node* p=pFirst;p;p=p->pNext,i++)
//	{
//		if(p&&(i==taskid))
//		{
//			return p->pT;
//		}
//	}	
//	return 0;
//}
//template<typename T>
//void IList<T>::PrintList()
//{	
//	for(Node* p=pFirst;p;p=p->pNext)
//	{
//			printf("%f\n",*(p->pT));		
//	}
//}

//template<typename T>
//IList<T>::~IList<T>()
//{
//	Node *p;
//	p=pFirst;
//	while(p)
//	{
//		pFirst=p->pNext;
//		delete p->pT;
//		delete p;
//		p=pFirst;
//	}
//}
//// 变长列表模版
//template <typename T> class List
//{
//    public:
//        List()
//        {
//                _count = _total = 0;
//        }
//        List(int size = 4)
//        {
//                _count = 0;
//                _total = size;
//                arr = new T[size];
//        }
//        List(T *items, uint count)
//        {
//                arr = new T[count];

//                _count = 0;
//                _total = count;
//                for (int i = 0; i < count; i++)
//                {
//                        arr[_count++] =  *items++;
//                }
//        }

//        ~List()
//        {
//                if (arr)
//                    delete [] arr;
//                arr = NULL;
//        }

//        void Add(T item)
//        {
//                // 检查大小
//                CheckSize();

//                arr[_count++] = item;
//        }

//        void Add(T *items, int count)
//        {
//                int size = _count + count;
//                if (size >= _total)
//                    ChangeSize(size *2);

//                for (int i = 0; i < count; i++)
//                {
//                        arr[_count++] =  *items++;
//                }
//        }

//        T *ToArray()
//        {
//                // 如果刚好完整则直接返回，否则重新调整空间
//                if (_count != _total)
//                {
//                        T *arr2 = new T[_count];
//                        memcpy(arr, arr2, _count);
//                        delete [] arr;
//                        arr = arr2;
//                }
//                return arr;
//        }
//        int Count()
//        {
//                return _count;
//        }

//        // 重载索引运算符[]，让它可以像数组一样使用下标索引。内部不检查下标越界，外部好自为之
//        T operator[](int i)
//        {
//                return arr;
//        }
//        T *operator = (List list)
//        {
//                return list.ToArray();
//        }

//    private:
//        T *arr;
//        uint _count;
//        uint _total;

//        void ChangeSize(int newSize)
//        {
//                if (_total == newSize)
//                    return ;

//                T *arr2 = new T[newSize];
//                if (arr)
//                {
//                        // 如果新数组较小，则直接复制；如果新数组较大，则先复制，再清空余下部分
//                        if (newSize < _total)
//                            memcpy(arr, arr2, newSize);
//                        else
//                        {
//                                memcpy(arr, arr2, _total);
//                                memset(arr2 + _total, newSize - _total);
//                        }
//                        delete [] arr;
//                }
//                arr = arr2;
//        }

//        void CheckSize()
//        {
//                // 如果数组空间已用完，则两倍扩容
//                if (_count >= _total)
//                    ChangeSize(_count *2);
//        }
//};
