#include "TTime.h"

TTime::TTime()
{
}
//���ص�ǰ������
uint TTime::Ms()
{
	return this->_Current;
}
//����ϵͳ��ǰʱ��,Unix��ʽ����
void TTime::SetTime(UInt64 seconds)
{
	
}
//ϵͳʱ��
uint TTime::Seconds()
{
	return 0;
}
//��ʱms
void TTime::Sleep(uint ms)
{
	
}
//ϵͳ���������ĺ�����
uint64_t TTime::Current()
{
	return this->_Current;
}
