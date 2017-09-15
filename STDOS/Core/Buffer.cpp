#include <string.h>
#include "Sys.h"
#include "Buffer.h"

char chartmp[512];
// 打包一个指针和长度指定的数据区
Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = (char*)ptr;
    this->_Length = len;
}

// 对象mov操作，指针和长度归我，清空对方
Buffer::Buffer(Buffer && rval)
{

}

// 从另一个对象拷贝数据和长度，长度不足且扩容失败时报错
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

// 从指针拷贝，使用我的长度
Buffer &Buffer::operator = (const void *prt)
{
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = ((byte*)prt)[i];
    }
    return  *this;
}

// 设置数组长度。只能缩小不能扩大，子类可以扩展以实现自动扩容
bool Buffer::SetLength(int len)
{
    {
        this->_Length = len;
    }
    return true;
}

//virtual void Buffer::SetBuffer(void* ptr, int len)
//{

//}

// 设置指定位置的值，长度不足时自动扩容
bool Buffer::SetAt(int index, byte value)
{
    if (index >= this->_Length)
    {
        return false;
    }
    ((byte*)this->_Arr)[index] = value;
    return true;
}

// 重载索引运算符[]，返回指定元素的第一个字节
byte &Buffer::operator[](int i)
{
    if ((i < 0) || (i > this->_Length))
    {
        debug_printf("Error: [] length error");
        return ((byte*)(this->_Arr))[0];
    }
    return ((byte*)(this->_Arr))[i];
}

//自我索引运算符[] 返回指定元素的第一个字节
byte Buffer::operator[](int i)const
{
    return ((byte*)this->_Arr)[i];
    //}
    //int  Buffer::operator[](int i)
    //{
    //auto v2 = this;
    auto a2 = i;
    auto v3 = a2;
    if (a2 < 0 || this->_Length <= a2)
        assert_failed2("%s", __FILE__, 0x4Bu);
    return this->_Arr[a2];
}

// 原始拷贝、清零，不检查边界
void Buffer::Copy(void *dest, const void *source, int len)
{
    memmove(dest, source, len);
}

void Buffer::Zero(void *dest, int len)
{
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[i] = 0;
    }
}

// 拷贝数据，默认-1长度表示当前长度
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

// 把数据复制到目标缓冲区，默认-1长度表示当前长度
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

// 拷贝数据，默认-1长度表示两者最小长度
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

// 从另一个对象拷贝数据和长度，长度不足且扩容失败时报错
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

int Buffer::Set(byte item, int index, int len)
{
    int ret;
    
    if (this->_Arr && len)
    {
        if (this->_Length - index > 0)
        {
            if (len < 0 || len > this->_Length - index)
                len = this->_Length - index;
            if (len)
                memset(this->_Arr + index, len, item);
            ret = len;
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

void Buffer::Clear(byte item)
{
    for (int i = 0; i < this->Length(); i++)
    {
        ((byte*)this->_Arr)[i] = item;
    }
}

// 截取一个子缓冲区，默认-1长度表示剩余全部
//### 这里逻辑可以考虑修改为，当len大于内部长度时，直接用内部长度而不报错，方便应用层免去比较长度的啰嗦
Buffer Buffer::Sub(int index, int len)
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

// 显示十六进制数据，指定分隔字符和换行长度
String &Buffer::ToHex(String &str, char sep, int newLine)const{

}
// 显示十六进制数据，指定分隔字符和换行长度
String Buffer::ToHex(char sep, int newLine)const{

}

ushort Buffer::ToUInt16()const
{
    return 0;
}

uint Buffer::ToUInt32()const
{
    return 0;
}

UInt64 Buffer::ToUInt64()const
{
    return 0;
}

void Buffer::Write(ushort value, int index){

}
void Buffer::Write(short value, int index){

}
void Buffer::Write(uint value, int index){

}
void Buffer::Write(int value, int index){

}
void Buffer::Write(UInt64 value, int index){

}

// 输出对象的字符串表示方式
String &Buffer::ToStr(String &str)const
{
    return Buffer::ToHex(str, 45, 32);
}

// 包装为字符串对象
String Buffer::AsString()const{

}

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

void Buffer::move(Buffer &rval){

}

String Buffer::ToString()const
{
    for (int i = 0; i < this->_Length; i++)
    {
        chartmp[i] = this->_Arr[i];
    }
    chartmp[this->_Length] = 0;
    return String(chartmp);
}

// 打包一个指针和长度指定的数据区
void BufferRef::Set(void *ptr, int len)
{
    this->_Arr = (char*)ptr;
    this->_Length = len;
}
