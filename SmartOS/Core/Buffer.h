#ifndef _Buffer_H_
#define _Buffer_H_

#include "Type.h"

/*
数据区设计原则：
1，固定数据区封装 Buffer	=> Object
2，变长数据区封装 Array		=> Buffer
3，自带初始缓冲区封装 ByteArray/String	=> Array

赋值运算符原则：
1，赋值运算拷贝长度和数据，核心语义为拷贝一份数据区
2，左值长度大于等于右值时，左值长度变小
3，左值长度小于右值时，左值尝试SetLength扩容
4，如果扩容失败，调试版断言失败，发行版左值长度保持不变
*/

// 内存数据区。包装指针和长度
// 参考C#的Byte[]，主要实现对象与指针的相互转化、赋值、拷贝、设置、截取、比较等操作。
// 内部指针指向的内存和长度，都由外部传入，内部不会自动分配。
// 所有的进出拷贝都是针对内部指针和最大长度，不会自动扩容，除非子类继承扩展SetLength。
// 拷贝的原则是尽力而为，有多少可用空间就拷贝多少长度。
class Buffer //: public Object
{

	
	
	
	
	
	
	
	
	
	
	
	
	
	
public:
	// 打包一个指针和长度指定的数据区
	Buffer(void* ptr, int len);
	int Length() { return this->_Length; }
	uint8_t operator[](int i) { return i < this->_Length ? this->_Arr[i] : 0;}
	// 支持 buf[i] = 0x36 的语法
//    byte& operator[](int i);
protected:
    char*	_Arr;		// 数据指针
	int		_Length;	// 长度

public:
	virtual void Show(bool newLine = false) const;
	virtual void ShowHex(bool newLine = false, char sep = 0X20) const;
};

// 带引用计数的缓冲区
class BufferRef : public Buffer
{
public:
	int	Ref	= 1;	// 引用计数

	// 打包一个指针和长度指定的数据区
	void Set(void* ptr, int len);
};

#endif
