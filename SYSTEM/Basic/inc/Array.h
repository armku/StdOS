/*
数组 Array
变长数据 Array 继承自缓冲区 Buffer，主要提供自动扩容功能。执行各种
操作，存储空间不足时，自动申请内存扩容，对象销毁时自动释放内存。
如果使用空指针构造 Array，那么第一次使用时就会申请内存。
*/
#pragma once

#include "Buffer.h"


//主要提供自动扩容功能
//数组
class Array:public Buffer
{
	public:
		
};
