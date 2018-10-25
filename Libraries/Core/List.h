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
   	
	inline int Count()	const { return _Count; }

	// ��ӵ���Ԫ��
    void Add(void* item);

	// ��Ӷ��Ԫ��
    void Add(void** items, int count);

	// ɾ��ָ��λ��Ԫ��
	void RemoveAt(int index);

	// ɾ��ָ��Ԫ��
	int Remove(const void* item);
	
	// ����ָ���������ʱ����-1
	int FindIndex(const void* item) const;
	
    // �������������[]������ָ��Ԫ�صĵ�һ��
    void* operator[](int i) const;
	
private:
	void**	_Arr;
	int		_Count;
	int		_Capacity;

	void*	Arr[0x04];
	char tmpbuf[4 * 30];
private:
	void Init();
	bool CheckCapacity(int count);
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
};

#endif
