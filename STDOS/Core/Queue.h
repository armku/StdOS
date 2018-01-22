#ifndef _Queue_H_
    #define _Queue_H_

    #include "Array.h"
#define _USEARRAY 0
    // 队列
    // 内有一个缓冲区，游标位置，数据长度。实际有效数据仅占用缓冲区中间部分，头尾都可能有剩余
    class Queue
    {
	private:
		int _size;		// 长度
		int _head;		// 头部位置
		int _tail;		// 尾部位置
#if _USEARRAY
		Array _s;	// 数据流
#else
		byte * pBuf; 	/* 缓冲区 */
		int _capacity; 	/* 缓冲区大小 */
#endif
	public:
		Queue();
		void Clear();
		int Length() const { return _size; }		// 队列大小
		bool Empty() const { return _size == 0; }	// 队列空

		int Write(const Buffer& bs);	// 批量写入
		int Read(Buffer& bs);		// 批量读取 
		void Enqueue(byte dat);
		byte Dequeue();
#if _USEARRAY		
		int Capacity() const { return _s.Capacity(); }	// 队列容量		
		void SetCapacity(int len);		   	
#else
		void SetBuf(void *buf, int len);							// 设置缓冲区		
		int Capacity()const { return _capacity; } 						// 队列容量
		bool Full()const { return this->_size >= this->_capacity; } 	// 队列满
#endif
    };
#endif
