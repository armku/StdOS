#ifndef __RC4_H__
#define __RC4_H__

#include "Kernel\Sys.h"

// RC4 �����㷨
class RC4
{
public:
	// �ӽ���
	static void Encrypt(Buffer& data, const Buffer& pass);
	static ByteArray Encrypt(const Buffer& data, const Buffer& pass);
};

#endif
