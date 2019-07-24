#ifndef __Heap_H__
#define __Heap_H__

// �ѹ���
class Heap
{
public:
	uint	Address;// ��ʼ��ַ
	int		Size;	// ��С

	Heap(uint addr, int size);

	int Used() const;	// ��ʹ���ڴ���
	int Count() const;	// ��ʹ���ڴ����
	int FreeSize() const;	// �����ڴ���

	void* Alloc(int size);
	void Free(void* ptr);

	// ��ǰ��
	static const Heap* Current;

private:
	int		_Used;
	int		_Count;
	void* _First;	// ��һ���п��е��ڴ�飬��������
};

#endif
