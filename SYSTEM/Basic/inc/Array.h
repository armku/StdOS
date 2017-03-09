/*
���� Array
�䳤���� Array �̳��Ի����� Buffer����Ҫ�ṩ�Զ����ݹ��ܡ�ִ�и���
�������洢�ռ䲻��ʱ���Զ������ڴ����ݣ���������ʱ�Զ��ͷ��ڴ档
���ʹ�ÿ�ָ�빹�� Array����ô��һ��ʹ��ʱ�ͻ������ڴ档
*/
#pragma once

#include "Buffer.h"

template <class T>
int ArrayLength(T& array)
{
  return (sizeof(array) / sizeof(array[0]));
}
//��Ҫ�ṩ�Զ����ݹ���
//����
class Array:public Buffer
{
	public:		
		Array(void* ptr,int len):Buffer(ptr,len){}
};
