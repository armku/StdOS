#ifndef __AES_H__
#define __AES_H__

#include "Kernel\Sys.h"

// AES �����㷨
class AES
{
public:
	// �ӽ���
	static ByteArray Encrypt(const Buffer& data, const Buffer& pass);
	static ByteArray Decrypt(const Buffer& data, const Buffer& pass);
};

#endif
