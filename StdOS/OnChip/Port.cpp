#include "Port.h"

#include "stm32f10x.h"
#include "Platform/STM32F1/Pin_STM32F1.h"

#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))
GPIO_TypeDef *IndexToGroup(uint8_t index);
uint8_t GroupToIndex(GPIO_TypeDef *group);
void Port_OnOpen(Pin pin);

OutputPort::OutputPort()
{
	this->Invert = 2;
	this->OpenDrain = 0;
}

OutputPort::OutputPort(Pin pin, uint8_t invert, bool openDrain)
{
	this->Opened = false;

	this->Invert = 2;
	this->OpenDrain = false;
	this->OpenDrain = openDrain;
	this->Invert = invert;
	if (pin != P0)
	{
		this->Set(pin);
		this->Open();
	}
}

void OutputPort::OnOpen(void *param)
{
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Speed = GPIO_Speed_50MHz;
	
	if (this->OpenDrain)
	{
		gpio->GPIO_Mode = GPIO_Mode_Out_OD;
	}
	else
	{
		gpio->GPIO_Mode = GPIO_Mode_Out_PP;
	}
}
