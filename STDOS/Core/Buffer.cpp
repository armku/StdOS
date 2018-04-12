#include <string.h>
#include "SString.h"
#include "Buffer.h"

//static char bufferName[]="Buffer"; 


// 打包一个指针和长度指定的数据区
Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = (char*)ptr;
    this->_Length = len;
}

// 对象mov操作，指针和长度归我，清空对方
Buffer::Buffer(Buffer && rval)
{
	this->move(rval);
}

// 从另一个对象拷贝数据和长度，长度不足且扩容失败时报错
Buffer &Buffer::operator = (const Buffer &rhs)
{
    int len = rhs.Length();
    //if (!(*(int(__fastcall **)(Buffer *, int))(*(_DWORD*)pthis + 12))(this, len))
    {
        //assert_failed2((const char*)dword_44C, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x29u); 
            //debug_printf("Error: Buffer copy: Buffer length mismath src: %d ,dst: %d \n", rhs._Length, this->_Length);
    }
    this->Copy(rhs, 0);
    return  *this;
}

// 从指针拷贝，使用我的长度
Buffer &Buffer::operator = (const void *prt)
{
    if(prt)
	{
		this->Copy(this->_Arr,prt,this->_Length);
	}
    return  *this;
}
// 对象mov操作，指针和长度归我，清空对方
Buffer& Buffer::operator = (Buffer&& rval)
{
	this->move(rval);
	return  *this;
}

// 设置数组长度。只能缩小不能扩大，子类可以扩展以实现自动扩容
bool Buffer::SetLength(int len)
{
    if (this->_Length >= len)
    {
        this->_Length = len;
        return true;
    }
    else
    {
        return false;
    }
}

// 设置指定位置的值，长度不足时自动扩容
bool Buffer::SetAt(int index, uint8_t value)
{
	//if ( this->_Length > index || (*(int (__fastcall **)(_DWORD, _DWORD))(*(_DWORD *)this + 12))(this, index + 1) )
    if (this->_Length > index || this->_Arr)
    {
        this->_Arr[index] = value;
        return true;
    }
    else
    {
        return false;
    }
}

// 重载索引运算符[]，返回指定元素的第一个字节
uint8_t &Buffer::operator[](int i)
{
	if ( i < 0 || this->_Length <= i )
	{
		//assert_failed2((const char *)dword_4B0, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x54u);//debug_printf("Error: [] length error");
	}
    return ((uint8_t*)(this->_Arr))[i];
}

//自我索引运算符[] 返回指定元素的第一个字节
uint8_t Buffer::operator[](int i)const
{
	if(i<0||this->_Length<=i)
	{
		//assert_failed2((const char *)dword_4B0, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x54u);
	}
    return ((uint8_t*)this->_Arr)[i];
}

// 原始拷贝、清零，不检查边界
void Buffer::Copy(void *dest, const void *source, int len)
{
    memmove(dest, source, len);
}

void Buffer::Zero(void *dest, int len)
{
	//memclr(dest,len);
    for (int i = 0; i < len; i++)
    {
        ((uint8_t*)dest)[i] = 0;
    }
}

// 拷贝数据，默认-1长度表示当前长度
int Buffer::Copy(int destIndex, const void *src, int len)
{
    int ret = 0;
    if (!src)
        return 0;
    if (!this->_Arr || !this->_Length)
        return 0;
    int copylen = this->_Length - destIndex;
    if (len >= 0)
    {
        // if ( len > copylen && !(*(int (__fastcall **)(Buffer *, int))(*(_DWORD *)this + 12))(this, destIndex + len) )
        if (len > copylen)
        {
            //debug_printf("Buffer::Copy (0x%p, %d) <= (%d, 0x%p, %d) \r\n", *(_QWORD *)((char *)pthis + 4), pdestIndex);
            //assert_failed2((const char *)dword_24C, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x95u);
            len = copylen;
        }
    }
    else
    {
        if (copylen <= 0)
        {
            //      debug_printf(
            //        "Buffer::Copy (0x%p, %d) <= (%d, 0x%p, %d) \r\n",
            //        *(_QWORD *)((char *)this + 4),
            //        destIndex,
            //        src,
            //        len);
            //      assert_failed2((const char *)dword_220, "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0x87u);
            return 0;
        }
        len = this->_Length - destIndex;
    }
    if (this->_Arr)
    {
        if ((const void*)(&this->_Arr[destIndex]) == src)
        {
            ret = len;
        }
        else
        {
            if (len)
                memmove(&(this->_Arr[destIndex]), src, len);
            ret = len;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
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
        ((uint8_t*)dest)[destIndex + i] = ((uint8_t*)(this->_Arr))[i];
    }
    return len;
}

// 拷贝数据，默认-1长度表示两者最小长度
int Buffer::Copy(int destIndex, const Buffer &src, int srcIndex, int len)
{
	//待处理
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
        ((uint8_t*)this->_Arr)[destIndex + i] = ((uint8_t*)(src._Arr))[srcIndex + i];
    }
    return len;
}

// 从另一个对象拷贝数据和长度，长度不足且扩容失败时报错
int Buffer::Copy(const Buffer &src, int destIndex)
{
	return this->Copy(destIndex,src._Arr,src.Length());
}

int Buffer::Set(uint8_t item, int index, int len)
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

void Buffer::Clear(uint8_t item)
{
	this->Set(item,0,this->_Length);	
}

// 截取一个子缓冲区，默认-1长度表示剩余全部
//### 这里逻辑可以考虑修改为，当len大于内部长度时，直接用内部长度而不报错，方便应用层免去比较长度的啰嗦
Buffer Buffer::Sub(int index, int len)
{
    if (index < 0)
    {
        //assert_failed2("index >= 0", "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0xF0u);
    }
    if (len < 0)
    {
        len = this->_Length - index;
    }
    if (index + len > this->_Length)
    {
        //debug_printf("Buffer::Sub (%d, %d) > %d \r\n", index, len, this->_Length);
    }
    if (index + len > this->_Length)
    {
        //assert_failed2("index + len <= _Length", "E:\\Smart\\SmartOS\\Core\\Buffer.cpp", 0xF6u);
    }
    return Buffer(((uint8_t*)this->_Arr) + index, len);
}

const Buffer Buffer::Sub(int index, int len)const
{
	if (len < 0)
    { 
        return Buffer(((uint8_t*)this->_Arr) + index, this->Length() - index);
    }
    if (index + len > this->_Length)
    {
        len = this->_Length - index;
    }
    return Buffer(((uint8_t*)this->_Arr) + index, len);
}

uint16_t Buffer::ToUInt16()const
{
    return 0;
}

uint32_t Buffer::ToUInt32()const
{
    return 0;
}

uint64_t Buffer::ToUInt64()const
{
    return 0;
}

void Buffer::Write(uint16_t value, int index)
{
	((uint16_t*)this->_Arr)[index]=value;
}
void Buffer::Write(short value, int index)
{
	((short*)this->_Arr)[index]=value;
}
void Buffer::Write(uint32_t value, int index)
{
	((uint32_t*)this->_Arr)[index]=value;
}
void Buffer::Write(int value, int index)
{
	//(*(int (__cdecl **)(Buffer *, int, int *))(*(_DWORD *)this + 16))(this, index, &pvalue);
	((int*)this->_Arr)[index]=value;
}
void Buffer::Write(uint64_t value, int index)
{
	((uint64_t*)this->_Arr)[index]=value;
}

static void ShowChar(char c)
{
	if(c>=10)
	{
		StdPrintf("%c",c-10+'A');
	}
	else
	{
		StdPrintf("%c",c+'0');
	}
}
void Buffer::Show(bool newLine)const
{	
	for(int i=0;i<this->_Length;i++)
	{
		StdPrintf("%c",this->_Arr[i]);
	}	
	if(newLine)
	{
		StdPrintf("\r\n");
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
			StdPrintf("%c",sep);
		}
	}
	
	if(newLine)
	{
		StdPrintf("\r\n");
	}
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
        if (this->GetBuffer()[i] > ((uint8_t*)ptr)[i])
        {
            return 1;
        }
        if (this->GetBuffer()[i] < ((uint8_t*)ptr)[i])
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
        if (bs1[i] != ((uint8_t*)ptr)[i])
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
        if (bs1[i] != ((uint8_t*)ptr)[i])
        {
            return true;
        }
    }
    return false;
}

void Buffer::move(Buffer &rval)
{
	this->_Arr=rval._Arr;
	this->_Length=rval._Length;
	rval._Arr=NULL;
	rval._Length=0;
}
