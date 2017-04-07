#ifndef _LinkedList_H_
#define _LinkedList_H_

// 双向链表
template <class T> class LinkedList;

// 双向链表节点。实体类继承该类
template <class T> class LinkedNode
{
	// 友元类。允许链表类控制本类私有成员
    friend class LinkedList<T>;

public:
    T* Prev;	// 上一个节点
    T* Next;	// 下一个节点

    void Initialize()
    {
        Next = nullptr;
        Prev = nullptr;
    }

    // 从链表中删除。需要修改前后节点的指针指向，但当前节点仍然指向之前的前后节点
    void RemoveFromList()
    {
        if(Prev) Prev->Next = Next;
        if(Next) Next->Prev = Prev;
    }

	// 完全脱离链表。不再指向其它节点
    void Unlink()
    {
        if(Prev) Prev->Next = Next;
        if(Next) Next->Prev = Prev;

        Next = nullptr;
        Prev = nullptr;
    }

	// 把当前节点附加到另一个节点之后
	void LinkAfter(T* node)
	{
		node->Next = (T*)this;
		Prev = node;
		// 不能清空Next，因为可能是两个链表的合并
		//Next = nullptr;
	}

	// 最后一个节点
	T* Last()
	{
		T* node = (T*)this;
		while(node->Next) node = node->Next;

		return node;
	}

	// 附加到当前节点后面
	void Append(T* node)
	{
		Next = node;
		node->Prev = (T*)this;
		//node->Next = nullptr;
	}
};

#endif
