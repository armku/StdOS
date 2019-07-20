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
	int Length() { return this->_Length; }
	uint8_t operator[](int i) { return i < this->_Length ? this->_Arr[i] : 0;}
	// ֧�� buf[i] = 0x36 ���﷨
//    byte& operator[](int i);
protected:
    char*	_Arr;		// ����ָ��
	int		_Length;	// ����

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
