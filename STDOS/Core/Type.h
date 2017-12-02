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
	virtual String& ToStr(String& str) const;
	// ���������ַ�����ʾ��ʽ��֧��RVO�Ż�
	virtual String ToString() const;
	// ��ʾ����Ĭ����ʾToString
	virtual void Show(bool newLine = false) const;

	const Type GetType() const;
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
#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))
// �������㣬�̶�����
//#define ArrayZero(arr) memset(arr, 0, sizeof(arr))

int StdPrintf(const char *format, ...); //���� printf

// ������
#if defined(_MSC_VER)
#define WEAK
#else
#define	WEAK	__attribute__((weak))
#endif

#endif
