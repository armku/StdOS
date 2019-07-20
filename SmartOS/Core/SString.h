#ifndef __String_H__
#define __String_H__

//#include "Array.h"
//#include "ByteArray.h"

// �ַ������֣���Ҫ�����ַ�������
class StringSplit;

// �ַ���
class String : public Array
{
public:
	
};

#define R(str) String(str)

class StringSplit
{
public:
	cstring Sep;		// �ָ�������һ��ҪѰ�ҵı߽��
	int		Position;	// ��ǰ��λ�á�������ʾ���˽�β��
	int		Length;		// ��ǰ�γ��ȡ�

	StringSplit(const String& str, cstring sep);

	const String Next();

	// �Ƿ��Ѿ�����ĩβ
	//bool End() const { return Position == -2; }

    explicit operator bool() const { return Position >= -1; }
    bool operator !() const { return Position < -1; }

private:
	const String& _Str;
};

#endif
