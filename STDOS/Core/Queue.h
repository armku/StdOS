#ifndef _Queue_H_
#define _Queue_H_

#include "Array.h"

// 队列
// 内有一个缓冲区，游标位置，数据长度。实际有效数据仅占用缓冲区中间部分，头尾都可能有剩余
class FIFO
{
	public:
		FIFO();
		void SetBuf(void *buf, int len);
		int Push(byte da);
		int Pop(byte *da);
		bool Empty();
		bool Full();
		void Clear();					
	private:			
		byte *pBuf; /* 缓冲区 */		
		int _write; /* 缓冲区写指针 */
		int BufSize; /* 缓冲区大小 */
		int _read; /* 缓冲区读指针 */
		int _count; /* 数据个数 */	
};
class Queue
{
private:
	Array _s;	// 数据流
	int _head;		// 头部位置
    int _tail;		// 尾部位置
	int _size;		// 长度

public:
	Queue();

	bool Empty() const { return _size == 0; }	// 队列空
	int Capacity() const { return _s.Capacity(); }	// 队列容量
	int Length() const { return _size; }		// 队列大小
	void SetCapacity(int len);

	void Clear();

	void Enqueue(byte dat);
	byte Dequeue();

	int Write(const Buffer& bs);	// 批量写入
	int Read(Buffer& bs);		// 批量读取
};
#endif
