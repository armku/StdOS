#ifndef __MD5_H__
#define __MD5_H__

#include "Kernel\Sys.h"

// MD5 ɢ���㷨
class MD5
{
public:
	// ɢ��
	static ByteArray Hash(const Buffer& data);
	// �ַ���ɢ�С���ȡ���ַ�����Ӧ���ֽ������ɢ�У�Ȼ��תΪHEX�����ַ���
	static String Hash(const String& str);
};

#endif
