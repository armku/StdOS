#ifndef __RC6_H__
#define __RC6_H__

#include "Kernel\Sys.h"

// RC6 �����㷨
class RC6
{
public:
	// �ӽ���
	static ByteArray Encrypt(const Buffer& data, const Buffer& pass);
	static ByteArray Decrypt(const Buffer& data, const Buffer& pass);
};

#endif
