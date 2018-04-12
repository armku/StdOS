#ifndef _Queue_H_
    #define _Queue_H_

    #include "Array.h"
    // ����
    // ����һ�����������α�λ�ã����ݳ��ȡ�ʵ����Ч���ݽ�ռ�û������м䲿�֣�ͷβ��������ʣ��
    class Queue
    {
	private:
		int _size;		// ����
		int _head;		// ͷ��λ��
		int _tail;		// β��λ��
		uint8_t * pBuf; 	/* ������ */
		int _capacity; 	/* ��������С */
	public:
		Queue();
		void Clear();
		int Length() const { return _size; }		// ���д�С
		bool Empty() const { return _size == 0; }	// ���п�

		int Write(const Buffer& bs);	// ����д��
		int Read(Buffer& bs);		// ������ȡ 
		void Enqueue(uint8_t dat);
		uint8_t Dequeue();

		void SetBuf(void *buf, int len);							// ���û�����		
		int Capacity()const { return _capacity; } 						// ��������
		bool Full()const { return this->_size >= this->_capacity; } 	// ������
    };
#endif
