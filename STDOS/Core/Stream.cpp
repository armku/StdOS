#include "ByteArray.h"
#include "Stream.h"

void Stream::Init(void* buf, int len)
{
}
bool Stream::CheckRemain(int count)
{
	return false;
}

// ʹ�û�������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
	Stream::Stream(void* buf, int len)
{
}
	Stream::Stream(const void* buf, int len)
{
}
	// ʹ���ֽ������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
	Stream::Stream(Buffer& bs)
{
}
	Stream::Stream(const Buffer& bs)
{
}
	Stream::~Stream()
{
}

	// ����������
	int Stream::Capacity() const
{
	return 0;
}
	void Stream::SetCapacity(int len)
{
}
	// ��ǰλ��
	int Stream::Position() const
{
	return 0;
}
	// ����λ��
	bool Stream::SetPosition(int p)
{
	return false;
}
	// ���µ���Ч���������ȡ�0��ʾ�Ѿ������յ�
	int Stream::Remain() const
{
	return 0;
}
	// ����ǰ���ƶ�һ�ξ��룬���سɹ�����ʧ�ܡ����ʧ�ܣ����ƶ��α�
	bool Stream::Seek(int offset)
{
	return false;
}

	// ������ָ�롣ע�⣺���ݺ�ָ���ı䣡
    byte* Stream::GetBuffer() const
{
}
	// ��������ǰλ��ָ�롣ע�⣺���ݺ�ָ���ı䣡
    byte* Stream::Current() const
{
}

	// ��ȡ7λѹ����������
	int Stream::ReadEncodeInt()
{
	return 0;
}
	// ��ȡ���ݵ��ֽ����飬���ֽ�����ָ����С������������ǰ׺
	int Stream::Read(Buffer& bs)
{
	return 0;
}

	// д��7λѹ����������
	int Stream::WriteEncodeInt(int value)
{
	return 0;
}
	// ���ֽ����������д�뵽������������������ǰ׺
	bool Stream::Write(const Buffer& bs)
{
	return false;
}

	// ����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
	int Stream::ReadArray(Buffer& bs)
{
	return 0;
}
	ByteArray Stream::ReadArray(int count)
{
}
	// ���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
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
	// ��ȡָ�����ȵ����ݲ��������ֽ�ָ�룬�ƶ�������λ��
	byte* Stream::ReadBytes(int count )
{
}

	// ��ȡһ���ֽڣ����ƶ��αꡣ���û�п������ݣ��򷵻�-1
	int Stream::Peek() const
{
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
bool MemoryStream::CheckRemain(int count)
{
	return false;
}

// ����ָ����С��������
//MemoryStream::MemoryStream(int len )
//{
//}
// ʹ�û�������ʼ����������֧���Զ�����
//MemoryStream::MemoryStream(void* buf, int len)
//{
//}

// ����������
MemoryStream::~MemoryStream()
{
}
