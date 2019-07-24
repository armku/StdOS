#if defined(__CC_ARM)
#include <string.h>
#else
#include <cstring>
#endif

#include "_Core.h"

#include "Buffer.h"
#include "SString.h"


/******************************** Buffer ********************************/

Buffer::Buffer(void* ptr, int len)
{
	_Arr = (char*)ptr;
	_Length = len;
}

/*Buffer::Buffer(const Buffer& buf)
{
	Copy(0, rhs, 0, -1);
}*/

Buffer::Buffer(Buffer&& rval)
{
	move(rval);
}

void Buffer::move(Buffer& rval)
{
	_Arr = rval._Arr;
	_Length = rval._Length;

	rval._Arr = nullptr;
	rval._Length = 0;
}

Buffer& Buffer::operator = (const Buffer& rhs)
{
	if (!SetLength(rhs.Length())) assert(false, "��ֵ�����޷�����");

	Copy(rhs, 0);

	return *this;
}

Buffer& Buffer::operator = (const void* ptr)
{
	if (ptr) Copy(0, ptr, -1);

	return *this;
}

Buffer& Buffer::operator = (Buffer&& rval)
{
	move(rval);

	return *this;
}

// ����ָ��λ�õ�ֵ�����Ȳ���ʱ�Զ�����
bool Buffer::SetAt(int index, byte value)
{
	if (index >= _Length && !SetLength(index + 1)) return false;

	_Arr[index] = value;

	return true;
}

// �������������[]������ָ��Ԫ�صĵ�һ���ֽ�
byte Buffer::operator[](int i) const
{
	assert(i >= 0 && i < _Length, "�±�Խ��");

	byte* buf = (byte*)_Arr;

	return buf[i];
}

byte& Buffer::operator[](int i)
{
	assert(i >= 0 && i < _Length, "�±�Խ��");

	byte* buf = (byte*)_Arr;

	return buf[i];
}

// �������鳤�ȡ������㹻����СLength������ʧ�ܡ����������չ��ʵ���Զ�����
bool Buffer::SetLength(int len)
{
	if (len > _Length) return false;

	_Length = len;

	return true;
}

/*void Buffer::SetBuffer(void* ptr, int len)
{
	_Arr		= (char*)ptr;
	_Length		= len;
}*/

// ԭʼ���������㣬�����߽�
void Buffer::Copy(void* dest, const void* src, int len)
{
	memmove(dest, src, len);
}

void Buffer::Zero(void* dest, int len)
{
	memset(dest, 0, len);
}

// �������ݣ�Ĭ��-1���ȱ�ʾ��ǰ����
int Buffer::Copy(int destIndex, const void* src, int len)
{
	if (!src) return 0;

	// �Լ��л��������ܿ���
	if (!_Arr || !_Length) return 0;

	int remain = _Length - destIndex;

	// ���û��ָ�����ȣ��򿽱���ʼλ��֮���ʣ�ಿ��
	if (len < 0)
	{
		// ��ָ�����ȣ���û��ʣ�������޷�����
		if (remain <= 0)
		{
			debug_printf("Buffer::Copy (0x%p, %d) <= (%d, 0x%p, %d) \r\n", _Arr, _Length, destIndex, src, len);
			assert(false, "Buffer::Copy δָ��Ҫ�����ĳ���");

			return 0;
		}

		len = remain;
	}
	else if (len > remain)
	{
		// Ҫ�������������ݳ����ڴ��С�������ೢ�����ݣ��������ʧ�ܣ���ֻ����û�г�������һ����
		// �����������������
		if (!SetLength(destIndex + len))
		{
			debug_printf("Buffer::Copy (0x%p, %d) <= (%d, 0x%p, %d) \r\n", _Arr, _Length, destIndex, src, len);
			assert(false, "Buffer::Copy ������̫С");

			len = remain;
		}
	}

	// �ŵ������жϣ�ǰ���п����Զ�����
	if (!_Arr) return 0;

	// ���ҿ���������
	if (_Arr + destIndex == src) return len;

	// ��������
	if (len) memmove((byte*)_Arr + destIndex, (byte*)src, len);

	return len;
}

// �������ݣ�Ĭ��-1���ȱ�ʾ������С����
int Buffer::Copy(int destIndex, const Buffer& src, int srcIndex, int len)
{
	if (len < 0) len = _Length - destIndex;

	// ����������
	// Դ���ݵ�ʵ�ʳ��ȿ��ܸ�Ҫ�����ĳ��Ȳ�һ��
	int remain = src._Length - srcIndex;
	if (len > remain) len = remain;
	//if(len <= 0) return 0;

	return Copy(destIndex, (byte*)src._Arr + srcIndex, len);
}

int Buffer::Copy(const Buffer& src, int destIndex)
{
	return Copy(destIndex, (byte*)src._Arr, src.Length());
}

// �����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
int Buffer::CopyTo(int srcIndex, void* data, int len) const
{
	if (!_Arr || !data) return 0;

	int remain = _Length - srcIndex;
	if (remain <= 0) return 0;

	if (len < 0 || len > remain) len = remain;

	// ��������
	if (len)
	{
		if (data != (byte*)_Arr + srcIndex)
			memmove((byte*)data, (byte*)_Arr + srcIndex, len);
	}

	return len;
}

// ��ָ���ֽ����ó�ʼ��һ������
int Buffer::Set(byte item, int index, int len)
{
	if (!_Arr || len == 0) return 0;

	int remain = _Length - index;
	if (remain <= 0) return 0;

	if (len < 0 || len > remain) len = remain;

	if (len) memset((byte*)_Arr + index, item, len);

	return len;
}

void Buffer::Clear(byte item)
{
	Set(item, 0, _Length);
}

// ��ȡһ���ӻ�������Ĭ��-1���ȱ�ʾʣ��ȫ��
//### �����߼����Կ����޸�Ϊ����len�����ڲ�����ʱ��ֱ�����ڲ����ȶ�����������Ӧ�ò���ȥ�Ƚϳ��ȵĆ���
Buffer Buffer::Sub(int index, int len)
{
	assert(index >= 0, "index >= 0");
	assert(index < _Length, "index < _Length");
	if (len < 0) len = _Length - index;
	assert(index + len <= _Length, "len <= _Length");

	return Buffer((byte*)_Arr + index, len);
}

const Buffer Buffer::Sub(int index, int len) const
{
	assert(index >= 0, "index >= 0");
	if (len < 0) len = _Length - index;
#if DEBUG
	if (index + len > _Length) debug_printf("Buffer::Sub (%d, %d) > %d \r\n", index, len, _Length);
#endif
	//assert(index <= _Length, "index < _Length");
	assert(index + len <= _Length, "index + len <= _Length");

	return Buffer((byte*)_Arr + index, len);
}

// ��ʾʮ���������ݣ�ָ���ָ��ַ�
String& Buffer::ToHex(String& str, char sep, int newLine) const
{
	auto buf = GetBuffer();

	// ƴ�����ַ�������
	for (int i = 0; i < Length(); i++, buf++)
	{
		if (i)
		{
			if (newLine > 0 && i % newLine == 0)
				str += "\r\n";
			else if (sep != '\0')
				str += sep;
		}

		str.Concat(*buf, -16);
	}

	return str;
}

// ��ʾʮ���������ݣ�ָ���ָ��ַ�
String Buffer::ToHex(char sep, int newLine) const
{
	String str;

	// �Ż�Ϊʹ��RVO
	ToHex(str, sep, newLine);

	return str;
}

ushort	Buffer::ToUInt16(int offset, bool isLittleEndian) const
{
	auto p = GetBuffer() + offset;
	if (isLittleEndian)
	{
		// �ֽڶ���ʱ����֮ǰתΪĿ������
		if (((int)p & 0x01) == 0) return *(ushort*)p;

		return p[0] | (p[1] << 8);
	}
	else
	{
		return p[1] | (p[0] << 8);
	}
}

//�������ݸ�ʽ��ͬѡ��ͬ��ת����ʽ��Ĭ�ϸ�λ�ں�
uint	Buffer::ToUInt32(int offset, bool isLittleEndian) const
{
	auto p = GetBuffer() + offset;
	if (isLittleEndian)
	{
		// �ֽڶ���ʱ����֮ǰתΪĿ������
		if (((int)p & 0x03) == 0) return *(uint*)p;

		return p[0] | (p[1] << 8) | (p[2] << 0x10) | (p[3] << 0x18);
	}
	else
	{
		return p[3] | (p[2] << 8) | (p[1] << 0x10) | (p[0] << 0x18);
	}
}

UInt64	Buffer::ToUInt64(int offset, bool isLittleEndian) const
{
	if (isLittleEndian)
	{
		auto p = GetBuffer() + offset;
		// �ֽڶ���ʱ����֮ǰתΪĿ������
		if (((int)p & 0x07) == 0) return *(UInt64*)p;

		uint n1 = ToUInt32(offset, isLittleEndian);
		offset += 4;
		uint n2 = ToUInt32(offset, isLittleEndian);

		return n1 | ((UInt64)n2 << 0x20);
	}
	else
	{
		uint n1 = ToUInt32(offset, isLittleEndian);
		offset += 4;
		uint n2 = ToUInt32(offset, isLittleEndian);

		return n2 | ((UInt64)n1 << 0x20);
	}
}

void Buffer::Write(ushort value, int index)
{
	Copy(index, (byte*)&value, sizeof(ushort));
}

void Buffer::Write(short value, int index)
{
	Copy(index, (byte*)&value, sizeof(short));
}

void Buffer::Write(uint value, int index)
{
	Copy(index, (byte*)&value, sizeof(uint));
}

void Buffer::Write(int value, int index)
{
	Copy(index, (byte*)&value, sizeof(int));
}

void Buffer::Write(UInt64 value, int index)
{
	Copy(index, (byte*)&value, sizeof(UInt64));
}

//String& Buffer::ToStr(String& str) const
//{
//	return ToHex(str, '-', 0x20);
//}

// ��װΪ�ַ�������
//String Buffer::AsString() const
//{
//	String str((cstring)_Arr, _Length);
//	return str;
//}

int Buffer::CompareTo(const Buffer& bs) const
{
	return CompareTo(bs._Arr, bs._Length);
}

int Buffer::CompareTo(const void* ptr, int len) const
{
	if (len < 0) len = _Length;

	// ͬһ��ָ�룬���Ⱦ�����С
	if (_Arr == ptr) return _Length - len;

	// ����̳������Ƚ�
	int count = _Length;
	if (count > len)	count = len;

	// ����ÿһ���ַ�
	for (cstring p1 = _Arr, p2 = (cstring)ptr; count > 0; count--, p1++, p2++)
	{
		int n = *p1 - *p2;
		if (n) return n;
	}

	// �ж�ʣ�೤�ȣ��Դ˾�����С
	return _Length - len;
}

bool operator == (const Buffer& bs1, const Buffer& bs2)
{
	if (bs1._Arr == bs2._Arr) return true;
	if (!bs1._Arr || !bs2._Arr) return false;
	if (bs1.Length() != bs2.Length()) return false;

	return bs1.CompareTo(bs2) == 0;
}

bool operator == (const Buffer& bs1, const void* ptr)
{
	if (bs1._Arr == ptr) return true;
	if (!bs1._Arr || !ptr) return false;

	return bs1.CompareTo(ptr) == 0;
}

bool operator != (const Buffer& bs1, const Buffer& bs2)
{
	if (bs1._Arr == bs2._Arr) return false;
	if (!bs1._Arr || !bs2._Arr) return true;
	if (bs1.Length() != bs2.Length()) return true;

	return bs1.CompareTo(bs2) != 0;
}

bool operator != (const Buffer& bs1, const void* ptr)
{
	if (bs1._Arr == ptr) return false;
	if (!bs1._Arr || !ptr) return true;

	return bs1.CompareTo(ptr) != 0;
}

/******************************** BufferRef ********************************/

// ���һ��ָ��ͳ���ָ����������
void BufferRef::Set(void* ptr, int len)
{
	_Arr = (char*)ptr;
	_Length = len;
}


static void ShowChar(char c)
{
	if (c >= 10)
	{
		debug_printf("%c", c - 10 + 'A');
	}
	else
	{
		debug_printf("%c", c + '0');
	}
}

void Buffer::ShowHex(bool newLine,char sep) const
{
	for(int i=0;i<this->_Length;i++)
	{
		ShowChar(this->_Arr[i]>>4);
		ShowChar(this->_Arr[i]&0x0F);
		if(i!=this->_Length-1)
		{
			debug_printf("%c",sep);
		}
	}
	
	if(newLine)
	{
		debug_printf("\n");
	}
}
