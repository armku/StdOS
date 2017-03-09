#include <stdio.h>
#include "Buffer.h"
#include "Sys.h"

Buffer::Buffer(void *ptr, int len)
{
    this->_Arr = ptr;
    this->_Length = len;
}

Buffer &Buffer::operator = (const void *prt)
{
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = ((byte*)prt)[i];
    }
    return  *this;
}

Buffer &Buffer::operator = (Buffer && rval)
{
    if (this->_Length < rval._Length)
    {
        debug_printf("Error: Buffer copy: Buffer length mismath src: %d ,dst: %d \n", rval._Length, this->_Length);
    }
    else
    {
        for (int i = 0; i < rval.Length(); i++)
        {
            ((byte*)(this->_Arr))[i] = rval.GetBuffer()[i];
        }
    }
    return  *this;
}

byte &Buffer::operator[](int pos)
{
    if ((pos < 0) || (pos > this->_Length))
    {
        debug_printf("Error: [] length error");
        return ((byte*)(this->_Arr))[0];
    }
    return ((byte*)(this->_Arr))[pos];
}

//���ó��ȣ����Զ����� 
bool Buffer::SetLength(int len)
{
    if (this->_Length >= len)
    {
        this->_Length = len;
    }
    else
    {
        //�Զ�����
        this->_Arr = new byte[len];
        this->_Length = len;
        delete []((byte*)(this->_Arr));
    }
    return true;
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

//�����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
void Buffer::CopyTo(int destIndex, const void *dest, int len)
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

//��ȡ�Ի�����  
Buffer Buffer::Sub(int index, int length)
{
    byte *pbufsub = new byte[length];
    if (length > this->_Length)
    {
        length = this->_Length;
    }
    Buffer buf(pbufsub, length);

    for (int i = index; i < (index + length); i++)
    {
        pbufsub[i] = this->GetBuffer()[index + i];
    }
    return buf;
}
