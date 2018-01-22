#ifndef _Queue_H_
    #define _Queue_H_

    #include "Array.h"
#define _USEARRAY 0
    // ����
    // ����һ�����������α�λ�ã����ݳ��ȡ�ʵ����Ч���ݽ�ռ�û������м䲿�֣�ͷβ��������ʣ��
    class Queue
    {
	private:
		int _size;		// ����
#if _USEARRAY
		Array _s;	// ������
		int _head;		// ͷ��λ��
		int _tail;		// β��λ��
		
#else
		byte * pBuf; 	/* ������ */
		int _write; 	/* ������дָ�� */
		int _capacity; 	/* ��������С */
		int _read; 		/* ��������ָ�� */
#endif
	public:
		Queue();
		void Clear();
		int Length() const { return _size; }		// ���д�С
		bool Empty() const { return _size == 0; }	// ���п�

		int Write(const Buffer& bs);	// ����д��
		int Read(Buffer& bs);		// ������ȡ 
		byte Dequeue();
#if _USEARRAY		
		int Capacity() const { return _s.Capacity(); }	// ��������		
		void SetCapacity(int len);
		void Enqueue(byte dat);		   	
#else
		void SetBuf(void *buf, int len);							// ���û�����		
		int Capacity()const { return _capacity; } 						// ��������
		bool Full()const { return this->_size >= this->_capacity; } 	// ������		
		int Enqueue(byte da);		
#endif
    };
#endif
