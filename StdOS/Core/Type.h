#ifndef __Type_H__
#define __Type_H__
#include <stdint.h>

typedef const char*		cstring;

#define UInt64_Max 0xFFFFFFFFFFFFFFFFull

// ��ʹ��char���byte���ڷ���������ʹ��char*���void*
// ��Ϊ��ʽ�����ʱ��������%c���char����%s���char*
// ���鳤��
#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#endif
