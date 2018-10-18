#ifndef _Buffer_H_
#define _Buffer_H_

#include "Type.h"

class Buffer 
{
public:
	// 打包一个指针和长度指定的数据区
	Buffer(void* ptr, int len);
	
protected:
    char*	_Arr;		// 数据指针 string(4) string(2)
	int		_Length;	// 长度 string(8)

public:
	virtual void Show(bool newLine = false) const;
	virtual void ShowHex(bool newLine = false, char sep = 0X20) const;
};

#endif
