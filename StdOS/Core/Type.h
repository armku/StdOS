#ifndef __Type_H__
#define __Type_H__
#include <stdint.h>
/* ���Ͷ��� */
typedef const char*		cstring;

#define UInt64_Max 0xFFFFFFFFFFFFFFFFull

// ��ʹ��char���byte���ڷ���������ʹ��char*���void*
// ��Ϊ��ʽ�����ʱ��������%c���char����%s���char*
// ������
class Object
{
public:
	
};
void assert_failed(uint8_t *file, uint32_t line, char *errstr);
// ���鳤��
#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

int StdPrintf(const char *format, ...); //���� printf

// ������
#if defined(_MSC_VER)
#define WEAK
#else
#define	WEAK	__attribute__((weak))
#endif

#endif
