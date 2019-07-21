#include <stddef.h>

#include "_Core.h"

#include "ByteArray.h"
#include "SString.h"

#include "Stream.h"

extern ushort _REV16(ushort);
extern uint _REV(uint);

// ʹ�û�������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
Stream::Stream(void* buf, int len)
{
	Init(buf, len);
}

Stream::Stream(const void* buf, int len)
{
	Init((void*)buf, len);

	CanWrite = false;
}

// ʹ���ֽ������ʼ����������ע�⣬��ʱָ��λ��0�������ݳ���Ϊ����������
Stream::Stream(Buffer& bs)
{
	Init(bs.GetBuffer(), bs.Length());
}

Stream::Stream(const Buffer& bs)
{
	Init((void*)bs.GetBuffer(), bs.Length());

	CanWrite = false;
}

Stream::~Stream() { }

void Stream::Init(void* buf, int len)
{
	assert(buf, "buf");

	_Buffer = (byte*)buf;
	_Capacity = len;
	_Position = 0;
	CanWrite = true;
	CanResize = true;
	Length = len;
	Little = true;
}

bool Stream::CheckRemain(int count)
{
	int remain = _Capacity - _Position;
	// ������������Ҫ����
	if (count > remain)
	{
		if (CanResize)
			debug_printf("������ 0x%p ʣ������ (%d - %d) = %d ���� %d ���޷����ݣ�\r\n", this, _Capacity, _Position, remain, count);
		else
			assert(false, "�޷�����");

		return false;
	}

	return true;
}

// ����������
int Stream::Capacity() const { return _Capacity; }
void Stream::SetCapacity(int len) { CheckRemain(len - _Position); }

// ��ǰλ��
int Stream::Position() const { return _Position; }

// ����λ��
bool Stream::SetPosition(int p)
{
	// �����ƶ������һ���ֽ�֮��Ҳ����Length
	//assert(p <= Length, "���õ�λ�ó�������");
	if (p < 0 && p > Length)
	{
		debug_printf("���õ�λ�� %d �������� %d\r\n", p, Length);
		return false;
	}

	_Position = p;
	return true;
}

// ���µ���Ч���������ȡ�0��ʾ�Ѿ������յ�
int Stream::Remain() const { return Length - _Position; };

// ����ǰ���ƶ�һ�ξ��룬���سɹ�����ʧ�ܡ����ʧ�ܣ����ƶ��α�
bool Stream::Seek(int offset)
{
	if (offset == 0) return true;

	return SetPosition(offset + _Position);
}

// ������ָ�롣ע�⣺���ݺ�ָ���ı䣡
byte* Stream::GetBuffer() const { return _Buffer; }

// ��������ǰλ��ָ�롣ע�⣺���ݺ�ָ���ı䣡
byte* Stream::Current() const { return &_Buffer[_Position]; }

// ��ȡ7λѹ����������
int Stream::ReadEncodeInt()
{
	int value = 0;
	// ͬʱ�������4�ֽڣ��������޶�ȡ��������
	for (int i = 0, k = 0; i < 4; i++, k += 7)
	{
		int temp = ReadByte();
		if (temp < 0) break;

		value |= (temp & 0x7F) << k;
		if ((temp & 0x80) == 0) break;
	}
	return value;
}

// ��ȡ���ݵ��ֽ����飬���ֽ�����ָ����С������������ǰ׺
int Stream::Read(Buffer& bs)
{
	if (bs.Length() == 0) return 0;

	Buffer ss(_Buffer, Length);
	int count = bs.Copy(0, ss, _Position, bs.Length());

	// �α��ƶ�
	_Position += count;

	bs.SetLength(count);

	return count;
}

// д��7λѹ����������
int Stream::WriteEncodeInt(int value)
{
	if (!CanWrite) return 0;

	byte buf[8];
	int k = 0;
	for (int i = 0; i < 4 && value >= 0x80; i++)
	{
		buf[k++] = (byte)(value | 0x80);

		value >>= 7;
	}
	{
		buf[k++] = (byte)value;
	}

	return Write(Buffer(buf, k));
}

// ���ֽ����������д�뵽������������������ǰ׺
bool Stream::Write(const Buffer& bs)
{
	int count = bs.Length();
	if (count == 0) return true;
	if (!CanWrite) return false;
	if (!CheckRemain(count)) return false;

	Buffer ss(_Buffer, _Capacity);
	count = ss.Copy(_Position, bs, 0, count);

	_Position += count;
	// ���ݳ��Ȳ����ۼӣ����Ǹ���λ�ö�����
	if (_Position > Length) Length = _Position;

	return true;
}

// ��ȡָ�����ȵ����ݲ��������ֽ�ָ�룬�ƶ�������λ��
byte* Stream::ReadBytes(int count)
{
	// Ĭ��С��0ʱ����ȡȫ������
	if (count < 0) count = Remain();

	byte* p = Current();
	if (!Seek(count)) return nullptr;

	return p;
}

// ��ȡһ���ֽڣ����ƶ��αꡣ���û�п������ݣ��򷵻�-1
int Stream::Peek() const
{
	if (!Remain()) return -1;
	return *Current();
}

// ����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
int Stream::ReadArray(Buffer& bs)
{
	int len = ReadEncodeInt();
	if (!len)
	{
		bs.SetLength(0);

		return 0;
	}

	if (len > bs.Length() && !bs.SetLength(len))
	{
		// �����ʱ�����ȡ�õĳ��ȳ����󣬿�������ƴ���
		//if(len > 0x40)
		{
			debug_printf("Stream::ReadArray ��������С���� �� %d > %d ", len, bs.Length());
			//assert(len <= bs.Capacity(), "��������С����");
		}
		return 0;
	}

	return Read(bs);
}

ByteArray Stream::ReadArray(int count)
{
	ByteArray bs;
	bs.SetLength(count);
	Read(bs);

	return bs;
}

// ��ȡ�ֽ����鷵�ء����õ�������ʱ�����Ĺ����������RVO��Ϊ�������ǽ���
ByteArray Stream::ReadArray()
{
	ByteArray bs;
	ReadArray(bs);

	return bs;
}

// ���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
bool Stream::WriteArray(const Buffer& bs)
{
	WriteEncodeInt(bs.Length());
	return Write(bs);
}

String Stream::ReadString()
{
	String str;

	ReadArray(str);

	return str;
}

int	Stream::ReadByte()
{
	if (Length == _Position) return -1;

	byte* p = Current();
	if (!Seek(1)) return 0;

	return *p;
}

ushort	Stream::ReadUInt16()
{
	ushort v;
	Buffer bs(&v, sizeof(v));
	if (!Read(bs)) return 0;
	if (!Little) v = _REV16(v);
	return v;
}

uint	Stream::ReadUInt32()
{
	uint v;
	Buffer bs(&v, sizeof(v));
	if (!Read(bs)) return 0;
	if (!Little) v = _REV(v);
	return v;
}

UInt64	Stream::ReadUInt64()
{
	UInt64 v;
	Buffer bs(&v, sizeof(v));
	if (!Read(bs)) return 0;
	if (!Little) v = _REV(v >> 32) | ((UInt64)_REV(v & 0xFFFFFFFF) << 32);
	return v;
}

bool Stream::Write(byte value)
{
	return Write(Buffer(&value, sizeof(value)));
}

bool Stream::Write(ushort value)
{
	if (!Little) value = _REV16(value);

	return Write(Buffer(&value, sizeof(value)));
}

bool Stream::Write(uint value)
{
	if (!Little) value = _REV(value);

	return Write(Buffer(&value, sizeof(value)));
}

bool Stream::Write(UInt64 value)
{
	if (!Little) value = _REV(value >> 32) | ((UInt64)_REV(value & 0xFFFFFFFF) << 32);

	return Write(Buffer(&value, sizeof(value)));
}

/******************************** MemoryStream ********************************/

MemoryStream::MemoryStream(int len) : Stream(_Arr, ArrayLength(_Arr))
{
	Length = 0;
	_needFree = false;
	if (len > ArrayLength(_Arr))
	{
		byte* buf = new byte[len];
		Init(buf, len);
		_needFree = true;
	}
}

MemoryStream::MemoryStream(void* buf, int len) : Stream(buf, len)
{
	_needFree = false;
}

// ����������
MemoryStream::~MemoryStream()
{
	if (_needFree)
	{
		if (_Buffer != _Arr) delete[] _Buffer;
		_Buffer = nullptr;
	}
}

bool MemoryStream::CheckRemain(int count)
{
	int remain = _Capacity - _Position;
	// ������������Ҫ����
	if (count > remain)
	{
		if (!CanResize) return Stream::CheckRemain(count);

		// ԭʼ�����ɱ�����
		int total = _Position + count;
		int size = _Capacity;
		if (size < 0x10) size = 0x10;
		while (size < total) size <<= 1;

		// �����µĿռ䣬����������
		byte* bufNew = new byte[size];
		if (Length > 0) Buffer(_Buffer, Length).CopyTo(0, bufNew, -1);

		if (_Buffer != _Arr && _needFree == true) delete[] _Buffer;

		_Buffer = bufNew;
		_Capacity = size;
		_needFree = true;
	}

	return true;
}

WEAK ushort _REV16(ushort value)
{
	return (ushort)((value << 8) | (value >> 8));
}

WEAK uint _REV(uint value)
{
	return (_REV16(value & 0xFFFF) << 16) | (_REV16(value >> 16));
}
