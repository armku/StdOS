#ifndef _Buffer_H_
#define _Buffer_H_

#include "Type.h"

/*
���������ԭ��
1���̶���������װ Buffer	=> Object
2���䳤��������װ Array		=> Buffer
3���Դ���ʼ��������װ ByteArray/String	=> Array

��ֵ�����ԭ��
1����ֵ���㿽�����Ⱥ����ݣ���������Ϊ����һ��������
2����ֵ���ȴ��ڵ�����ֵʱ����ֵ���ȱ�С
3����ֵ����С����ֵʱ����ֵ����SetLength����
4���������ʧ�ܣ����԰����ʧ�ܣ����а���ֵ���ȱ��ֲ���
*/

// �ڴ�����������װָ��ͳ���
// �ο�C#��Byte[]����Ҫʵ�ֶ�����ָ����໥ת������ֵ�����������á���ȡ���ȽϵȲ�����
// �ڲ�ָ��ָ����ڴ�ͳ��ȣ������ⲿ���룬�ڲ������Զ����䡣
// ���еĽ���������������ڲ�ָ�����󳤶ȣ������Զ����ݣ���������̳���չSetLength��
// ������ԭ���Ǿ�����Ϊ���ж��ٿ��ÿռ�Ϳ������ٳ��ȡ�
class Buffer //: public Object
{
public:
	// ���һ��ָ��ͳ���ָ����������
	Buffer(void* ptr, int len);
// ���ÿ������캯��
	Buffer(const Buffer& buf) = delete;
	// ����mov������ָ��ͳ��ȹ��ң���նԷ�
	Buffer(Buffer&& rval);

	// ����һ�����󿽱����ݺͳ��ȣ����Ȳ���������ʧ��ʱ����
	Buffer& operator = (const Buffer& rhs);
	// ��ָ�뿽����ʹ���ҵĳ���
	Buffer& operator = (const void* ptr);
	// ����mov������ָ��ͳ��ȹ��ң���նԷ�
	Buffer& operator = (Buffer&& rval);

	// �ó�ָ�빩�ⲿʹ��
	inline byte* GetBuffer()				{ return (byte*)_Arr; }
	inline const byte* GetBuffer() const	{ return (byte*)_Arr; }
	inline int Length() const				{ return _Length; }

	// �������鳤�ȡ�ֻ����С�����������������չ��ʵ���Զ�����
	//virtual bool SetLength(int len);
	//virtual void SetBuffer(void* ptr, int len);

	// ����ָ��λ�õ�ֵ�����Ȳ���ʱ�Զ�����
	bool SetAt(int index, byte value);

    // �������������[]������ָ��Ԫ�صĵ�һ���ֽ�
    byte operator[](int i) const;
	// ֧�� buf[i] = 0x36 ���﷨
    byte& operator[](int i);

	// ԭʼ���������㣬�����߽�
	static void Copy(void* dest, const void* src, int len);
	static void Zero(void* dest, int len);

	// �������ݣ�Ĭ��-1���ȱ�ʾ��ǰ����
	//virtual int Copy(int destIndex, const void* src, int len);
	// �����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
	//virtual int CopyTo(int srcIndex, void* dest, int len) const;
	// �������ݣ�Ĭ��-1���ȱ�ʾ������С����
	//virtual int Copy(int destIndex, const Buffer& src, int srcIndex, int len);
	// ����һ�����󿽱����ݺͳ��ȣ����Ȳ���������ʧ��ʱ����
	int Copy(const Buffer& src, int destIndex = 0);

	// ��ָ���ֽ����ó�ʼ��һ������
	int Set(byte item, int index, int len);
	void Clear(byte item = 0);

	// ��ȡһ���ӻ�������Ĭ��-1���ȱ�ʾʣ��ȫ��
	//### �����߼����Կ����޸�Ϊ����len�����ڲ�����ʱ��ֱ�����ڲ����ȶ�����������Ӧ�ò���ȥ�Ƚϳ��ȵĆ���
	Buffer Sub(int index, int len);
	const Buffer Sub(int index, int len) const;

	// ��ʾʮ���������ݣ�ָ���ָ��ַ��ͻ��г���
	String& ToHex(String& str, char sep = 0, int newLine = 0) const;
	// ��ʾʮ���������ݣ�ָ���ָ��ַ��ͻ��г���
	String ToHex(char sep = 0, int newLine = 0) const;

	ushort	ToUInt16(int offset = 0, bool isLittleEndian = true) const;
	uint	ToUInt32(int offset = 0, bool isLittleEndian = true) const;
	UInt64	ToUInt64(int offset = 0, bool isLittleEndian = true) const;
	void Write(ushort value, int index = 0);
	void Write(short value, int index = 0);
	void Write(uint value, int index = 0);
	void Write(int value, int index = 0);
	void Write(UInt64 value, int index = 0);

	// ���������ַ�����ʾ��ʽ
	//virtual String& ToStr(String& str) const;
	// ��װΪ�ַ�������
	String AsString() const;

    explicit operator bool() const { return _Length > 0; }
    bool operator !() const { return _Length == 0; }
	int CompareTo(const Buffer& bs) const;
	int CompareTo(const void* ptr, int len = -1) const;
	friend bool operator == (const Buffer& bs1, const Buffer& bs2);
	friend bool operator == (const Buffer& bs1, const void* ptr);
	friend bool operator != (const Buffer& bs1, const Buffer& bs2);
	friend bool operator != (const Buffer& bs1, const void* ptr);

#if DEBUG
	static void Test();
#endif

protected:
    char*	_Arr;		// ����ָ��
	int		_Length;	// ����

	void move(Buffer& rval);
	
	
	
public:
	virtual void Show(bool newLine = false) const;
	virtual void ShowHex(bool newLine = false, char sep = 0X20) const;
};

// �����ü����Ļ�����
class BufferRef : public Buffer
{
public:
	int	Ref	= 1;	// ���ü���

	// ���һ��ָ��ͳ���ָ����������
	void Set(void* ptr, int len);
};

#endif
