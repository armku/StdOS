#ifndef _ByteArray_H_
#define _ByteArray_H_

#include "Array.h"

// �ֽ�����
class ByteArray : public Array
{
public:
	explicit ByteArray(int length = 0);
	ByteArray(byte item, int length);
	// ��Ϊʹ���ⲿָ�룬�����ʼ��ʱû��Ҫ�����ڴ�����˷�
	ByteArray(const void* data, int length, bool copy = false);
	ByteArray(void* data, int length, bool copy = false);
	explicit ByteArray(const Buffer& arr);
	ByteArray(const ByteArray& arr) = delete;
	ByteArray(ByteArray&& rval);

	ByteArray& operator = (const Buffer& rhs);
	ByteArray& operator = (const ByteArray& rhs);
	ByteArray& operator = (const void* p);
	ByteArray& operator = (ByteArray&& rval);

	// ���صȺ��������ʹ���ⲿָ�롢�ڲ����ȣ��û��Լ�ע�ⰲȫ
	//ByteArray& operator=(const void* data);

	// ���浽��ͨ�ֽ����飬���ֽ�Ϊ����
	int Load(const void* data, int maxsize = -1);
	// ����ͨ�ֽ���������أ����ֽ�Ϊ����
	int Save(void* data, int maxsize = -1) const;

	//friend bool operator==(const ByteArray& bs1, const ByteArray& bs2);
	//friend bool operator!=(const ByteArray& bs1, const ByteArray& bs2);

#if DEBUG
	static void Test();
#endif

protected:
	byte	Arr[0x40];	// �ڲ�������

	virtual void* Alloc(int len);

	void move(ByteArray& rval);
};

#endif
