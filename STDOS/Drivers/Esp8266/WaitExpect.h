#ifndef __WaitExpect_H__
#define __WaitExpect_H__

#include "Kernel\WaitHandle.h"

// �ȴ�
class WaitExpect
{
public:
	WaitHandle	Handle;	// �ȴ����

	uint	TaskID	= 0;
	const String*	Command	= nullptr;
	String*	Result	= nullptr;
	cstring	Key1	= nullptr;
	cstring	Key2	= nullptr;

	bool	Capture	= true;	// �Ƿ񲶻�����

	bool Wait(int msTimeout);
	uint Parse(const Buffer& bs);
	uint FindKey(const String& str);
};

#endif
