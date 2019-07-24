#include "Type.h"
#include "Buffer.h"
#include "List.h"

IList::IList()
{
	Init();
}

IList::IList(const IList& list)
{
	Init();

	operator=(list);
}

IList::IList(IList&& list)
{
	Init();

	move(list);
}

IList::~IList()
{
	if (_Arr && _Arr != Arr) delete[] _Arr;
}

void IList::Init()
{
	_Arr = Arr;
	_Count = 0;
	_Capacity = ArrayLength(Arr);

	Comparer = nullptr;
}

IList& IList::operator=(const IList& list)
{
	if (this != &list)
	{
		_Count = list._Count;
		Comparer = list.Comparer;

		if (list._Arr != list.Arr) CheckCapacity(list._Count);

		Buffer(_Arr, _Count << 2) = list._Arr;
	}

	return *this;
}

IList& IList::operator=(IList&& list)
{
	if (this != &list) move(list);

	return *this;
}

void IList::move(IList& list)
{
	_Count = list._Count;
	_Capacity = list._Capacity;
	Comparer = list.Comparer;

	// ���list�Ļ��������Լ��ģ��򿽱�����
	// ��������Լ��ģ���ֱ���ù�����
	if (list._Arr == list.Arr)
	{
		_Arr = Arr;
		Buffer(_Arr, _Count << 2) = list._Arr;
	}
	else
	{
		// ������������������ͷ�
		if (_Arr && _Arr != Arr) delete[] _Arr;

		_Arr = list._Arr;
		list.Init();
	}
}

//int IList::Count() const { return _Count; }

// ��ӵ���Ԫ��
void IList::Add(void* item)
{
	CheckCapacity(_Count + 1);

	_Arr[_Count++] = item;
}

// ��Ӷ��Ԫ��
void IList::Add(void** items, int count)
{
	if (!items || !count) return;

	CheckCapacity(_Count + count);

	while (count--) _Arr[_Count++] = items++;
}

// ɾ��ָ��λ��Ԫ��
void IList::RemoveAt(int index)
{
	int len = _Count;
	if (len <= 0 || index >= len) return;

	// ����Ԫ�أ����һ�����ø���
	int remain = len - 1 - index;
	if (remain)
	{
		len = remain * sizeof(void*);
		Buffer(&_Arr[index], len).Copy(0, &_Arr[index + 1], len);
	}
	_Count--;
}

// ɾ��ָ��Ԫ��
int IList::Remove(const void* item)
{
	int idx = FindIndex(item);
	if (idx >= 0) RemoveAt(idx);

	return idx;
}

int IList::FindIndex(const void* item) const
{
	for (int i = 0; i < _Count; i++)
	{
		if (_Arr[i] == item) return i;
		if (Comparer && Comparer(_Arr[i], item) == 0) return i;
	}

	return -1;
}

// �ͷ�����ָ��ָ����ڴ�
IList& IList::DeleteAll()
{
	for (int i = 0; i < _Count; i++)
	{
		if (_Arr[i]) delete (int*)_Arr[i];
	}

	return *this;
}

void IList::Clear()
{
	_Count = 0;
}

// �������������[]������ָ��Ԫ�صĵ�һ��
void* IList::operator[](int i) const
{
	if (i < 0 || i >= _Count) return nullptr;

	return _Arr[i];
}

void*& IList::operator[](int i)
{
	if (i < 0 || i >= _Count)
	{
		static void* dummy;
		return dummy;
	}

	return _Arr[i];
}

bool IList::CheckCapacity(int count)
{
	// �Ƿ񳬳�����
	if (_Arr && count <= _Capacity) return true;

	// �Զ�������ʵ�����
	int sz = 0x40 >> 2;
	while (sz < count) sz <<= 1;

	void* p = new byte[sz << 2];
	if (!p) return false;

	// ��Ҫ��������
	if (_Count > 0 && _Arr)
		// Ϊ�˰�ȫ�������ֽڿ���
		Buffer(p, sz << 2).Copy(0, _Arr, _Count << 2);

	if (_Arr && _Arr != Arr) delete[] _Arr;

	_Arr = (void**)p;
	_Capacity = sz;

	return true;
}
