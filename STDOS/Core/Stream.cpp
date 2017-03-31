#include "Stream.h"
#include <stdio.h>

#if 0
//数据流容量
uint Stream::Capacity() const
{
    return 0;
}
#endif
#if 0
void Stream::SetCapacity(uint len){}
#endif
#if 0
//当前位置
uint Stream::Position() const
{
    return 0;
}
#endif
//设置位置
bool Stream::SetPosition(int p)
{
    return false;
}
#if 0
//余下的有效数据流长度，0表示已经到达终点
uint Stream::Remain() const
{
    return 0;
}
#endif
//尝试前后移动一段距离，返回成功或者失败。如果失败，不移动游标
bool Stream::Seek(int offset)
{
    return false;
}

//数据流指针，注意：扩容后指针会改变
byte *Stream::GetBuffer() const
{
    return NULL;
}

//数据流udagnqian位置指针。注意：扩容后指针会改变
byte *Stream::Current() const
{
    return NULL;
}
#if 0
//读取7为压缩编码整数
uint Stream::ReadEncodeInt()
{
    return 0;
}
#endif
#if 0
//读取数据到字节数组，由于字节数组指定大小，不包含长度前缀
uint Stream::Read(Buffer &bs)
{
	int len=bs.Length();
	if(len>this->capcity)
	{
		len=this->capcity;
	}
	for(int i=0;i<len;i++)
	{
		this->pbuf[i]=bs[i];
	}
    return 0;
}
#endif
#if 0
//写入7位压缩编码整数
uint Stream::WriteEncodeInt(uint value)
{
    return 0;
}
#endif
//把字节数组的数据写入到数据流，不包含长度前缀
bool Stream::Write(const Buffer &bs)
{
	return false;
}
#if 0
//从数据流读取变长数据到字节数组。以压缩整数开头表示长度
uint Stream::ReadArray(Buffer &bs)
{
    return 0;
}
#endif
#if 0
ByteArray Stream::ReadArray(int count)
{
	byte buf[3];
    return ByteArray(buf,3);
}
#endif
//把字节数组作为变长数据写入到数据流。以压缩整数开头表示长度
bool Stream::WriteArray(const Buffer &bs)
{
    return false;
}
