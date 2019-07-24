#ifndef __Version_H__
#define __Version_H__

#include "Type.h"

// �汾
class Version
{
public:
//	byte	Major;	// ���汾
//	byte	Minor;	// �ΰ汾
	short	Build;	// ����ʱ�䡣2000����������

	Version();
	Version(int value);
	Version(int major, int minor, int build);
	Version(const Version& ver);
	Version(Version&& ver);

	Version& operator=(const Version& ver);

	int ToValue() const;
	operator int() const { return ToValue(); }

	int CompareTo(const Version& value) const;
	friend bool operator==	(const Version& left, const Version& right);
	friend bool operator!=	(const Version& left, const Version& right);
	friend bool operator>	(const Version& left, const Version& right);
	friend bool operator<	(const Version& left, const Version& right);
	friend bool operator>=	(const Version& left, const Version& right);
	friend bool operator<=	(const Version& left, const Version& right);

	// ���ݰ汾�ŷ��Ʊ���ʱ�䡣
	DateTime Compile() const;
	// ���ñ�������
	Version& SetCompile(int year, int month, int day);
	// ���ñ������� 170325
	Version& SetCompile(int buildday);

	String ToString() const;
public:
	int	Major;	// ���汾
	int	Minor;	// �ΰ汾
	int Year;	// ��
	int MonthDay;// ����

	Version(int major , int minor , int year , int monday );
	int Parse(char* str);//����汾����ʽ1.0.2018.1114
	int Parse(const char* str) { return Parse((char*)str); }//����汾����ʽ1.0.2018.1114
	
	void Show(bool newLine = true) const;
	int Show(char* buf, int pos);
	int Show(byte* buf, int pos) { return Show((char*)buf, pos); }
};

/*
�汾�Ÿ�ʽ��1.0.2018.1114
*/

#endif
