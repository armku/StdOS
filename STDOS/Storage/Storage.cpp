#include "Storage.h"
// ��ȡ
bool BlockStorage::Read(uint address, Buffer& bs) const
	{
		return false;
	}
    // д��
    bool BlockStorage::Write(uint address, const Buffer& bs) const
	{
		return false;
	}
	// ���
    bool BlockStorage::Memset(uint address, byte data, int len) const
	{
		return false;
	}
    // ����
    bool BlockStorage::Erase(uint address, int len) const
	{
		return false;
	}
	// ָ�����Ƿ񱻲���
    bool BlockStorage::IsErased(uint address, int len) const
	{
		return false;
	}
	// ��ȡ
    bool CharStorage::Read(uint address, Buffer& bs) const
	{
		return false;
	}
		
    // д��
    bool CharStorage::Write(uint address, const Buffer& bs) const
	{
		return false;
	}
