#ifndef _Queue_H_
    #define _Queue_H_

    #include "Buffer.h"

    // ����
    // ����һ�����������α�λ�ã����ݳ��ȡ�ʵ����Ч���ݽ�ռ�û������м䲿�֣�ͷβ��������ʣ��
    class Queue
    {
        private:
            byte *pBuf; /* ������ */
            int _write; /* ������дָ�� */
            int _capacity; /* ��������С */
            int _read; /* ��������ָ�� */
            int _count; /* ���ݸ��� */
        public:
            Queue();
            void SetBuf(void *buf, int len);
            bool Empty()const{return _count == 0;} // ���п�
            int Capacity()const{return _capacity;} // ��������
            bool Full()const{return this->_count >= this->_capacity;} // ������
            int Length()const{return _count;} // ���д�С
            void Clear();

            int Enqueue(byte da);
            byte Dequeue();

            int Write(const Buffer &bs); // ����д��
            int Read(Buffer &bs); // ������ȡ
    };
#endif
