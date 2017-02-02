/*
�ַ��� String
�ַ��� String �̳��� Array���ڲ��洢ָ��ͳ��ȡ��ر�ע�⣬�ڲ���Ϊ
��β��\0 �����洢�ռ䣬��ȷ��ȡ�õ��ַ���ָ���Ǳ�׼ C ��ʽ�ַ���ָ�롣
�ο� C#�� String �࣬���л������Ͷ�����תΪ�ַ�����֧���ַ�����ֵ��
�������ӡ���ȱȽϡ�ת���֡���ȡ�ȡ�
1) ��������תΪ�ַ�
3) ����תΪʮ�����ַ���
5) ����תΪʮ�������ַ���
6) �ַ������ַ�������Ϊ���ݿ�����������ȣ�ָ�벻ͬ
7) �ַ�������֧�ֻ������͡�����Ĭ����ʮ���Ʒ�ʽ���ӡ�
***�ر�ע�⣬Ϊ�˱������������Ƭ�����ַ�������ʵ��������Դ�ַ�
�����ۼӣ���ı�Դ�ַ�������һ���� C#/Java/C++���ַ�������Ʋ�ͬ��
9) �ַ�����ʮ��������������
10) ʮ�������ַ���תΪ����������
11) �ַ�������
13) �ַ�����ȡ
14) �ַ����ָ� Split
������ C++�������ԣ��޷����� C#�汾 String.Split��������ö�ٽṹ��

*/
#pragma once

#include "Array.h"
#include "DateTime.h"

//�ڲ���Ϊ
//��β��\0 �����洢�ռ䣬��ȷ��ȡ�õ��ַ���ָ���Ǳ�׼ C ��ʽ�ַ���ָ��
//�Դ�64�ֽڵĻ�������
class String:public Array
{
	public:
		String();
		String(const char * str,int length=-1);
		String(char ch);
		String(const String& str1);
		String(byte value,byte radix=10);//�����ַ���
		String(short value,byte radix=10);//�����ַ���		
		String(int value,byte radix=10);//�����ַ���
		String(uint value,byte radix=10);//�����ַ���
		String(Int64 value,byte radix=10);//�����ַ���
		String(UInt64 value,byte radix=10);//�����ַ���
		String(float value,byte dot=2);//������
		String(double value,byte dot=2);//������
		
		~String();
		
		String& operator=(const char* str) ;
		String& operator=(const String& str) ;
		bool operator==(const char* str) ;
		bool operator==(const String& str);
	
		String& operator+=(const DateTime& dt);
		String& operator+=(const String& str);
		String& operator+=(const float da);
		
		String& Concat(byte value,byte radix=10);
		String& Concat(short value,byte radix=10);
		String& Concat(int value,byte radix=10);
		String& Concat(uint value,byte radix=10);
		String& Concat(Int64 value,byte radix=10);
		String& Concat(UInt64 value,byte radix=10);
		String& Concat(float value,byte dot=2);
		String& Concat(double value,byte dot=2);
		
		String ToHex();
		virtual void Show(bool newLine=true) const;
		uint Capacity() const;//��������
	private:
		void initCapacity();//��ʼ������
		void jsLength();//�����ַ�������
		short mcapacity;//�ַ�������
		
		
};
