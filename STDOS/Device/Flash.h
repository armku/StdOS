#ifndef __Flash_H__
#define __Flash_H__

#include "Storage\Storage.h"

// Flash��
class Flash : public BlockStorage
{
public:
	// д��
	virtual bool WriteBlock(uint address, const byte* buf, int len, bool inc) const;
    // ������ ���ε�ַ��
    virtual bool EraseBlock(uint address) const;
    // ָ�����Ƿ񱻲���
    //virtual bool IsErased(uint address, int len) const;

public:
	Flash();
	// ���ö�����   ע�⣺���������ʱ��������� Flash ����
	static bool ReadOutProtection(bool set);

private:
	void OnInit();
};

#endif
