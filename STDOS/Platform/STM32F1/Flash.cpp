#include "Flash.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif


// ���ö�����   ע�⣺���������ʱ��������� Flash ����
bool Flash::ReadOutProtection(bool set)
{
	set=set;
	return false;
}
void OnInit()
{
}
// ������ ���ε�ַ��
bool Flash::EraseBlock(uint address) const
{
	return false;
}
// д��
bool Flash::WriteBlock(uint address, const byte* buf, int len, bool inc) const
{
	return false;
}
