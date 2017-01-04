#include "stm32f10x.h"
#include "Port.h"
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

Port::Port()
{
	this->_Pin=P0;
}
// 设置引脚，并应用配置。
Port& Port::Set(Pin pin)
{
	this->_Pin = pin;
	
	return *this;
}






























