#include "Storage.h"
// 读取
bool BlockStorage::Read(uint32_t address, Buffer& bs) const
	{
		return false;
	}
    // 写入
    bool BlockStorage::Write(uint32_t address, const Buffer& bs) const
	{
		return false;
	}
	// 清空
    bool BlockStorage::Memset(uint32_t address, uint8_t data, int len) const
	{
		return false;
	}
    // 擦除
    bool BlockStorage::Erase(uint32_t address, int len) const
	{
		return false;
	}
	// 指定块是否被擦除
    bool BlockStorage::IsErased(uint32_t address, int len) const
	{
		return false;
	}
	// 读取
    bool CharStorage::Read(uint32_t address, Buffer& bs) const
	{
		return false;
	}
		
    // 写入
    bool CharStorage::Write(uint32_t address, const Buffer& bs) const
	{
		return false;
	}
