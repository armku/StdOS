#ifndef _Array_H_
#define _Array_H_

#include "Buffer.h"

// �䳤���顣�Զ�����
class Array : public Buffer
{
public:
	bool	Expand;	// �Ƿ������
	
	// ���������������ʼ��ʱ���������治����ı�
	inline int Capacity() const { return _Capacity; }

	Array(void* data, int len);
	Array(const void* data, int len);
	Array(const Array& rhs) = delete;
	Array(Array&& rval);
	explicit Array(const Buffer& rhs);

	virtual ~Array();

	Array& operator = (const Buffer& rhs);
	Array& operator = (const void* p);
	Array& operator = (Array&& rval);

	using Buffer::Set;
	using Buffer::SetLength;
	using Buffer::Copy;

	// �������鳤�ȡ������㹻����СLength������������ȷ�����������㹻������η����ڴ�
	virtual bool SetLength(int len);
	virtual bool SetLength(int len, bool bak);
	//virtual void SetBuffer(void* ptr, int len);
	//virtual void SetBuffer(const void* ptr, int len);
	// �������ݣ�Ĭ��-1���ȱ�ʾʹ���ұ���󳤶ȣ���߲���ʱ�Զ�����
	virtual int Copy(int destIndex, const Buffer& src, int srcIndex, int len);

	// ��������Ԫ��Ϊָ��ֵ���Զ�����
	bool SetItem(const void* data, int index, int count);
	// �������顣ֱ��ʹ��ָ�룬����������
	bool Set(void* data, int len);
	// �������顣ֱ��ʹ��ָ�룬����������
	bool Set(const void* data, int len);
	// ����Ѵ洢���ݡ�
	virtual void Clear();
	// ����ָ��λ�õ�ֵ������ʱ�Զ�����
	virtual void SetItemAt(int i, const void* item);

    // �������������[]������ָ��Ԫ�صĵ�һ���ֽ�
    byte operator[](int i) const;
    byte& operator[](int i);

	friend bool operator==(const Array& bs1, const Array& bs2);
	friend bool operator!=(const Array& bs1, const Array& bs2);

#if DEBUG
	static void Test();
#endif

protected:
	bool	_needFree;	// �Ƿ���Ҫ�ͷ� string(4)
	bool	_canWrite;	// �Ƿ��д string(8)
	byte	_Size;		// ����Ԫ�ش�С���ֽ� string(13)
	int		_Capacity;	// �����������ֽ��� string(16)

	void Init();
	void move(Array& rval);

	// ���������������������󣬲�����ָ�����ȵ�����
	bool CheckCapacity(int len, int bak);
	virtual void* Alloc(int len);
	// �ͷ���ռ���ڴ�
	virtual bool Release();
};

#endif
