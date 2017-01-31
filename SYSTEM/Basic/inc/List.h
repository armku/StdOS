#pragma once

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

