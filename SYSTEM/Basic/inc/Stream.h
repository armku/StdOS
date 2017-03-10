#pragma once

#include "ByteArray.h"

class Stream: public Object
{
    public:
        //����������
        uint Capacity()const;
        void SetCapacity(uint len);
        //��ǰλ��
        uint Position()const;
        //����λ��
        bool SetPosition(int p);
        //���µ���Ч���������ȣ�0��ʾ�Ѿ������յ�
        uint Remain()const;
        //����ǰ���ƶ�һ�ξ��룬���سɹ�����ʧ�ܡ����ʧ�ܣ����ƶ��α�
        bool Seek(int offset);

        //������ָ�룬ע�⣺���ݺ�ָ���ı�
        byte *GetBuffer()const;
        //������udagnqianλ��ָ�롣ע�⣺���ݺ�ָ���ı�
        byte *Current()const;

        //��ȡ7Ϊѹ����������
        uint ReadEncodeInt();
        //��ȡ���ݵ��ֽ����飬�����ֽ�����ָ����С������������ǰ׺
        uint Read(Buffer &bs);

        //д��7λѹ����������
        uint WriteEncodeInt(uint value);
        //���ֽ����������д�뵽������������������ǰ׺
        bool Write(const Buffer &bs);

        //����������ȡ�䳤���ݵ��ֽ����顣��ѹ��������ͷ��ʾ����
        uint ReadArray(Buffer &bs);
        ByteArray ReadArray(int count);
        //���ֽ�������Ϊ�䳤����д�뵽����������ѹ��������ͷ��ʾ����
        bool WriteArray(const Buffer &bs);
};
