#ifndef _Buffer_H_
#define _Buffer_H_

#include "Type.h"

class Buffer 
{
public:
	// ���һ��ָ��ͳ���ָ����������
	Buffer(void* ptr, int len);
	
protected:
    char*	_Arr;		// ����ָ��
	int		_Length;	// ����

public:
	virtual void Show(bool newLine = false) const;
	virtual void ShowHex(bool newLine = false, char sep = 0X20) const;
};

#endif
