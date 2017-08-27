#include "Flash.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif


// 设置读保护   注意：解除读保护时会擦除所有 Flash 内容
bool Flash::ReadOutProtection(bool set)
{
	set=set;
	return false;
}
void OnInit()
{
}
// 擦除块 （段地址）
bool Flash::EraseBlock(uint address) const
{
	return false;
}
// 写块
bool Flash::WriteBlock(uint address, const byte* buf, int len, bool inc) const
{
	return false;
}
