/*
������ Object
�ο� C#�� Object �࣬ϵͳ�д󲿷��඼�̳��� Object
��Ҫ�ṩ����ת�ַ����Լ�������Դ�ӡ�ĳ���ӿڡ�
����ת�ַ��� ToStr/ToString ��Ĭ��ʵ��ʱ��ʾ������
��ʾ���� Show ��Ĭ��ʵ����ͨ�����ӵĵ��Դ��������

*/
#pragma once

#include "CString.h"

//������
class Object
{
	public:
		//���������ַ�����ʾ��ʽ
		//virtual String & ToStr(String& str) const;
		//���������ַ�����ʾ��ʽ.֧��RVO�Ż�
		//virtual String& ToString() const;
		//��ʾ����Ĭ����ʾ ToString
		virtual void Show(bool newLine=false) const;
		const CType GetType() const;
};
