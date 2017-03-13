#include <stdio.h>
#include "Buffer.h"
#include "Sys.h"

Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = ptr;
    this->_Length = len;
    this->_LengthInit = len;
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

//���ó��ȣ����Զ����� 
bool Buffer::SetLength(int len)
{
    if (this->_LengthInit >= len)
    {
        this->_Length = len;
    }
    return true;
}

//����ָ��λ�õ�ֵ�����Ȳ���ʱ����
bool Buffer::SetAt(int index, byte value)
{
    if (index >= this->_Length)
    {
        return false;
    }
    ((byte*)this->_Arr)[index] = value;
    return true;
}

//�������������[] ����ָ��Ԫ�صĵ�һ���ֽ�
byte Buffer::operator[](int i)const
{
    return ((byte*)this->_Arr)[i];
}

//֧��buf[i]=0x33���﷨
byte &Buffer::operator[](int i)
{
    if ((i < 0) || (i > this->_Length))
    {
        debug_printf("Error: [] length error");
        return ((byte*)(this->_Arr))[0];
    }
    return ((byte*)(this->_Arr))[i];
}

//ԭʼ���� ���� �����߽�
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

//�������ݣ�Ĭ��-1���ȱ�ʾ��ǰ���� 
void Buffer::Copy(int destIndex, const void *src, int len)
{
    if (len ==  - 1)
    {
        len = this->_Length;
    }
    if (len <= 0)
    {
        return ;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)(this->_Arr))[destIndex + i] = ((byte*)src)[i];
    }
}

//�����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
void Buffer::CopyTo(int destIndex, const void *dest, int len)const
{
    if (len ==  - 1)
    {
        len = this->_Length;
    }
    if (len <= 0)
    {
        return ;
    }
    for (int i = 0; i < len; i++)
    {
        ((byte*)dest)[destIndex + i] = ((byte*)(this->_Arr))[i];
    }
}

//�������ݣ�Ĭ��-1���ȱ�ʾ������С���� 
void Buffer::Copy(int destIndex, const Buffer &src, int srcIndex, int len)
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

//��ָ���ֽ����ó�ʼ��һ������
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

//��ȡһ���ӻ�������Ĭ��-1���ȱ�ʾʣ��ȫ��		
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
    //��ʾ16��ֹ���ݣ�ָ���ָ��ַ��ͻ��г���
    String &Buffer::ToHex(String &str, char sep, int newLine)const
    {
        return  *String("Fuck", 5);
    }

    //��ʾ16��ֹ���ݣ�ָ���ָ��ַ��ͻ��г���
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
    //���������ַ�����ʾ��ʽ
    String &Buffer::ToStr(String &str)const{}
    //��װΪ�ַ�������
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









Buffer::Buffer(){}
String &Buffer::ToStr(String &str)const
{
    String *ret = new String();
    return  *ret;
}

void *Buffer::Alloc(int len)
{
    return this->_Arr;
}

#if DEBUG
    void Buffer::Test()
    {
        //ʹ��ָ��ͳ��ȹ���һ���ڴ���
        char cs[] = "This is Buffer Test.";
        Buffer bs0(cs, sizeof(cs));
        debug_printf("Buffer bs (cs,strlen(cs)) => %s\r\n", cs);
        assert(bs0.GetBuffer() == (byte*)cs, "GetBuffer()");
        assert(bs0 == cs, "Buffer(void* p = nullptr, int len = 0)");

        byte buf0[] = 
        {
            1, 2, 3, 4
        };
        byte bts0[] = 
        {
            5, 6, 7, 8, 9, 10
        };
        Buffer bs1(buf0, sizeof(buf0));

        char err[] = "Buffer& operator = (const void* ptr)";

        //��ָ�뿽����ʹ���ҵĳ���
        bs1 = bts0;
        assert(bs1.GetBuffer() == buf0, err);
        assert(bs1.GetBuffer() != bts0, err);
        assert(bs1.Length() == sizeof(buf0), err);
        assert(buf0[0] == bts0[0] && buf0[3] == bts0[3], err);

        byte buf1[] = 
        {
            1, 2, 3, 4
        };
        byte bts1[] = 
        {
            5, 6, 7
        };
        Buffer bs2(buf1, sizeof(buf0));
        Buffer bs22(bts0, sizeof(bts0));

        char err1[] = "Buffer& operator = (const Buffer& rhs)";

        //����һ�����󿽱����ݺͳ��� ���Ȳ���������ʧ��ʱ����
        //Buffer �޷��Զ����ݣ�Array����
        //bs2 = bs;
        bs2 = bs22;
        assert(bs2.GetBuffer() == buf1, err1);
        assert(bs2.GetBuffer() != bts1, err1);
        assert(bs2.Length() == sizeof(bts1), err1);
        assert(bs22.Length() == sizeof(buf1), err1);
        assert(buf1[0] == bts1[0] && buf1[2] == bts1[2], err1);
        assert(buf1[3] == 4, err1);


    }
#endif
