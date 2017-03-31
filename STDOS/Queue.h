#ifndef _Queue_H_
#define _Queue_H_

#include "Array.h"

// ����
// ����һ�����������α�λ�ã����ݳ��ȡ�ʵ����Ч���ݽ�ռ�û������м䲿�֣�ͷβ��������ʣ��
class Queue
{
private:
	Array _s;	// ������
	int _head;		// ͷ��λ��
    int _tail;		// β��λ��
	int _size;		// ����

public:
	Queue();

	bool Empty() const { return _size == 0; }	// ���п�
	int Capacity() const { return _s.Capacity(); }	// ��������
	int Length() const { return _size; }		// ���д�С
	void SetCapacity(int len);

	void Clear();

	void Enqueue(byte dat);
	byte Dequeue();

	int Write(const Buffer& bs);	// ����д��
	int Read(Buffer& bs);		// ������ȡ
};

#endif
