#ifndef __String_H__
#define __String_H__

#include "Array.h"
#include "ByteArray.h"

// �ַ������֣���Ҫ�����ַ�������
class StringSplit;

// �ַ���
class String : public Array
{
public:
	String(cstring cstr = "");
	String(const String& str);
	String(String&& rval);
	// �ⲿ���뻺�������ڲ�ʹ�ã��ڲ������ַ������ȣ�ע�ⳤ�ȼ�ȥ�������
	String(char* str, int length);
	String(char* str, int length, bool expand);
	// ��װ��̬�ַ�����ֱ��ʹ�ã��޸�ʱ����
	String(cstring str, int length);
	explicit String(bool value);
	explicit String(char c);
	explicit String(uint8_t value, int radix = 10);
	explicit String(short value, int radix = 10);
	explicit String(uint16_t value, int radix = 10);
	explicit String(int value, int radix = 10);
	explicit String(uint32_t value, int radix = 10);
	explicit String(int64_t value, int radix = 10);
	explicit String(uint64_t value, int radix = 10);
	explicit String(float value, int decimalPlaces = 4);
	explicit String(double value, int decimalPlaces = 8);

	using Array::SetLength;
	using Array::Copy;

	// �ڴ����
	inline cstring GetBuffer() const { return (cstring)_Arr; }
	// �������鳤�ȡ��ı䳤�Ⱥ�ȷ�������0��β
	virtual bool SetLength(int length, bool bak);

	// �������ݣ�Ĭ��-1���ȱ�ʾ��ǰ����
	virtual int Copy(int destIndex, const void* src, int len);
	// �������ݣ�Ĭ��-1���ȱ�ʾ������С����
	//virtual int Copy(int destIndex, const Buffer& src, int srcIndex, int len);
	// �����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
	virtual int CopyTo(int srcIndex, void* dest, int len) const;

	// Ϊ����ֵ������һ�����ݡ�
	// ���ֵΪ�ջ���Ч�������ڴ����ʧ�ܣ��ַ������ᱻ���Ϊ��Ч
	String& operator = (const String& rhs);
	String& operator = (cstring cstr);
	String& operator = (String&& rval);

	// �����ڽ����͡����������Ч����Ϊ����ʧ��
	bool Concat(const Object& obj);
	bool Concat(const String& str);
	bool Concat(cstring cstr);
	bool Concat(bool value);
	bool Concat(char c);
	bool Concat(uint8_t c, int radix = 10);
	bool Concat(short num, int radix = 10);
	bool Concat(uint16_t num, int radix = 10);
	bool Concat(int num, int radix = 10);
	bool Concat(uint32_t num, int radix = 10);
	bool Concat(int64_t num, int radix = 10);
	bool Concat(uint64_t num, int radix = 10);
	bool Concat(float num, int decimalPlaces = 4);
	bool Concat(double num, int decimalPlaces = 8);

	// ģ�����ӿ������κδ���ToString�����Ķ���
	template<class T>
	bool Concat(const T& rhs) { return Concat(rhs.ToString()); }

	// �����ַ������������
	template<class T>
	String& operator += (const T& rhs)		{ Concat(rhs); return (*this); }
	template<class T>
	friend String& operator + (String& lhs, const T& rhs) { lhs.Concat(rhs); return lhs; }

    explicit operator bool() const { return _Length > 0; }
    bool operator !() const { return _Length == 0; }
	//operator char*() const { return _Arr; }
	int CompareTo(const String& s) const;
	int CompareTo(cstring cstr, int len = -1, bool ignoreCase = false) const;
	bool Equals(const String& s) const;
	bool Equals(cstring cstr) const;
	bool EqualsIgnoreCase(const String& s) const;
	bool EqualsIgnoreCase(cstring cstr) const;
	bool operator == (const String& rhs) const	{return Equals(rhs);	}
	bool operator == (cstring cstr) const 		{return Equals(cstr);	}
	bool operator != (const String& rhs) const	{return !Equals(rhs);	}
	bool operator != (cstring cstr) const		{return !Equals(cstr);	}
	bool operator <  (const String& rhs) const;
	bool operator >  (const String& rhs) const;
	bool operator <= (const String& rhs) const;
	bool operator >= (const String& rhs) const;

	//void SetAt(int index, char c);
	char operator [] (int index) const;
	char& operator [] (int index);
	void GetBytes(uint8_t* buf, int bufsize, int index=0) const;
	ByteArray GetBytes() const;
	ByteArray ToHex() const;
	void ToArray(char* buf, int bufsize, int index=0) const { GetBytes((uint8_t*)buf, bufsize, index); }

	int ToInt() const;
	float ToFloat() const;
	double ToDouble() const;

	// ���������ַ�����ʾ��ʽ
	virtual String& ToStr(String& str) const;
	// ���������ַ�����ʾ��ʽ
	virtual String ToString() const;

	// ��������ַ���
	virtual void Show(bool newLine = false) const;

	// ��ʽ���ַ���������������ַ������档��������������ʽ������ַ���
	String& Format(cstring format, ...);

	int IndexOf(const char ch, int startIndex = 0) const;
	int IndexOf(const String& str, int startIndex = 0) const;
	int IndexOf(cstring str, int startIndex = 0) const;
	int LastIndexOf(const char ch, int startIndex = 0) const;
	int LastIndexOf(const String& str, int startIndex = 0) const;
	int LastIndexOf(cstring str, int startIndex = 0) const;
	bool Contains(const String& str) const;
	bool Contains(cstring str) const;
	bool StartsWith(const String& str, int startIndex = 0) const;
	bool StartsWith(cstring str, int startIndex = 0) const;
	bool EndsWith(const String& str) const;
	bool EndsWith(cstring str) const;

	StringSplit Split(const String& sep) const;
	StringSplit Split(cstring sep) const;

	String Substring(int start, int len = -1) const;
	String TrimStart() const;
	String TrimEnd() const;
	String Trim() const;
	String Replace(char find, char replace) const;
	String Replace(const String& find, const String& replace) const;
	String Remove(int index) const;
	String Remove(int index, int count) const;
	String ToLower() const;
	String ToUpper() const;

	// ��̬�Ƚ������Ƚ������ַ���ָ��
	static int Compare(const void* v1, const void* v2);

#if DEBUG
	static void Test();
#endif

private:
	char	Arr[0x40];
	void init();
	void release();
	bool Concat(cstring cstr, int length);

	String& copy(cstring cstr, int length);
	void move(String& rhs);
	bool CopyOrWrite();

	using Array::CheckCapacity;
	bool CheckCapacity(int size);
	virtual void* Alloc(int len);

	int Search(cstring str, int len, int startIndex, bool rev) const;
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
