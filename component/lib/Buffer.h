#ifndef _Buffer_H_
#define _Buffer_H_

#include <stdint.h>
//#include "../../StdOS/Core/Type.h"

class Buffer 
{
public:
	// 打包一个指针和长度指定的数据区
	Buffer(void* ptr, int len);
	int Length() { return this->_Length; }
	uint8_t operator[](int i) { return i < this->_Length ? this->_Arr[i] : 0;}
protected:
    char*	_Arr;		// 数据指针
	int		_Length;	// 长度

public:
	virtual void Show(bool newLine = false) const;
	virtual void ShowHex(bool newLine = false, char sep = 0X20) const;
};

#endif
