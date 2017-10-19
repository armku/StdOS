#include "ByteArray.h"
#include "Stream.h"

ushort _REV16(ushort a1)
{
  return (ushort)(a1 << 8) | (a1 >> 8);
}
uint _REV(uint a1)
{
  ushort v1;
  uint v2;

  v1 = a1;
  v2 = _REV16(a1 >> 16);
  return v2 | (_REV16(v1) << 16);
}

void Stream::Init(void *buf, int len)
{
    if (!buf)
    {
        //assert_failed2("buf,%s",__FILE__);
    }

    this->_Buffer = (byte*)buf;
    this->_Capacity = len;
    this->_Position = 0;
	this->Length=len;
	this->Little=true;
	this->CanWrite=true;
	this->CanResize=true;
}

bool Stream::CheckRemain(int count)
{
    if (count <= this->_Capacity)
    {
        return true;
    }
    else
    {
        if (this->_Buffer)
        {
            //SmartOS_printf("(%d - %d) = %d  %d \r\n",this);
        }
        else
        {
            //assert_failed2((const char *)dword_2AC, __FILE__, 0x3Fu);
            return false;
        }
        return false;
    }
}

// 使用缓冲区初始化数据流。注意，此时指针位于0，而内容长度为缓冲区长度
Stream::Stream(void *buf, int len)
{
    this->Init(buf, len);
}

Stream::Stream(const void *buf, int len)
{
    this->Init((void*)buf, len);
}

// 使用字节数组初始化数据流。注意，此时指针位于0，而内容长度为缓冲区长度
Stream::Stream(Buffer &bs)
{
    this->Init(bs.GetBuffer(), bs.Length());
}

Stream::Stream(const Buffer &bs)
{
    this->Init((void*)bs.GetBuffer(), bs.Length());
}

Stream::~Stream(){}

// 数据流容量
int Stream::Capacity()const
{
    return this->_Capacity;
}

void Stream::SetCapacity(int len)
{
    this->CheckRemain(len);
}

// 当前位置
int Stream::Position()const
{
    return this->_Position;
}

// 设置位置
bool Stream::SetPosition(int p)
{
    if (p >= 0 || this->_Capacity >= p)
    {
        this->_Position = p;
        return true;
    }
    else
    {
        //SmartOS_printf((const char *)sub_2F4);
        return false;
    }
}

// 余下的有效数据流长度。0表示已经到达终点
int Stream::Remain()const
{
    return this->_Capacity - this->_Position;
}

// 尝试前后移动一段距离，返回成功或者失败。如果失败，不移动游标
bool Stream::Seek(int offset)
{
    if (offset)
    {
        return this->SetPosition(this->_Position + offset);
    }
    else
    {
        return true;
    }
}

// 数据流指针。注意：扩容后指针会改变！
byte *Stream::GetBuffer()const
{
    return this->_Buffer;
}

// 数据流当前位置指针。注意：扩容后指针会改变！
byte *Stream::Current()const
{
    return this->_Buffer + this->_Position;
}

// 读取7位压缩编码整数
int Stream::ReadEncodeInt()
{
    int ret = 0;
    int v3 = 0;
    for (int i = 0; v3 < 4; i += 7)
    {
        int v5 = this->ReadByte();
        if (v5 < 0)
            break;
        ret |= (v5 &0x7F) << i;
        if (!(v5 &0x80))
            break;
        ++v3;
    }
    return ret;
}

// 读取数据到字节数组，由字节数组指定大小。不包含长度前缀
int Stream::Read(Buffer &bs)
{
    return 0;
}

// 写入7位压缩编码整数
int Stream::WriteEncodeInt(int value)
{
    return 0;
}

// 把字节数组的数据写入到数据流。不包含长度前缀
bool Stream::Write(const Buffer &bs)
{
    return false;
}

// 从数据流读取变长数据到字节数组。以压缩整数开头表示长度
int Stream::ReadArray(Buffer &bs)
{
    return 0;
}

ByteArray Stream::ReadArray(int count){}
// 把字节数组作为变长数据写入到数据流。以压缩整数开头表示长度
bool Stream::WriteArray(const Buffer &bs)
{
    return false;
}

ByteArray Stream::ReadArray(){}
//String Stream::ReadString()
//{
//	return nullptr;
//}

int Stream::ReadByte(){}
ushort Stream::ReadUInt16()
{	
    return 0;
}

uint Stream::ReadUInt32()
{
    return 0;
}

UInt64 Stream::ReadUInt64()
{
    return 0;
}

bool Stream::Write(byte value)
{
	byte buf[1];
	buf[0]=value;
	const Buffer v3(buf,1);
	return this->Write(v3);
}

bool Stream::Write(ushort value)
{
    ushort buf[1];
	buf[0]=value;
	const Buffer v3(buf,2);
	if(!this->Little)
		buf[0]= _REV16(buf[0]);
	return this->Write(v3);
}

bool Stream::Write(uint value)
{
    uint buf[1];
	buf[0]=value;
	const Buffer v3(buf,4);
	if(!this->Little)
		buf[0]=_REV(buf[0]);
	return this->Write(v3);
}

bool Stream::Write(UInt64 value)
{
	UInt64 buf[1];
	buf[0]=value;
	const Buffer v3(buf,8);
	if(!this->Little)
	{
		uint hi=_REV(buf[0]>>32);
		buf[0]=(_REV(buf[0])<<16)|hi;
	}
	return this->Write(v3);
}

// 读取指定长度的数据并返回首字节指针，移动数据流位置
byte *Stream::ReadBytes(int count)
{
	int v3;
	if(count<0)
		v3=this->Remain();	
	if(this->Seek(v3))
		return this->Current();
	else
		return nullptr;	
	
}

// 读取一个字节，不移动游标。如果没有可用数据，则返回-1
int Stream::Peek()const
{
	if(this->Remain())
	{
		return (int)*this->Current();
	}
	else
	{
		return -1;
	}
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
MemoryStream::~MemoryStream(){}
