#include <stdio.h>
#include "Buffer.h"
#include "Sys.h"

Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = (char*)ptr;
    this->_Length = len;    
}

Buffer &Buffer::operator = (const Buffer &rhs)
{
    if (this->_Length < rhs._Length)
    {
        debug_printf("Error: Buffer copy: Buffer length mismath src: %d ,dst: %d \n", rhs._Length, this->_Length);
    }
    else
    {
        for (int i = 0; i < rhs.Length(); i++)
        {
            ((byte*)(this->_Arr))[i] = rhs.GetBuffer()[i];
        }
        this->_Length = rhs._Length;
    }
    return  *this;
}

Buffer &Buffer::operator = (const void *prt)
{
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = ((byte*)prt)[i];
    }
    return  *this;
}

//设置长度，可自动扩容 
bool Buffer::SetLength(int len)
{
    
    {
        this->_Length = len;
    }
    return true;
}

//设置指定位置的值，长度不足时报错
bool Buffer::SetAt(int index, byte value)
{
    if (index >= this->_Length)
    {
        return false;
    }
    ((byte*)this->_Arr)[index] = value;
    return true;
}

//自我索引运算符[] 返回指定元素的第一个字节
byte Buffer::operator[](int i)const
{
    return ((byte*)this->_Arr)[i];
}

//支持buf[i]=0x33的语法
byte &Buffer::operator[](int i)
{
    if ((i < 0) || (i > this->_Length))
    {
        debug_printf("Error: [] length error");
        return ((byte*)(this->_Arr))[0];
    }
    return ((byte*)(this->_Arr))[i];
}

//原始拷贝 清零 不检查边界
void Buffer::Copy(void *dest, const void *source, int len)
{
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

void Buffer::Zero(void *dest, int len)
{
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[i] = 0;
    }
}

//拷贝数据，默认-1长度表示当前长度 
int Buffer::Copy(int destIndex, const void *src, int len)
{
    if (len ==  - 1)
    {
        len = this->_Length;
    }
    if (len <= 0)
    {
        return 0;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)(this->_Arr))[destIndex + i] = ((byte*)src)[i];
    }
	return len;
}

//把数据复制到目标缓冲区，默认-1长度表示当前长度
int Buffer::CopyTo(int destIndex, void *dest, int len)const
{
    if (len ==  - 1)
    {
        len = this->_Length;
    }
    if (len <= 0)
    {
        return 0;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[destIndex + i] = ((byte*)(this->_Arr))[i];
    }
	return len;
}

//拷贝数据，默认-1长度表示两者最小长度 
int Buffer::Copy(int destIndex, const Buffer &src, int srcIndex, int len)
{
    if (len ==  - 1)
    {
        len = this->_Length;
        if (len < src._Length)
        {
            this->SetLength(src._Length);
            len = this->_Length;
        }
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)this->_Arr)[destIndex + i] = ((byte*)(src._Arr))[srcIndex + i];
    }
	return len;
}

int Buffer::Copy(const Buffer &src, int destIndex)
{
    int len = this->Length();
    if ((destIndex + this->Length()) > src.Length())
    {
        len = src.Length();
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)this->_Arr)[i + destIndex] = src[i];
    }
    return len;
}

//用指定字节设置初始化一个区域
int Buffer::Set(byte item, int index, int len)
{
    if (index + this->Length() < len)
    {
        len = this->Length() - index;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)this->_Arr)[index + i] = item;
    }
    return len;
}

void Buffer::Clear(byte item)
{
    for (int i = 0; i < this->Length(); i++)
    {
        ((byte*)this->_Arr)[i] = item;
    }
}

//截取一个子缓冲区，默认-1长度表示剩余全部		
Buffer Buffer::Sub(int index, int length)
{
    if (length < 0)
    {
        return Buffer(((byte*)this->_Arr) + index, this->Length() - index);
    }
    if (index + length > this->_Length)
    {
        length = this->_Length - index;
    }
    return Buffer(((byte*)this->_Arr) + index, length);
}

const Buffer Buffer::Sub(int index, int len)const
{
    if (len < 0)
    {
        return Buffer(((byte*)this->_Arr) + index, this->Length() - index);
    }
    if (index + len > this->_Length)
    {
        len = this->_Length - index;
    }
    return Buffer(((byte*)this->_Arr) + index, len);
}

#if 0
    //显示16禁止数据，指定分割字符和换行长度
    String &Buffer::ToHex(String &str, char sep, int newLine)const
    {
        return  *String("Fuck", 5);
    }

    //显示16禁止数据，指定分割字符和换行长度
    String Buffer::ToHex(char sep, int newLine)
    {
        return  *String("Fuck", 5);
    }
#endif 
#if 0
    ushort Buffer::ToUInt16()const{}
    uint Buffer::ToUInt32()const{}

    UInt64 Buffer::ToUInt64()const{}
#endif 
void Buffer::Write(ushort value, int index){

}
void Buffer::Write(short value, int index){}
void Buffer::Write(uint value, int index){}
void Buffer::Write(int value, int index){}
void Buffer::Write(UInt64 value, int index){}
#if 0
    //输出对象的字符串表示方式
    String &Buffer::ToStr(String &str)const{}
    //包装为字符串对象
    String Buffer::AsString()const{}
#endif 
int Buffer::CompareTo(const Buffer &bs)const
{
    for (int i = 0; i < this->_Length; i++)
    {
        if (this->GetBuffer()[i] > bs[i])
        {
            return 1;
        }
        if (this->GetBuffer()[i] < bs[i])
        {
            return  - 1;
        }
    }
    return 0;
}

int Buffer::CompareTo(const void *ptr, int len)const
{
    if (len > this->_Length)
    {
        len = this->_Length;
    }
    int ret = 0;
    for (int i = 0; i < this->_Length; i++)
    {
        if (this->GetBuffer()[i] > ((byte*)ptr)[i])
        {
            return 1;
        }
        if (this->GetBuffer()[i] < ((byte*)ptr)[i])
        {
            return  - 1;
        }
    }
    return ret;
}


bool operator == (const Buffer &bs1, const Buffer &bs2)
{
    if (bs1.Length() != bs2.Length())
    {
        return false;
    }
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != bs2[i])
        {
            return false;
        }
    }
    return true;
}


bool operator == (const Buffer &bs1, const void *ptr)
{
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != ((byte*)ptr)[i])
        {
            return false;
        }
    }
    return true;
}

bool operator != (const Buffer &bs1, const Buffer &bs2)
{
    if (bs1.Length() != bs2.Length())
    {
        return true;
    }
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != bs2[i])
        {
            return true;
        }
    }
    return false;
}

bool operator != (const Buffer &bs1, const void *ptr)
{
    for (int i = 0; i < bs1.Length(); i++)
    {
        if (bs1[i] != ((byte*)ptr)[i])
        {
            return true;
        }
    }
    return false;
}















void Buffer::Show(bool newLine)const
{
    if (newLine)
    {
        for (int i = 0; i < this->_Length - 1; i++)
        {
            printf("%02X ", ((byte*)(this->_Arr))[i]);
        }
        printf("%02X", ((byte*)(this->_Arr))[this->_Length - 1]);
    }
    else
    {
        printf("%s", ((byte*)(this->_Arr)));
    }
    //Object::Show(newLine);
    Object::Show(true);
}









//Buffer::Buffer(){}
String &Buffer::ToStr(String &str)const
{
    String *ret = new String();
    return  *ret;
}

#if DEBUG
    void Buffer::Test()
    {
        #if 0
            //使用指针和长度构造一个内存区
            char cs[] = "This is Buffer Test.";
            Buffer bs(cs, sizeof(cs));
            debug_printf("Buffer bs (cs,strlen(cs)) => %s\r\n", cs);
            assert(bs.GetBuffer() == (byte*)cs, "GetBuffer()");
            assert(bs == cs, "Buffer(void* p = nullptr, int len = 0)");
        #endif 
        #if 0
            byte buf[] = 
            {
                1, 2, 3, 4
            };
            byte bts[] = 
            {
                5, 6, 7, 8, 9, 10
            };
            Buffer bs1(buf, sizeof(buf));

            char err[] = "Buffer& operator = (const void* ptr)";

            //从指针拷贝，使用我的长度
            bs1 = bts;
            assert(bs1.GetBuffer() == buf, err);
            assert(bs1.GetBuffer() != bts, err);
            assert(bs1.Length() == sizeof(buf), err);
            assert(buf[0] == bts[0] && buf[3] == bts[3], err);
        #endif 
        #if 0
            byte buf[] = 
            {
                1, 2, 3, 4
            };
            byte bts[] = 
            {
                5, 6, 7
            };
            Buffer bs(buf, sizeof(buf));
            Buffer bs2(bts, sizeof(bts));

            char err[] = "Buffer& operator = (const Buffer& rhs)";

            //从另一个对象拷贝数据和长度 长度不足且扩容失败时报错
            //Buffer 无法自动扩容，Array可以
            //bs2 = bs;
            bs = bs2;
            assert(bs.GetBuffer() == buf, err);
            assert(bs.GetBuffer() != bts, err);
            assert(bs.Length() == sizeof(bts), err);
            assert(bs.Length() != sizeof(buf), err);
            assert(buf[0] == bts[0] && buf[2] == bts[2], err);
            assert(buf[3] == 4, err);
        #endif 

    }
#endif
