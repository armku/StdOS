#include "List.h"

template<class T> List<T>::List()
{
	this->pFirst=0;
}

template<class T> 
void	List<T>::Add(T& t)
{
	Node *temp=new Node();
	temp->pT=&t;
	temp->pNext=pFirst;
	pFirst=temp;
}

template<class T>
	void List<T>::Remove(T& t)
	{
		Node *q=0;//用来定位待删除的节点
		if(*(pFirst->pT)==t)
		{
			q=pFirst;
			pFirst=pFirst->pNext;//待删除节点在链首时的脱链
		}
		else
		{
			for(Node *p=pFirst;p->pNext;p=p->pNext)//顺链查找
			{
				if(*(p->pNext->pT)==t)
				{
					q=p->pNext;
					p->pNext=q->pNext;
					break;
				}
			}
		}
		if(q)
		{
			delete q->pT;
			delete q;
		}
	}

