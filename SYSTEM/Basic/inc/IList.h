#pragma once

#include "Type.h"

template<typename T> class IList
{
	public:
		IList();
		void Add(T&);
		void Add(T*);
		int Count();
		int FindIndex(T&);
		void Remove(T&);
		void RemoveAt(int pos=-1);
		bool MoveNext(int pos);//����һ���ڵ�
		T* Find(T&);
		T *operator[](int id);
		void PrintList();
		~IList();
	protected:
		struct Node{
			Node* pNext;
			T* pT;
		};
		Node *pFirst;//���׽ڵ�ָ��
		Node *mpCur;//��ǰ�ڵ�
		int count;
};

template <typename T> 
int IList<T>::Count()
{
	return this->count;
}

template <typename T> 
IList<T>::IList()
{
        this->pFirst = 0;
		this->count=0;
}
template <typename T> 
void IList<T>::Add(T &t)
{
        Node *temp = new Node();
        temp->pT = &t;
        temp->pNext = pFirst;
        pFirst = temp;
		mpCur=temp;
		this->count++;
}

template <typename T> 
void IList<T>::Add(T *t)
{
        Node *temp = new Node();
        temp->pT = t;
        temp->pNext = pFirst;
        pFirst = temp;
		this->count++;
}

template <typename T> void IList<T>::Remove(T &t)
{
        Node *q = 0; //������λ��ɾ���Ľڵ�
        if (*(pFirst->pT) == t)
        {
                q = pFirst;
                pFirst = pFirst->pNext; //��ɾ���ڵ�������ʱ������
        }
        else
        {
                for (Node *p = pFirst; p->pNext; p = p->pNext)
                //˳������
                {
                        if (*(p->pNext->pT) == t)
                        {
                                q = p->pNext;
                                p->pNext = q->pNext;
                                break;
                        }
                }
        }
        if (q)
        {
                delete q->pT;
                delete q;
        }
		this->count--;
}
template <typename T> void IList<T>::RemoveAt(int pos)
{
	if(pos==-1)
	{
		this->Remove(*(mpCur->pT));
	}
}
template<typename T>
T* IList<T>::Find(T& t)
{
	for(Node* p=pFirst;p;p=p->pNext)
	{
		if(*(p->pT)==t)
		{
			return p->pT;
		}
	}
	return 0;
}
template<typename T>
bool IList<T>::MoveNext(int pos)
{
	if(pos<0)
	{	
		if(mpCur)
		{
			mpCur=mpCur->pNext;
			if(mpCur)
			{
				return true;
			}
			else
			{				
				return false;
			}
		}
		else
		{
			mpCur=pFirst;
			return true;
		}
	}
	
	
	int i=0;
	for(Node* p=pFirst;p;p=p->pNext,i++)
	{
		if(p&&(i==pos))
		{
			return true;
		}
	}
	return false;
}
template<typename T>
T* IList<T>::operator[](int taskid)
{	
	if(taskid<0)
	{
		return this->mpCur->pT;
	}
	int i=0;
	for(Node* p=pFirst;p;p=p->pNext,i++)
	{
		if(p&&(i==taskid))
		{
			return p->pT;
		}
	}	
	return 0;
}
template<typename T>
void IList<T>::PrintList()
{	
	for(Node* p=pFirst;p;p=p->pNext)
	{
			printf("%f\n",*(p->pT));		
	}
}

template<typename T>
IList<T>::~IList<T>()
{
	Node *p;
	p=pFirst;
	while(p)
	{
		pFirst=p->pNext;
		delete p->pT;
		delete p;
		p=pFirst;
	}
}
