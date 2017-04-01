#ifndef _IPEndPoint_H_
#define _IPEndPoint_H_

#include "IPAddress.h"

// IP���
class IPEndPoint
{
public:
	IPAddress	Address;	// ��ַ
	ushort		Port;		// �˿�

	IPEndPoint();
	IPEndPoint(const IPAddress& addr, ushort port);
	IPEndPoint(const Buffer& arr);

    IPEndPoint& operator=(const Buffer& arr);
    IPEndPoint& operator=(const IPEndPoint& endPoint);

	// �ֽ�����
    ByteArray ToArray() const;
	void CopyTo(byte* ips) const;

	// ���������ַ�����ʾ��ʽ
	String ToString() const;
	void Show(bool newLine = false) const { ToString().Show(newLine); }

	static const IPEndPoint& Any();
};

bool operator==(const IPEndPoint& addr1, const IPEndPoint& addr2);
bool operator!=(const IPEndPoint& addr1, const IPEndPoint& addr2);

#endif
