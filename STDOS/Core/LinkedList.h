#ifndef _LinkedList_H_
#define _LinkedList_H_

// ˫������
template <class T> class LinkedList;

// ˫������ڵ㡣ʵ����̳и���
template <class T> class LinkedNode
{
	// ��Ԫ�ࡣ������������Ʊ���˽�г�Ա
    friend class LinkedList<T>;

public:
    T* Prev;	// ��һ���ڵ�
    T* Next;	// ��һ���ڵ�

    void Initialize()
    {
        Next = nullptr;
        Prev = nullptr;
    }

    // ��������ɾ������Ҫ�޸�ǰ��ڵ��ָ��ָ�򣬵���ǰ�ڵ���Ȼָ��֮ǰ��ǰ��ڵ�
    void RemoveFromList()
    {
        if(Prev) Prev->Next = Next;
        if(Next) Next->Prev = Prev;
    }

	// ��ȫ������������ָ�������ڵ�
    void Unlink()
    {
        if(Prev) Prev->Next = Next;
        if(Next) Next->Prev = Prev;

        Next = nullptr;
        Prev = nullptr;
    }

	// �ѵ�ǰ�ڵ㸽�ӵ���һ���ڵ�֮��
	void LinkAfter(T* node)
	{
		node->Next = (T*)this;
		Prev = node;
		// �������Next����Ϊ��������������ĺϲ�
		//Next = nullptr;
	}

	// ���һ���ڵ�
	T* Last()
	{
		T* node = (T*)this;
		while(node->Next) node = node->Next;

		return node;
	}

	// ���ӵ���ǰ�ڵ����
	void Append(T* node)
	{
		Next = node;
		node->Prev = (T*)this;
		//node->Next = nullptr;
	}
};

/*// ˫������
template <class T>
class LinkedList
{
public:
    // ����ڵ㡣ʵ���಻��Ҫ�̳и��ڲ���
	class Node
	{
	public:
		T		Item;	// Ԫ��
		Node*	Prev;	// ǰһ�ڵ�
		Node*	Next;	// ��һ�ڵ�

		Node()
		{
			Prev = nullptr;
			Next = nullptr;
		}

		// �Ӷ���������
		void RemoveFromList()
		{
			// ˫���գ�ֻ����ǰ��ڵ�ָ��ǰ�ڵ�ʱ�����޸����ǵ�ֵ
			if(Prev && Prev->Next == this) Prev->Next = Next;
			if(Next && Next->Prev == this) Next->Prev = Prev;
		}

		// ���ӵ���ǰ�ڵ����
		void Append(Node* node)
		{
			Next = node;
			node->Prev = this;
		}
	};

private:
	Node*	_Head;		// ����ͷ��
    Node*	_Tail;		// ����β��
	int 	_Count;		// Ԫ�ظ���

	void Init()
	{
        _Head = nullptr;
		_Tail = nullptr;
		_Count = 0;
	}

	void Remove(Node* node)
	{
		// �������
		node->RemoveFromList();
		// ���⴦��ͷβ
		if(node == _Head) _Head = node->Next;
		if(node == _Tail) _Tail = node->Prev;

		delete node;
		_Count--;
	}

public:
    LinkedList()
    {
		Init();
    }

	// ��������ڵ���
    virtual int Count() const { return _Count; }

	// ��ĳ����ӵ�����
	virtual void Add(const T& item)
	{
		Node* node = new Node();
		node->Item = item;

		if(_Tail)
			_Tail->Append(node);
		else
			_Head = _Tail = node;

		_Count++;
	}

	// �Ӽ������Ƴ��ض�����ĵ�һ��ƥ����
	virtual void Remove(const T& item)
	{
		if(!_Count) return;

		Node* node;
		for(node = _Head; node; node = node->Next)
		{
			if(node->Item == item) break;
		}

		if(node) Remove(node);
	}

	// ȷ�������Ƿ�����ض�ֵ
	virtual bool Contains(const T& item)
	{
		if(!_Count) return false;

		Node* node;
		for(node = _Head; node; node = node->Next)
		{
			if(node->Item == item) return true;
		}

		return false;
	}

	// �Ӽ������Ƴ������ע�⣬�÷��������ͷ�Ԫ��ָ��
	virtual void Clear()
	{
		if(!_Count) return;

		Node* node;
		for(node = _Head; node;)
		{
			// �ȱ��ݣ�����ɾ������Ӱ��ָ��
			Node* next = node->Next;
			delete node;
			node = next;
		}

		Init();
	}

	// ������Ԫ�ظ��Ƶ�������
	virtual void CopyTo(T* arr)
	{
		if(!_Count) return;

		Node* node;
		for(node = _Head; node; node = node->Next)
		{
			*arr++ = node->Item;
		}
	}

	T& First() { return _Head->Item; }
	T& Last() { return _Tail->Item; }

	// �ͷŵ�һ����Ч�ڵ�
    T& ExtractFirst()
    {
		if(!_Count) return nullptr;

        Node* node = _Head;
        _Head = _Head->Next;
		// ����ֻ��һ���ڵ�
		if(!_Head)
			_Tail = nullptr;
		else
			_Head->Prev = nullptr;

		T& item = node->Item;
		delete node;
		_Count--;

        return item;
    }

	// �ͷ����һ����Ч�ڵ�
    T& ExtractLast()
    {
		if(!_Count) return nullptr;

        Node* node = _Tail;
        _Tail = _Tail->Prev;
		// ����ֻ��һ���ڵ�
		if(!_Tail)
			_Head = nullptr;
		else
			_Tail->Next = nullptr;

		T& item = node->Item;
		delete node;
		_Count--;

        return item;
    }
};*/

#endif
