#pragma once
#include <stdio.h>

template<class T> class List
{
	public:
		List();
		void Add(T&);
		void Remove(T&);
		T* Find(T&);
		void PrintList();
		~List();
	protected:
		struct Node{
			Node* pNext;
			T* pT;
		};
		Node *pFirst;//链首节点指针
};
template <class T> List<T>::List()
{
        this->pFirst = 0;
}

template <class T> void List<T>::Add(T &t)
{
        Node *temp = new Node();
        temp->pT = &t;
        temp->pNext = pFirst;
        pFirst = temp;
}

template <class T> void List<T>::Remove(T &t)
{
        Node *q = 0; //用来定位待删除的节点
        if (*(pFirst->pT) == t)
        {
                q = pFirst;
                pFirst = pFirst->pNext; //待删除节点在链首时的脱链
        }
        else
        {
                for (Node *p = pFirst; p->pNext; p = p->pNext)
                //顺链查找
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
}
template<class T>
T* List<T>::Find(T& t)
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
template<class T>
void List<T>::PrintList()
{
	int i=0;
	for(Node* p=pFirst;p;p=p->pNext)
	{
			printf("%d\n",i++);		
	}
}

template<class T>
List<T>::~List<T>()
{
	Node *p;
	while(p=pFirst)
	{
		pFirst=pFirst->pNext;
		delete p->pT;
		delete p;
	}
}


