#include "ByteArray.h"
#include "Stream.h"

void Stream::Init(void* buf, int len)
{
}
bool Stream::CheckRemain(int count)
{
	return false;
}

// 使用缓冲区初始化数据流。注意，此时指针位于0，而内容长度为缓冲区长度
	Stream::Stream(void* buf, int len)
{
}
	Stream::Stream(const void* buf, int len)
{
}
	// 使用字节数组初始化数据流。注意，此时指针位于0，而内容长度为缓冲区长度
	Stream::Stream(Buffer& bs)
{
}
	Stream::Stream(const Buffer& bs)
{
}
	Stream::~Stream()
{
}

	// 数据流容量
	int Stream::Capacity() const
{
	return 0;
}
	void Stream::SetCapacity(int len)
{
}
	// 当前位置
	int Stream::Position() const
{
	return 0;
}
	// 设置位置
	bool Stream::SetPosition(int p)
{
	return false;
}
	// 余下的有效数据流长度。0表示已经到达终点
	int Stream::Remain() const
{
	return 0;
}
	// 尝试前后移动一段距离，返回成功或者失败。如果失败，不移动游标
	bool Stream::Seek(int offset)
{
	return false;
}

	// 数据流指针。注意：扩容后指针会改变！
    byte* Stream::GetBuffer() const
{
}
	// 数据流当前位置指针。注意：扩容后指针会改变！
    byte* Stream::Current() const
{
}

	// 读取7位压缩编码整数
	int Stream::ReadEncodeInt()
{
	return 0;
}
	// 读取数据到字节数组，由字节数组指定大小。不包含长度前缀
	int Stream::Read(Buffer& bs)
{
	return 0;
}

	// 写入7位压缩编码整数
	int Stream::WriteEncodeInt(int value)
{
	return 0;
}
	// 把字节数组的数据写入到数据流。不包含长度前缀
	bool Stream::Write(const Buffer& bs)
{
	return false;
}

	// 从数据流读取变长数据到字节数组。以压缩整数开头表示长度
	int Stream::ReadArray(Buffer& bs)
{
	return 0;
}
	ByteArray Stream::ReadArray(int count)
{
}
	// 把字节数组作为变长数据写入到数据流。以压缩整数开头表示长度
	bool Stream::WriteArray(const Buffer& bs)
{
	return false;
}

	ByteArray Stream::ReadArray()
{
}
//String Stream::ReadString()
//{
//	return nullptr;
//}

	int		Stream::ReadByte()
{
}
	ushort	Stream::ReadUInt16()
{
	return 0;
}
	uint	Stream::ReadUInt32()
{
	return 0;
}
	UInt64	Stream::ReadUInt64()
{
	return 0;
}

	bool Stream::Write(byte value)
{
	return false;
}
	bool Stream::Write(ushort value)
{
	return false;
}
	bool Stream::Write(uint value)
{
	return false;
}
	bool Stream::Write(UInt64 value)
{
	return false;
}
	// 读取指定长度的数据并返回首字节指针，移动数据流位置
	byte* Stream::ReadBytes(int count )
{
}

	// 读取一个字节，不移动游标。如果没有可用数据，则返回-1
	int Stream::Peek() const
{
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
bool MemoryStream::CheckRemain(int count)
{
	return false;
}

// 分配指定大小的数据流
//MemoryStream::MemoryStream(int len )
//{
//}
// 使用缓冲区初始化数据流，支持自动扩容
//MemoryStream::MemoryStream(void* buf, int len)
//{
//}

// 销毁数据流
MemoryStream::~MemoryStream()
{
}
