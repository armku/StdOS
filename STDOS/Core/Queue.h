#ifndef _Queue_H_
    #define _Queue_H_

    #include "Array.h"
    // 队列
    // 内有一个缓冲区，游标位置，数据长度。实际有效数据仅占用缓冲区中间部分，头尾都可能有剩余
    class Queue
    {
	private:
		int _size;		// 长度
		int _head;		// 头部位置
		int _tail;		// 尾部位置
		uint8_t * pBuf; 	/* 缓冲区 */
		int _capacity; 	/* 缓冲区大小 */
	public:
		Queue();
		void Clear();
		int Length() const { return _size; }		// 队列大小
		bool Empty() const { return _size == 0; }	// 队列空

		int Write(const Buffer& bs);	// 批量写入
		int Read(Buffer& bs);		// 批量读取 
		void Enqueue(uint8_t dat);
		uint8_t Dequeue();

		void SetBuf(void *buf, int len);							// 设置缓冲区		
		int Capacity()const { return _capacity; } 						// 队列容量
		bool Full()const { return this->_size >= this->_capacity; } 	// 队列满
    };
#endif
