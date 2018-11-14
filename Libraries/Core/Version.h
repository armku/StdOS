#ifndef __Version_H__
#define __Version_H__

#include "Type.h"

// �汾
class Version
{
public:
	int	Major;	// ���汾
	int	Minor;	// �ΰ汾
	int Year;	// ��
	int MonthDay;// ����

	Version(int major = 0, int minor = 0, int year = 0, int monday = 0);
	Version(const Version& ver);
	int Parse(char* str);//����汾����ʽ1.0.2018.1114
	int Parse(const char* str) { return Parse((char*)str); }//����汾����ʽ1.0.2018.1114
	
    Version& operator=(const Version& ver);
	void Show(bool newLine = true) const;
	
	int CompareTo(const Version& value) const;
    friend bool operator==	(const Version& left, const Version& right);
    friend bool operator!=	(const Version& left, const Version& right);
    friend bool operator>	(const Version& left, const Version& right);
    friend bool operator<	(const Version& left, const Version& right);
    friend bool operator>=	(const Version& left, const Version& right);
    friend bool operator<=	(const Version& left, const Version& right);
};

/*
�汾�Ÿ�ʽ��1.0.2018.1114
*/

#endif
