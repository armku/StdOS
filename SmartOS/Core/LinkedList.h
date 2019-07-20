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

#endif
