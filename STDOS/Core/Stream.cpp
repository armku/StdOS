#include "Stream.h"
#include <stdio.h>

#if 0
//����������
uint Stream::Capacity() const
{
    return 0;
}
#endif
#if 0
void Stream::SetCapacity(uint len){}
#endif
#if 0
//��ǰλ��
uint Stream::Position() const
{
    return 0;
}
#endif
//����λ��
bool Stream::SetPosition(int p)
{
    return false;
}
#if 0
//���µ���Ч���������ȣ�0��ʾ�Ѿ������յ�
uint Stream::Remain() const
{
    return 0;
}
#endif
//����ǰ���ƶ�һ�ξ��룬���سɹ�����ʧ�ܡ����ʧ�ܣ����ƶ��α�
bool Stream::Seek(int offset)
{
    return false;
}

//������ָ�룬ע�⣺���ݺ�ָ���ı�
byte *Stream::GetBuffer() const
{
    return NULL;
}

//������udagnqianλ��ָ�롣ע�⣺���ݺ�ָ���ı�
byte *Stream::Current() const
{
    return NULL;
}
#if 0
//��ȡ7Ϊѹ����������
uint Stream::ReadEncodeInt()
{
    return 0;
}
#endif
#if 0
//��ȡ���ݵ��ֽ����飬�����ֽ�����ָ����С������������ǰ׺
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
//д��7λѹ����������
uint Stream::WriteEncodeInt(uint value)
{
    return 0;
}
#endif
//���ֽ����������д�뵽������������������ǰ׺
bool Stream::Write(const Buffer &bs)
{
	return false;
}
#if 0
//����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
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
//���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
bool Stream::WriteArray(const Buffer &bs)
{
    return false;
}
