/*
������ Object
�ο� C#�� Object �࣬ϵͳ�д󲿷��඼�̳��� Object
��Ҫ�ṩ����ת�ַ����Լ�������Դ�ӡ�ĳ���ӿڡ�
����ת�ַ��� ToStr/ToString ��Ĭ��ʵ��ʱ��ʾ������
��ʾ���� Show ��Ĭ��ʵ����ͨ�����ӵĵ��Դ��������

 */
#pragma once

class CType;
class String;

//������
class Object
{
    public:
        #if 0
            //���������ַ�����ʾ��ʽ
            virtual String &ToStr(String &str)const;
		#endif
		#if 0
        //���������ַ�����ʾ��ʽ.֧��RVO�Ż�
        virtual String ToString()const;        
		#endif
        //��ʾ����Ĭ����ʾ ToString
        virtual void Show(bool newLine = false)const;
        const CType GetType()const;
};
