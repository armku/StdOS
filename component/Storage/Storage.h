#ifndef __Storage_H__
#define __Storage_H__

#include "Sys.h"

// 存储接口
class Storage
{
public:
    // 读取
    virtual bool Read(uint32_t address, void * buf, int len) const = 0;
    // 写入
    virtual bool Write(uint32_t address, void * buf, int len) const = 0;
};

// 块存储接口
class BlockStorage : public Storage
{
public:
	int Size;		// 容量大小，字节
    uint32_t Start;		// 起始地址
    int Block;		// 每块字节数
	bool ReadModifyWrite;	// 是否读改写
	bool XIP;		// 是否映射片上执行

    // 读取
    virtual bool Read(uint32_t address, void * buf, int len) const;
    // 写入
    virtual bool Write(uint32_t address, void * buf, int len) const;
	// 清空
    virtual bool Memset(uint32_t address, uint8_t data, int len) const;
    // 擦除
    virtual bool Erase(uint32_t address, int len) const;

protected:
	// 写入块
	virtual bool WriteBlock(uint32_t address, const uint8_t* buf, int len, bool inc) const = 0;
    // 擦除块
    virtual bool EraseBlock(uint32_t address) const = 0;
    // 指定块是否被擦除
    virtual bool IsErased(uint32_t address, int len) const;
};

// 字符存储接口
class CharStorage : public Storage
{
public:
    // 读取
    virtual bool Read(uint32_t address, void * buf,int len) const;
    // 写入
    virtual bool Write(uint32_t address, void * buf, int len) const;
};

#endif
