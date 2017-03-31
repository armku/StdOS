#pragma once

template<class T,int length> class FixedArray
{
	public:
		FixedArray();
		void Add(T&);
		void Add(T*);
		void Remove(T&);
		void RemoveAt(int pos=-1);
		bool MoveNext(int pos);//到下一个节点
		T* Find(T&);
		T *operator[](int taskid);
		void PrintList();
		~FixedArray();
	protected:
		struct Node{
			Node* pNext;
			T* pT;
		};
		Node *pFirst;//链首节点指针
		Node *mpCur;//当前节点
};


template <class T,int length> 
FixedArray<T,length>::FixedArray()
{
        this->pFirst = 0;
}
template <class T,int length> 
void FixedArray<T,length>::Add(T &t)
{
        Node *temp = new Node();
        temp->pT = &t;
        temp->pNext = pFirst;
        pFirst = temp;
		mpCur=temp;
}

template <class T,int length> 
void FixedArray<T,length>::Add(T *t)
{
        Node *temp = new Node();
        temp->pT = t;
        temp->pNext = pFirst;
        pFirst = temp;
}

template <class T,int length> void FixedArray<T,length>::Remove(T &t)
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
template <class T,int length> void FixedArray<T,length>::RemoveAt(int pos)
{
	if(pos==-1)
	{
		this->Remove(*(mpCur->pT));
	}
}
template<class T,int length>
T* FixedArray<T,length>::Find(T& t)
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
template<class T,int length>
bool FixedArray<T,length>::MoveNext(int pos)
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
template<class T,int length>
T* FixedArray<T,length>::operator[](int taskid)
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
template<class T,int length>
void FixedArray<T,length>::PrintList()
{	
	for(Node* p=pFirst;p;p=p->pNext)
	{
			printf("%f\n",*(p->pT));		
	}
}

template<class T,int length>
FixedArray<T,length>::~FixedArray<T,length>()
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
