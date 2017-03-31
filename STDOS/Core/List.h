#ifndef _List_H_
#define _List_H_

typedef int (*IComparer)(const void* v1, const void* v2);

// �䳤�б������ڴ洢ָ��
class IList
{
public:
    IComparer	Comparer;	// �Ƚ���

	IList();
    IList(const IList& list);
    IList(IList&& list);
	virtual ~IList();

	IList& operator=(const IList& list);
	IList& operator=(IList&& list);

	inline int Count()	const { return _Count; }

	// ��ӵ���Ԫ��
    void Add(void* item);

	// ��Ӷ��Ԫ��
    void Add(void** items, int count);

	// ɾ��ָ��λ��Ԫ��
	void RemoveAt(int index);

	// ɾ��ָ��Ԫ��
	int Remove(const void* item);

	void Clear();

	// ����ָ���������ʱ����-1
	int FindIndex(const void* item) const;

	// �ͷ�����ָ��ָ����ڴ�
	IList& DeleteAll();

    // �������������[]������ָ��Ԫ�صĵ�һ��
    void* operator[](int i) const;
    void*& operator[](int i);

#if DEBUG
	static void Test();
#endif

private:
	void**	_Arr;
	int		_Count;
	int		_Capacity;

	void*	Arr[0x04];

	void Init();
	bool CheckCapacity(int count);
	void move(IList& list);
};

template<typename T>
class List : public IList
{
	static_assert(sizeof(T) <= 4, "List only support pointer or int");
public:
	virtual ~List() { };

	// ��ӵ���Ԫ��
    void Add(T item) { IList::Add((void*)item); }

	// ɾ��ָ��Ԫ��
	int Remove(const T item) { return IList::Remove((const void*)item); }

	// ����ָ���������ʱ����-1
	int FindIndex(const T item) const { return IList::FindIndex((const void*)item); }

    // �������������[]������ָ��Ԫ�صĵ�һ��
    T operator[](int i) const	{ return (T)IList::operator[](i); }
    T& operator[](int i)		{ return (T&)IList::operator[](i); }
};

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
//		bool MoveNext(int pos);//����һ���ڵ�
//		T* Find(T&);
//		T *operator[](int id);
//		void PrintList();
//		~IList();
//	protected:
//		struct Node{
//			Node* pNext;
//			T* pT;
//		};
//		Node *pFirst;//���׽ڵ�ָ��
//		Node *mpCur;//��ǰ�ڵ�
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
//        Node *q = 0; //������λ��ɾ���Ľڵ�
//        if (*(pFirst->pT) == t)
//        {
//                q = pFirst;
//                pFirst = pFirst->pNext; //��ɾ���ڵ�������ʱ������
//        }
//        else
//        {
//                for (Node *p = pFirst; p->pNext; p = p->pNext)
//                //˳������
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
//// �䳤�б�ģ��
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
//                // ����С
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
//                // ����պ�������ֱ�ӷ��أ��������µ����ռ�
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

//        // �������������[]����������������һ��ʹ���±��������ڲ�������±�Խ�磬�ⲿ����Ϊ֮
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
//                        // ����������С����ֱ�Ӹ��ƣ����������ϴ����ȸ��ƣ���������²���
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
//                // �������ռ������꣬����������
//                if (_count >= _total)
//                    ChangeSize(_count *2);
//        }
//};
