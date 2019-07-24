#include "_Core.h"

#include "Type.h"
#include "Buffer.h"
#include "Array.h"

/******************************** Array ********************************/

Array::Array(void* data, int len) : Buffer(data, len)
{
	Init();
}

Array::Array(const void* data, int len) : Buffer((void*)data, len)
{
	Init();

	_canWrite = false;
}

Array::Array(const Buffer& rhs) : Buffer(nullptr, 0)
{
	Copy(0, rhs, 0, -1);

	Init();
}

Array::Array(Array&& rval) : Buffer(nullptr, 0)
{
	move(rval);
}

void Array::move(Array& rval)
{
	// ����Լ��������ڴ棬����Ҫ���ͷ�
	if (_needFree && _Arr != rval._Arr) Release();

	Buffer::move(rval);

	_Capacity = rval._Capacity;
	_needFree = rval._needFree;
	_canWrite = rval._canWrite;
	Expand = rval.Expand;

	rval._Capacity = 0;
	rval._needFree = false;
	rval._canWrite = false;
}

void Array::Init()
{
	Expand = true;
	_Size = 1;

	_Capacity = _Length;
	_needFree = false;
	_canWrite = true;
}

// �������ͷ���Դ
Array::~Array()
{
	Release();
}

// �ͷ���ռ���ڴ�
bool Array::Release()
{
	auto p = _Arr;
	bool fr = _needFree;

	_Arr = nullptr;
	_Capacity = 0;
	_Length = 0;

	_needFree = false;
	_canWrite = true;

	if (fr && p)
	{
		delete[](byte*)p;

		return true;
	}

	return false;
}

Array& Array::operator = (const Buffer& rhs)
{
	// ������Ҫ�����ݣ�����Buffer����ʱ�����ȿ��ܲ�׼ȷ
	// ����ҲҪ��ȣ����ܻ���˶�����
	//SetLength(rhs.Length());

	Buffer::operator=(rhs);

	return *this;
}

Array& Array::operator = (const void* p)
{
	Buffer::operator=(p);

	return *this;
}

Array& Array::operator = (Array&& rval)
{
	move(rval);

	return *this;
}

// �������鳤�ȡ������㹻����СLength������������ȷ�����������㹻������η����ڴ�
bool Array::SetLength(int len)
{
	return SetLength(len, true);
}

bool Array::SetLength(int len, bool bak)
{
	if (len <= _Capacity)
	{
		_Length = len;
	}
	else
	{
		if (!CheckCapacity(len, bak ? _Length : 0)) return false;
		// ���󳤶�
		if (len > _Length) _Length = len;
	}
	return true;
}

/*void Array::SetBuffer(void* ptr, int len)
{
	Release();

	Buffer::SetBuffer(ptr, len);
}

void Array::SetBuffer(const void* ptr, int len)
{
	SetBuffer((void*)ptr, len);

	_canWrite	= false;
}*/

// �������ݣ�Ĭ��-1���ȱ�ʾʹ���ұ���󳤶ȣ���߲���ʱ�Զ�����
int Array::Copy(int destIndex, const Buffer& src, int srcIndex, int len)
{
	// ������Ҫ�����ݣ�����Buffer����ʱ�����ȿ��ܲ�׼ȷ
	int remain = src.Length() - srcIndex;
	if (len < 0)
	{
		// -1ʱѡ���ұ���󳤶�
		len = remain;
		if (len <= 0) return 0;
	}
	else
	{
		// �ұ߿��ܲ���len
		if (len > remain) len = remain;
	}

	// ��߲���ʱ�Զ����ݣ���Ҫ������������
	if (Length() < destIndex + len) SetLength(destIndex + len, true);

	return Buffer::Copy(destIndex, src, srcIndex, len);
}

// ��������Ԫ��Ϊָ��ֵ���Զ�����
bool Array::SetItem(const void* data, int index, int count)
{
	assert(_canWrite, "��ֹSetItem�޸�");
	assert(data, "Array::SetItem data Error");

	// count<=0 ��ʾ����ȫ��Ԫ��
	if (count <= 0) count = _Length - index;
	assert(count > 0, "Array::SetItem count Error");

	// ��鳤���Ƿ��㹻
	int len2 = index + count;
	CheckCapacity(len2, index);

	// ���󳤶�
	if (len2 > _Length) _Length = len2;

	//byte* buf = (byte*)GetBuffer();
	// ���Ԫ�����ʹ�СΪ1����ô����ֱ�ӵ����ڴ����ú���
	if (_Size == 1)
		//memset(&buf[index], *(byte*)data, count);
		Set(*(byte*)data, index, count);
	else
	{
		while (count-- > 0)
		{
			//memcpy(buf, data, _Size);
			//buf += _Size;
			Copy(index, data, _Size);
			index += _Size;
		}
	}

	return true;
}

// �������顣ֱ��ʹ��ָ�룬����������
bool Array::Set(void* data, int len)
{
	if (!Set((const void*)data, len)) return false;

	_canWrite = true;

	return true;
}

// �������顣ֱ��ʹ��ָ�룬����������
bool Array::Set(const void* data, int len)
{
	// ���پɵ�
	if (_needFree && _Arr != data) Release();

	_Arr = (char*)data;
	_Length = len;
	_Capacity = len;
	_needFree = false;
	_canWrite = false;

	return true;
}

// ����Ѵ洢���ݡ�
void Array::Clear()
{
	assert(_canWrite, "��ֹClear�޸�");
	assert(_Arr, "Clear���ݲ���Ϊ��ָ��");

	//memset(_Arr, 0, _Size * _Length);
	Set(0, 0, _Size * _Length);
}

// ����ָ��λ�õ�ֵ������ʱ�Զ�����
void Array::SetItemAt(int i, const void* item)
{
	assert(_canWrite, "��ֹSetItemAt�޸�");

	// ��鳤�ȣ�����ʱ����
	CheckCapacity(i + 1, _Length);

	if (i >= _Length) _Length = i + 1;

	//memcpy((byte*)_Arr + _Size * i, item, _Size);
	Copy(_Size * i, item, _Size);
}

// �������������[]������ָ��Ԫ�صĵ�һ���ֽ�
byte Array::operator[](int i) const
{
	assert(_Arr && i >= 0 && i < _Length, "�±�Խ��");

	byte* buf = (byte*)_Arr;
	if (_Size > 1) i *= _Size;

	return buf[i];
}

byte& Array::operator[](int i)
{
	assert(_Arr && i >= 0 && i < _Length, "�±�Խ��");

	byte* buf = (byte*)_Arr;
	if (_Size > 1) i *= _Size;

	return buf[i];
}

// ���������������������󣬲�����ָ�����ȵ�����
bool Array::CheckCapacity(int len, int bak)
{
	// �Ƿ񳬳�����
	// ������ǿ�д�������ݼ��ʱ��ҲҪ�������ݣ������ڲ�����д���ݱ��޸�
	if (_Arr && len <= _Capacity && _canWrite) return true;
	// �Ƿ��������
	if (!Expand) return false;

	// �Զ�������ʵ�����
	int sz = 0x40;
	while (sz < len) sz <<= 1;

	bool _free = _needFree;

	void* p = Alloc(sz);
	if (!p) return false;

	// �Ƿ���Ҫ��������
	if (bak > _Length) bak = _Length;
	if (bak > 0 && _Arr)
		// Ϊ�˰�ȫ�������ֽڿ���
		Buffer(p, sz).Copy(0, _Arr, bak);

	int oldlen = _Length;
	if (_free && _Arr != p)
	{
		// Release(); �ᶯ��־λ ��������
		delete _Arr;
	}

	_Arr = (char*)p;
	_Capacity = sz;
	_Length = oldlen;

	// _needFree ��Alloc����
	// �п��ܵ�ǰ�õ��ڴ治���ڲ��ڴ棬Ȼ��Ҫ������ڴ�С���ڲ��ڴ棬��ֱ��ʹ���ڲ�������Ҫ�ͷ�
	//_needFree	= true;

	return true;
}

void* Array::Alloc(int len)
{
	_needFree = true;

	return new byte[_Size * len];
}

bool operator==(const Array& bs1, const Array& bs2)
{
	if (bs1.Length() != bs2.Length()) return false;

	return bs1.CompareTo(bs2) == 0;
	//return memcmp(bs1._Arr, bs2._Arr, bs1.Length() * bs1._Size) == 0;
}

bool operator!=(const Array& bs1, const Array& bs2)
{
	if (bs1.Length() != bs2.Length()) return true;

	return bs1.CompareTo(bs2) != 0;
	//return memcmp(bs1._Arr, bs2._Arr, bs1.Length() * bs1._Size) != 0;
}
