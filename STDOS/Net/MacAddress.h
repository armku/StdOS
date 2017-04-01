#ifndef _MacAddress_H_
#define _MacAddress_H_

class Buffer;

// Mac��ַ
class MacAddress
{
public:
	// ������תΪMac��ַ��ȡ�ڴ�ǰ6�ֽڡ���Ϊ��С�ֽ�����Ҫv4��ǰ��v2�ں�
	UInt64	Value;	// ��ַ

	MacAddress(UInt64 v = 0);
	MacAddress(const byte* macs);
	MacAddress(const Buffer& arr);

	// �Ƿ�㲥��ַ��ȫ0��ȫ1
	bool IsBroadcast() const;

    MacAddress& operator=(UInt64 v);
    MacAddress& operator=(const byte* buf);
    MacAddress& operator=(const Buffer& arr);
    MacAddress& operator=(const MacAddress& mac);

    // �������������[]����������������һ��ʹ���±�������
    byte& operator[](int i);

	// �ֽ�����
    ByteArray ToArray() const;
	void CopyTo(byte* macs) const;

	// ���������ַ�����ʾ��ʽ
	String ToString() const;
	void Show(bool newLine = false) const { ToString().Show(newLine); }

    friend bool operator==(const MacAddress& addr1, const MacAddress& addr2)
	{
		return addr1.Value == addr2.Value;
	}
    friend bool operator!=(const MacAddress& addr1, const MacAddress& addr2)
	{
		return addr1.Value != addr2.Value;
	}

	static const MacAddress& Empty();
	static const MacAddress& Full();

	// ���ַ���Mac��ַ����ΪMacAddress
	static MacAddress Parse(const String& macstr);
};

#endif
