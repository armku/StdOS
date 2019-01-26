#ifndef _Buffer_H_
#define _Buffer_H_

#include <stdint.h>
//#include "../../StdOS/Core/Type.h"

class Buffer 
{
public:
	// ���һ��ָ��ͳ���ָ����������
	Buffer(void* ptr, int len);
	int Length() { return this->_Length; }
	uint8_t operator[](int i) { return i < this->_Length ? this->_Arr[i] : 0;}
protected:
    char*	_Arr;		// ����ָ��
	int		_Length;	// ����

public:
	virtual void Show(bool newLine = false) const;
	virtual void ShowHex(bool newLine = false, char sep = 0X20) const;
};

#endif
