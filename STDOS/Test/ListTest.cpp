#include "List.h"
#include <stdio.h>
#include "Sys.h"

class TaskTest
{
	public:
		int aa;
		int bb;
	void show()
	{
		debug_printf("aa:%d,bb:%d \r\n",aa,bb);
	}
};


#if DEBUG
	#define SmartOS_printf debug_printf
    void IList::Test()
    {
//		TraceStack ts("TestList");
//		SmartOS_printf("TestList......\r\n");		
//		IList list;
		
		debug_printf("\r\n\r\n TestList begin");
		List<TaskTest*>	_Tasks;	
		
		debug_printf("\r\n\r\n TestList end\r\n\r\n");
    }
#endif 
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

