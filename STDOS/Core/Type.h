#ifndef __Type_H__
#define __Type_H__

/* ���Ͷ��� */
typedef char			sbyte;
typedef unsigned char	byte;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long long	UInt64;
typedef long long		Int64;
typedef const char*		cstring;

#define UInt64_Max 0xFFFFFFFFFFFFFFFFull

// ��ʹ��char���byte���ڷ���������ʹ��char*���void*
// ��Ϊ��ʽ�����ʱ��������%c���char����%s���char*

class String;
class Type;

// ������
class Object
{
public:
	// ���������ַ�����ʾ��ʽ
	//virtual String& ToStr(String& str) const;
	// ���������ַ�����ʾ��ʽ��֧��RVO�Ż�
	//virtual String ToString() const;
	// ��ʾ����Ĭ����ʾToString
	virtual void Show(bool newLine = false) const;

	//const Type GetType() const;
};
// ����
class Type
{
private:
	const void* _info;

	friend class Object;

	Type();

public:
	int		Size;	// ��С
	//String	Name;	// ����

	const String Name() const;	// ����
};
// ���鳤��
//#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))
// �������㣬�̶�����
//#define ArrayZero(arr) memset(arr, 0, sizeof(arr))

#include <stdint.h>

#ifndef NULL
    #define NULL 0
#endif

#ifdef  DEBUG
void assert_failed(byte* file, uint line,char * errstr);
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert(expr,errstr) ((expr) ?(void)0:assert_failed((uint8_t *)__FILE__, __LINE__,errstr))
/* Exported functions ------------------------------------------------------- */  
#else
  #define assert(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus         

    class CType{

    }
    ;
#endif /* __cplusplus */


#endif
