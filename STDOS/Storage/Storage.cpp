#include "Storage.h"
// ��ȡ
bool BlockStorage::Read(uint32_t address, Buffer& bs) const
	{
		return false;
	}
    // д��
    bool BlockStorage::Write(uint32_t address, const Buffer& bs) const
	{
		return false;
	}
	// ���
    bool BlockStorage::Memset(uint32_t address, uint8_t data, int len) const
	{
		return false;
	}
    // ����
    bool BlockStorage::Erase(uint32_t address, int len) const
	{
		return false;
	}
	// ָ�����Ƿ񱻲���
    bool BlockStorage::IsErased(uint32_t address, int len) const
	{
		return false;
	}
	// ��ȡ
    bool CharStorage::Read(uint32_t address, Buffer& bs) const
	{
		return false;
	}
		
    // д��
    bool CharStorage::Write(uint32_t address, const Buffer& bs) const
	{
		return false;
	}
