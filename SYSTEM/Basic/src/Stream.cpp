#include "Stream.h"

//����������
uint Stream::Capacity() const
{
    return 0;
}

void Stream::SetCapacity(uint len){}
//��ǰλ��
uint Stream::Position() const
{
    return 0;
}

//����λ��
bool Stream::SetPosition(int p)
{
    return false;
}

//���µ���Ч���������ȣ�0��ʾ�Ѿ������յ�
uint Stream::Remain() const
{
    return 0;
}

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

//��ȡ7Ϊѹ����������
uint Stream::ReadEncodeInt()
{
    return 0;
}

//��ȡ���ݵ��ֽ����飬�����ֽ�����ָ����С������������ǰ׺
uint Stream::Read(Buffer &bs)
{
    return 0;
}

//д��7λѹ����������
uint Stream::WriteEncodeInt(uint value)
{
    return 0;
}

//���ֽ����������д�뵽������������������ǰ׺
bool Stream::Write(const Buffer &bs)
{
	return false;
}

//����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
uint Stream::ReadArray(Buffer &bs)
{
    return 0;
}

ByteArray Stream::ReadArray(int count)
{
	byte buf[3];
    return ByteArray(buf,3);
}

//���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
bool Stream::WriteArray(const Buffer &bs)
{
    return false;
}
