#ifndef _Stream_H_
#define _Stream_H_
#include "ByteArray.h"

// 数据流
// 数据流内有一个缓冲区，游标位置，数据长度。实际有效数据仅占用缓冲区中间部分，头尾都可能有剩余
class Stream
{
protected:
	uint8_t* _Buffer;	// 数据缓冲区。扩容后会重新分配缓冲区 1
	int _Capacity;	// 缓冲区容量 2
    int _Position;	// 游标位置 3

	void Init(void* buf, int len);
	virtual bool CheckRemain(int count);
public:
	int	Length;	// 数据长度 4
	bool Little;	// 默认小字节序。仅影响数据读写操作 20
	bool CanWrite;	// 是否可写 21
	bool CanResize;	// 是否可以自动扩容 22

	// 使用缓冲区初始化数据流。注意，此时指针位于0，而内容长度为缓冲区长度
	Stream(void* buf, int len);
	Stream(const void* buf, int len);
	// 使用字节数组初始化数据流。注意，此时指针位于0，而内容长度为缓冲区长度
	explicit Stream(Buffer& bs);
	explicit Stream(const Buffer& bs);
	virtual ~Stream();

	// 数据流容量
	int Capacity() const;
	void SetCapacity(int len);
	// 当前位置
	int Position() const;
	// 设置位置
	bool SetPosition(int p);
	// 余下的有效数据流长度。0表示已经到达终点
	int Remain() const;
	// 尝试前后移动一段距离，返回成功或者失败。如果失败，不移动游标
	bool Seek(int offset);

	// 数据流指针。注意：扩容后指针会改变！
    uint8_t* GetBuffer() const;
	// 数据流当前位置指针。注意：扩容后指针会改变！
    uint8_t* Current() const;

	// 读取7位压缩编码整数
	int ReadEncodeInt();
	// 读取数据到字节数组，由字节数组指定大小。不包含长度前缀
	int Read(Buffer& bs);

	// 写入7位压缩编码整数
	int WriteEncodeInt(int value);
	// 把字节数组的数据写入到数据流。不包含长度前缀
	bool Write(const Buffer& bs);

	// 从数据流读取变长数据到字节数组。以压缩整数开头表示长度
	int ReadArray(Buffer& bs);
	//ByteArray ReadArray(int count);
	// 把字节数组作为变长数据写入到数据流。以压缩整数开头表示长度
	bool WriteArray(const Buffer& bs);

	//ByteArray ReadArray();
	String ReadString();

	int		ReadByte();
	uint16_t	ReadUInt16();
	uint32_t	ReadUInt32();
	uint64_t	ReadUInt64();

	bool Write(uint8_t value);
	bool Write(uint16_t value);
	bool Write(uint32_t value);
	bool Write(uint64_t value);
	//bool Write(int8_t value)	{ return Write((uint8_t)value); }
	bool Write(short value)	{ return Write((uint16_t)value); }
	bool Write(int value)	{ return Write((uint32_t)value); }
	bool Write(int64_t value)	{ return Write((uint64_t)value); }

	// 取回指定结构体指针，并移动游标位置
	template<typename T>
	T* Retrieve(bool move = true)
	{
		int p = sizeof(T) + _Position;
		// 允许移动到最后一个字节之后，也就是Length
		if(p < 0 || p > Length) return nullptr;

		T* pt = (T*)Current();
		if(move && !Seek(sizeof(T))) return nullptr;

		return pt;
	}

	// 读取指定长度的数据并返回首字节指针，移动数据流位置
	uint8_t* ReadBytes(int count = -1);

	// 读取一个字节，不移动游标。如果没有可用数据，则返回-1
	int Peek() const;
};

// 内存数据流。预分配空间，自动扩容
class MemoryStream : public Stream
{
private:
	uint8_t _Arr[0x40];	// 内部缓冲区。较小内存需要时，直接使用栈分配，提高性能。
	bool _needFree;		// 是否自动释放
	//bool _resize;		// 是否可以自动扩容

	virtual bool CheckRemain(int count);

public:
	// 分配指定大小的数据流
	MemoryStream(int len = 0);
	// 使用缓冲区初始化数据流，支持自动扩容
	MemoryStream(void* buf, int len);
	// 销毁数据流
	virtual ~MemoryStream();
};

#endif
