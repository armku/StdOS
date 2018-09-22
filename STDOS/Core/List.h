#ifndef _List_H_
#define _List_H_
#include "Type.h"

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
	void**	_Arr;			//	8
	int		_Count;//12
	int		_Capacity;//16      

	void*	Arr[0x04];//20

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
