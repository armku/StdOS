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

bool Port::Open()
{
	if (this->Opened == false)
	{
		if (_Pin != P0)
		{
			// 打开时钟
			int gi = _Pin >> 4;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
			// PA15/PB3/PB4 需要关闭JTAG
			switch (_Pin)
			{
			case PA15:
			case PB3:
			case PB4:
			{
				//debug_printf("Close JTAG for P%c%d\r\n", _PIN_NAME(_Pin));
				// PA15是jtag接口中的一员 想要使用 必须开启remap
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
				GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
				break;
			}
			}

			GPIO_InitTypeDef gpio;
			// 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
			GPIO_StructInit(&gpio);
			gpio.GPIO_Pin = 1 << (this->_Pin & 0x0F);
			gpio.GPIO_Speed = GPIO_Speed_50MHz;

			GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
		}

		this->Opened = true;
	}
	return true;
}
