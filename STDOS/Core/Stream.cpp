#include "Stream.h"
#include <stdio.h>

void Stream::Init(void *buf, int len)
{
    this->_Buffer = (byte*)buf;
    this->_Capacity = len;
    this->_Position = 0;
    this->Little = true;
    this->CanWrite = true;
    this->CanResize = false;
}

bool Stream::CheckRemain(int count)
{
    if ((this->_Capacity - this->_Position) >= count)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Stream::Stream(void *buf, int len)
{
    this->Init(buf, len);
}

Stream::Stream(const void *buf, int len)
{
    this->Init((void*)buf, len);
}

Stream::Stream(Buffer &bs)
{
    this->Init(bs.GetBuffer(), bs.Length());
}

Stream::Stream(const Buffer &bs)
{
    this->Init((void*)(bs.GetBuffer()), bs.Length());
}

Stream::~Stream(){

}
int Stream::Capacity()const
{
    return this->_Capacity;
}

void Stream::SetCapacity(int len)
{
    if (len <= this->_Capacity)
    {
        this->_Capacity = len;
    }
}

int Stream::Position()const
{
    return this->_Position;
}

bool Stream::SetPosition(int p)
{
    if (p < this->_Capacity)
    {
        this->_Position = p;
        return true;
    }
    return false;
}

int Stream::Remain()const
{
    return this->_Capacity - this->_Position - 1;
}

bool Stream::Seek(int offset)
{
    int tmp = offset + this->_Position;
    if ((tmp >= 0) && (tmp < this->_Capacity))
    {
        this->_Position += offset;
        return true;
    }
    return false;
}

byte *Stream::GetBuffer()const
{
    return this->_Buffer;
}

byte *Stream::Current()const
{
    return this->_Buffer + this->_Position;
}

//int Stream::ReadEncodeInt()
//{
//	return 0;	
//}
// 读取数据到字节数组，由字节数组指定大小。不包含长度前缀
int Stream::Read(Buffer &bs)
{
    if (this->_Capacity <= bs.Length())
    {
        for (int i = 0; i < this->_Capacity; i++)
        {
            bs[i] = this->_Buffer[i];
        }
        return this->_Capacity;
    }
    else
    {
        for (int i = 0; i < bs.Length(); i++)
        {
            bs[i] = this->_Buffer[i];
        }
        return bs.Length();
    }
}

//int Stream::WriteEncodeInt(int value)
//{
//	return 0;
//}
bool Stream::Write(const Buffer &bs)
{
    if (this->_Capacity >= bs.Length())
    {
        for (int i = 0; i < bs.Length(); i++)
        {
            this->_Buffer[i] = bs[i];
        }

        return true;
    }
    else
    {
        for (int i = 0; i < this->_Capacity; i++)
        {
            this->_Buffer[i] = bs[i];
        }
        return false;
    }
}

int Stream::ReadArray(Buffer &bs)
{
    //未实现
    return 0;
}

//ByteArray Stream::ReadArray(int count)
//{
//	//未实现
//	return nullptr;
//}
//bool Stream::WriteArray(const Buffer& bs)
//{
//	
//	return false;
//}
//ByteArray Stream::ReadArray()
//{
//}
//	String Stream::ReadString()
//{
//}

int Stream::ReadByte()
{
    int ret = 0;
    if (this->_Position < this->Length)
    {
        ret = this->_Buffer[this->_Position];
    }
    return ret;
}

ushort Stream::ReadUInt16()
{
    ushort ret = 0;
    if (this->_Position < this->Length - 1)
    {
        if (this->Little)
        {
            ret = this->_Buffer[this->_Position + 0];
            ret <<= 8;
            ret |= this->_Buffer[this->_Position + 1];
        }
        else
        {
            ret = this->_Buffer[this->_Position + 1];
            ret <<= 8;
            ret |= this->_Buffer[this->_Position + 0];
        }
    }
    return ret;
}

uint Stream::ReadUInt32()
{
    uint ret = 0;
    if (this->_Position < this->Length - 3)
    {
        if (this->Little)
		{
			ret = this->_Buffer[this->_Position + 0];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 1];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 2];
            ret <<= 8;
            ret |= this->_Buffer[this->_Position+ 3];
		}
        else
        {
            ret = this->_Buffer[this->_Position + 3];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 2];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 1];
            ret <<= 8;
            ret |= this->_Buffer[this->_Position+ 0];
        }
    }
    return ret;
}

UInt64 Stream::ReadUInt64()
{
    UInt64 ret = 0;
    if (this->_Position < this->Length - 3)
    {
        if (this->Little)
		{
			ret = this->_Buffer[this->_Position + 0];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 1];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 2];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 3];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 4];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 5];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 6];
            ret <<= 8;
            ret |= this->_Buffer[this->_Position+ 7];
		}
        else
        {
            ret = this->_Buffer[this->_Position + 7];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 6];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 5];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 4];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 3];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 2];
            ret <<= 8;
            ret = this->_Buffer[this->_Position + 1];
            ret <<= 8;
            ret |= this->_Buffer[this->_Position+ 0];
        }
    }
    return ret;
}

bool Stream::Write(byte value)
{
    if (this->_Position < this->Length)
    {
        this->_Buffer[this->_Position] = value;
        return true;
    }
    return false;
}

bool Stream::Write(ushort value)
{
    if (this->_Position < this->Length - 1)
    {
        if (this->Little)
		{
			this->_Buffer[this->_Position + 1] = value &0XFF;
            this->_Buffer[this->_Position + 0] = (value >> 8) &0XFF;
		}
        else
        {
            this->_Buffer[this->_Position + 0] = value &0XFF;
            this->_Buffer[this->_Position + 1] = (value >> 8) &0XFF;
        }
        return true;
    }
    return false;
}

bool Stream::Write(uint value)
{
    if (this->_Position < this->Length - 3)
    {
        if (this->Little)
		{	
			this->_Buffer[this->_Position + 3] = value &0xff;
            this->_Buffer[this->_Position + 2] = (value >> 8) &0xff;
            this->_Buffer[this->_Position + 1] = (value >> 16) &0xff;
            this->_Buffer[this->_Position + 0] = (value >> 24) &0xff;
		}
        else
        {
            this->_Buffer[this->_Position + 0] = value &0xff;
            this->_Buffer[this->_Position + 1] = (value >> 8) &0xff;
            this->_Buffer[this->_Position + 2] = (value >> 16) &0xff;
            this->_Buffer[this->_Position + 3] = (value >> 24) &0xff;
        }

        return true;
    }
    return false;
}

bool Stream::Write(UInt64 value)
{
    if (this->_Position < this->Length - 7)
    {
        if (this->Little)
		{
			this->_Buffer[this->_Position + 7] = value &0xff;
            this->_Buffer[this->_Position + 6] = (value >> 8) &0xff;
            this->_Buffer[this->_Position + 5] = (value >> 16) &0xff;
            this->_Buffer[this->_Position + 4] = (value >> 24) &0xff;
            this->_Buffer[this->_Position + 3] = (value >> 32) &0xff;
            this->_Buffer[this->_Position + 2] = (value >> 40) &0xff;
            this->_Buffer[this->_Position + 1] = (value >> 48) &0xff;
            this->_Buffer[this->_Position + 0] = (value >> 56) &0xff;
		}
        else
        {
            this->_Buffer[this->_Position + 0] = value &0xff;
            this->_Buffer[this->_Position + 1] = (value >> 8) &0xff;
            this->_Buffer[this->_Position + 2] = (value >> 16) &0xff;
            this->_Buffer[this->_Position + 3] = (value >> 24) &0xff;
            this->_Buffer[this->_Position + 4] = (value >> 32) &0xff;
            this->_Buffer[this->_Position + 5] = (value >> 40) &0xff;
            this->_Buffer[this->_Position + 6] = (value >> 48) &0xff;
            this->_Buffer[this->_Position + 7] = (value >> 56) &0xff;
        }

        return true;
    }
    return false;
}

//	byte* Stream::ReadBytes(int count)
//{
//}

//	// 读取一个字节，不移动游标。如果没有可用数据，则返回-1
//	int Stream::Peek() const
//{
//}
//bool MemoryStream::CheckRemain(int count)
//{
//}
//// 分配指定大小的数据流
//MemoryStream::MemoryStream(int len )
//	{
//	}
//	// 使用缓冲区初始化数据流，支持自动扩容
//	MemoryStream::MemoryStream(void* buf, int len)
//	{
//	}
//	// 销毁数据流
//	MemoryStream:: ~MemoryStream()
//	{
//	}
