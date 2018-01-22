#ifndef _Queue_H_
    #define _Queue_H_

    #include "Array.h"
#define _USEARRAY 0
    // ����
    // ����һ�����������α�λ�ã����ݳ��ȡ�ʵ����Ч���ݽ�ռ�û������м䲿�֣�ͷβ��������ʣ��
    class Queue
    {
#if _USEARRAY
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
#else
	private:
		byte * pBuf; 	/* ������ */
		int _write; 	/* ������дָ�� */
		int _capacity; 	/* ��������С */
		int _read; 		/* ��������ָ�� */
		int _count; 	/* ���ݸ��� */
	public:
		Queue();
		void SetBuf(void *buf, int len);							// ���û�����
		bool Empty()const { return _count == 0; } 						// ���п�
		int Capacity()const { return _capacity; } 						// ��������
		bool Full()const { return this->_count >= this->_capacity; } 	// ������
		int Length()const { return _count; } 							// ���д�С
		void Clear();

		int Enqueue(byte da);
		byte Dequeue();

		int Write(const Buffer &bs); 								// ����д��
		int Read(Buffer &bs); 										// ������ȡ
#endif
    };
#endif
