#include "Storage.h"
// 读取
bool BlockStorage::Read(uint address, Buffer& bs) const
	{
		return false;
	}
    // 写入
    bool BlockStorage::Write(uint address, const Buffer& bs) const
	{
		return false;
	}
	// 清空
    bool BlockStorage::Memset(uint address, byte data, int len) const
	{
		return false;
	}
    // 擦除
    bool BlockStorage::Erase(uint address, int len) const
	{
		return false;
	}
	// 指定块是否被擦除
    bool BlockStorage::IsErased(uint address, int len) const
	{
		return false;
	}
	// 读取
    bool CharStorage::Read(uint address, Buffer& bs) const
	{
		return false;
	}
		
    // 写入
    bool CharStorage::Write(uint address, const Buffer& bs) const
	{
		return false;
	}
