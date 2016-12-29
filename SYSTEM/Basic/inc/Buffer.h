/*
������ Buffer
Buffer ��ʾһ�����������ڲ���һ��ָ���һ��������ɡ�
����ϵͳ��Ʋ������䵥�ֽ�ָ�룬 �Ա������������������ڴ�й©
��Ƿ��޸�������������ݡ�
���ò�����
1) ʹ��ָ��ͳ��ȹ���һ����������GetBuffer ����ָ�룬Length ���س��ȡ�
Buffer ��ֱ����ָ�������ȱȽϡ�
2) ��Դָ�뿽�����ݵ�����������ֵ���ǿ������ݶ������滻�ڲ�ָ�롣
�������̣�bs=bts ����� bts ���� 4 ���ֽڵ� bs��Ҳ���� buf �����ڴ�����
���Դָ���������ڻ��������ȣ� ��ô����Դָ�����ݺ�����ſ���������
�ӻ������������ݵ���һ�����������ر�ע��Դ���������ȴ���Ŀ�껺����
�������
�ı仺�������� SetLength��
SetLength ���԰ѻ������������õĸ�С��������ø������漰�Զ�����
���⡣
��Դ���������ȴ���Ŀ�껺����ʱ��Buffer ����ᱨ������ Buffer ������
Array/ByteArray/String �Ȼ���� SetLength �Զ����ݣ��Ӵ󻺳�����
5) �߼����� Copy/CopyTo��
֧�ֻ������ͻ�������������������ָ��֮��ָ��ƫ�ƺͳ��ȵĸ߼�������
�ر�ע�⣬Ŀ�껺�������Ȳ���ʱ���ο� SetLength ����ԭ��
6) ��ȡ�ӻ����� Sub
7) תΪʮ�������ַ��� ToHex
8) ���������������ݻ��� ToUInt32/Write
9) ����������ֱ����Ϊ�ַ��� AsString()
 */
#pragma once

#include <Type.h>

class Buffer
{
    public:
        //����ָ��
        void GetBuffer();
        //����
        ushort Length();
        //���ó��ȣ����Զ�����
        void SetLength(ushort len);
        //�������ݣ�Ĭ��-1���ȱ�ʾ��ǰ����
        virtual void Copy(int destIndex, const void *src, int len);
        //�����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
        virtual void CopyTo(int destIndex, const void *dest, int len)const;
        //�������ݣ�Ĭ��-1���ȱ�ʾ������С����
        virtual void Copy(int destIndex, const Buffer &sec, int srcIndex, int len);
        //����һ�����󿽱����ݺͳ��ȣ����Ȳ���������ʧ��ʱ����
        int Copy(const Buffer &src, int destIndex = 0);
        //��ȡ�Ի�����
        void Sub();
        //ת��Ϊ16�����ַ���
        void ToHex();
        //����������ֱ����Ϊ�ַ���
        void AsString();

};
