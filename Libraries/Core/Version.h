#ifndef __Version_H__
#define __Version_H__

// �汾
class Version
{
public:
	uint8_t	Major;	// ���汾
	uint8_t	Minor;	// �ΰ汾
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
};

/*
���ΰ汾һ�����ֻ�м�ʮ���������ʱ���Ǹ�λ��������Ҫ̫��
*/

#endif
