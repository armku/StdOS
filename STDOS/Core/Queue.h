#ifndef _Queue_H_
    #define _Queue_H_

    #include "Buffer.h"

    // 队列
    // 内有一个缓冲区，游标位置，数据长度。实际有效数据仅占用缓冲区中间部分，头尾都可能有剩余
    class Queue
    {
        private:
            byte *pBuf; /* 缓冲区 */
            int _write; /* 缓冲区写指针 */
            int _capacity; /* 缓冲区大小 */
            int _read; /* 缓冲区读指针 */
            int _count; /* 数据个数 */
        public:
            Queue();
            void SetBuf(void *buf, int len);
            bool Empty()const{return _count == 0;} // 队列空
            int Capacity()const{return _capacity;} // 队列容量
            bool Full()const{return this->_count >= this->_capacity;} // 队列满
            int Length()const{return _count;} // 队列大小
            void Clear();

            int Enqueue(byte da);
            byte Dequeue();

            int Write(const Buffer &bs); // 批量写入
            int Read(Buffer &bs); // 批量读取
    };
#endif
