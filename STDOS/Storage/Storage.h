#ifndef __Storage_H__
#define __Storage_H__

#include "Kernel\Sys.h"

// �洢�ӿ�
class Storage
{
public:
    // ��ȡ
    virtual bool Read(uint32_t address, Buffer& bs) const = 0;
    // д��
    virtual bool Write(uint32_t address, const Buffer& bs) const = 0;
};

// ��洢�ӿ�
class BlockStorage : public Storage
{
public:
	int Size;		// ������С���ֽ�
    uint32_t Start;		// ��ʼ��ַ
    int Block;		// ÿ���ֽ���
	bool ReadModifyWrite;	// �Ƿ����д
	bool XIP;		// �Ƿ�ӳ��Ƭ��ִ��

    // ��ȡ
    virtual bool Read(uint32_t address, Buffer& bs) const;
    // д��
    virtual bool Write(uint32_t address, const Buffer& bs) const;
	// ���
    virtual bool Memset(uint32_t address, byte data, int len) const;
    // ����
    virtual bool Erase(uint32_t address, int len) const;

protected:
	// д���
	virtual bool WriteBlock(uint32_t address, const byte* buf, int len, bool inc) const = 0;
    // ������
    virtual bool EraseBlock(uint32_t address) const = 0;
    // ָ�����Ƿ񱻲���
    virtual bool IsErased(uint32_t address, int len) const;
};

// �ַ��洢�ӿ�
class CharStorage : public Storage
{
public:
    // ��ȡ
    virtual bool Read(uint32_t address, Buffer& bs) const;
    // д��
    virtual bool Write(uint32_t address, const Buffer& bs) const;
};

#endif
