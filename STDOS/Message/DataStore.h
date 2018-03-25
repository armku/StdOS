#ifndef __DataStore_H__
#define __DataStore_H__

class IDataPort;

// 数据存储适配器
class DataStore
{
public:
	ByteArray	Data;	// 数据
	bool		Strict;	// 是否严格限制存储区，读写不许越界。默认true
	uint32_t		VirAddrBase = 0;	// 虚拟地址起始位置， 可以吧Store定义到任意位置

	// 初始化
	DataStore();

	// 写入数据 offset 为虚拟地址
	int Write(uint32_t offset, const Buffer& bs);
	int Write(uint32_t offset, uint8_t data) { return Write(offset, Buffer(&data, 1)); }
	// 读取数据 offset 为虚拟地址
	int Read(uint32_t offset, Buffer& bs);

	typedef bool (*Handler)(uint32_t offset, uint32_t size, bool write);
	// 注册某一块区域的读写钩子函数
	void Register(uint32_t offset, uint32_t size, Handler hook);
	void Register(uint32_t offset, IDataPort& port);

private:
	IList	Areas;

	bool OnHook(uint32_t offset, uint32_t size, bool write);
};

/****************************** 数据操作接口 ************************************/

// 数据操作接口。提供字节数据的读写能力
class IDataPort
{
public:
	virtual int Size() const { return 1; };
	virtual int Write(uint8_t* data) { return Size(); };
	virtual int Read(uint8_t* data) { return Size(); };

	int Write(int data) { return Write((uint8_t*)&data); }
};

/****************************** 字节数据操作接口 ************************************/

// 字节数据操作接口
class ByteDataPort : public IDataPort
{
public:
	bool Busy;	// 是否忙于处理异步动作

	ByteDataPort();
	virtual ~ByteDataPort();

	virtual int Write(uint8_t* data);
	virtual int Read(uint8_t* data) { *data = OnRead(); return Size(); };

	// 让父类的所有Write函数在这里可见
	using IDataPort::Write;

	void Flush(int second);
	void FlushMs(int ms);
	void DelayOpen(int second);
	void DelayClose(int second);

protected:
	uint8_t	Next;	// 开关延迟后的下一个状态
	uint32_t	_tid;
	void StartAsync(int ms);
	static void AsyncTask(void* param);

	virtual int OnWrite(uint8_t data) { return OnRead(); };
	virtual uint8_t OnRead() { return Size(); };
};

#include "Device\Port.h"

// 数据输出口
class DataOutputPort : public ByteDataPort
{
public:
	OutputPort*	Port;

	DataOutputPort(OutputPort* port = nullptr) { Port = port; }

protected:
	virtual int OnWrite(uint8_t data);
	virtual uint8_t OnRead();

	String ToString() const;
};

// 数据输入口
class DataInputPort : public IDataPort
{
public:
	InputPort*	Port;

	DataInputPort(InputPort* port = nullptr) { Port = port; }

	virtual int Write(uint8_t* data);
	virtual int Read(uint8_t* data);

	String ToString() const;
};

#endif
