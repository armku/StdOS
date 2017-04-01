#ifndef __Storage_H__
#define __Storage_H__

#include "Kernel\Sys.h"

// �洢�ӿ�
class Storage
{
public:
    // ��ȡ
    virtual bool Read(uint address, Buffer& bs) const = 0;
    // д��
    virtual bool Write(uint address, const Buffer& bs) const = 0;
};

// ��洢�ӿ�
class BlockStorage : public Storage
{
public:
	int Size;		// ������С���ֽ�
    uint Start;		// ��ʼ��ַ
    int Block;		// ÿ���ֽ���
	bool ReadModifyWrite;	// �Ƿ����д
	bool XIP;		// �Ƿ�ӳ��Ƭ��ִ��

    // ��ȡ
    virtual bool Read(uint address, Buffer& bs) const;
    // д��
    virtual bool Write(uint address, const Buffer& bs) const;
	// ���
    virtual bool Memset(uint address, byte data, int len) const;
    // ����
    virtual bool Erase(uint address, int len) const;

protected:
	// д���
	virtual bool WriteBlock(uint address, const byte* buf, int len, bool inc) const = 0;
    // ������
    virtual bool EraseBlock(uint address) const = 0;
    // ָ�����Ƿ񱻲���
    virtual bool IsErased(uint address, int len) const;
};

// �ַ��洢�ӿ�
class CharStorage : public Storage
{
public:
    // ��ȡ
    virtual bool Read(uint address, Buffer& bs) const;
    // д��
    virtual bool Write(uint address, const Buffer& bs) const;
};

#endif
