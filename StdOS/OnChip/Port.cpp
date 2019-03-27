#include "Port.h"
#include "stm32f10x.h"
#include "Platform/STM32F1/Pin_STM32F1.h"

Port::Port()
{
	this->_Pin = P0;
	this->Opened = false;
}

/*
单一引脚初始化
 */
Port &Port::Set(Pin pin)
{
	if (this->_Pin != pin)
	{
		if (this->_Pin != P0)
			this->Close();
		this->_Pin = pin;

	}
	return  *this;
}

bool Port::Empty()const
{
	return _Pin == P0;
}

void Port::Close()
{
	if (this->Opened)
	{
		if (this->_Pin != P0)
		{
			this->Opened = false;
		}
	}
}

bool Port::ReadInput()const
{
	if (this->Empty())
		return false;
	else
		return Port::Read();
}
// 读取本组所有引脚，任意脚为true则返回true，主要为单一引脚服务
GPIO_TypeDef *IndexToGroup(uint8_t index);

void Port_OnOpen(Pin pin)
{
	int pinindex;
	int portname;

	#if defined STM32F0
	#elif defined STM32F1

	if (pin == PA15 || pin == PB3 || pin == PB4)
	{
		if (pin == P0)
			pinindex = 48;
		else
			pinindex = pin & 0xF;

		if (pin == P0)
			portname = 95;
		else
			portname = (pin >> 4) + 'A';
		//debug_printf("Close JTAG Pin P%c%d \r\n", portname, pinindex);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << (pin >> 4), ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	}
	#elif defined STM32F4
	#endif
	pinindex = pinindex;
	portname = portname;
}

void OpenPeriphClock(Pin pin)
{
	#if defined STM32F0
	#elif defined STM32F1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << (pin >> 4), ENABLE);
	#elif defined STM32F4
	#endif
}

bool Port::Open()
{
#if defined STM32F0
	if (this->Opened == false)
	{
		if (_Pin != P0)
		{
			// 打开时钟
			int gi = _Pin >> 4;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA << gi, ENABLE);

			GPIO_InitTypeDef gpio;
			// 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
			GPIO_StructInit(&gpio);
			gpio.GPIO_Pin = 1 << (this->_Pin & 0x0F);
			this->OnOpen(&gpio);

			GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
		}

		this->Opened = true;
	}
	return true;
#elif defined STM32F1
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
			//this->OnOpen(&gpio);

			GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
		}

		this->Opened = true;
	}
	return true;
#elif defined STM32F4
	if (this->_Pin == P0)
	{
		return false;
	}
	else if (this->Opened)
	{
		return true;
	}
	else
	{
		// 打开时钟
		int gi = _Pin >> 4;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);

		GPIO_InitTypeDef gpio;
		// 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
		GPIO_StructInit(&gpio);
		gpio.GPIO_Pin = 1 << (this->_Pin & 0x0F);
		this->OnOpen(&gpio);

		GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
		this->Opened = true;
		return true;
	}
#endif
}
///////////////////////////////以下为添加///////////////////////////////////////
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

GPIO_TypeDef *IndexToGroup(uint8_t index)
{
#if defined STM32F0
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
#elif defined STM32F1
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
#elif defined STM32F4
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
#endif
}

uint8_t GroupToIndex(GPIO_TypeDef *group)
{
#if defined STM32F0
	return (uint8_t)(((int)group - GPIOA_BASE) >> 10);
#elif defined STM32F1
	return (uint8_t)(((int)group - GPIOA_BASE) >> 10);
#elif defined STM32F4
	return (uint8_t)(((int)group - GPIOA_BASE) >> 10);
#endif
}

void Port::Write(Pin pin, bool value)
{
#if defined STM32F0
	if (value)
	{
		GPIO_SetBits(_GROUP(pin), _PORT(pin));
	}
	else
	{
		GPIO_ResetBits(_GROUP(pin), _PORT(pin));
	}
#elif defined STM32F1
	if (value)
	{
		GPIO_SetBits(_GROUP(pin), _PORT(pin));
	}
	else
	{
		GPIO_ResetBits(_GROUP(pin), _PORT(pin));
	}
#elif defined STM32F4
	if (value)
	{
		GPIO_SetBits(_GROUP(pin), _PORT(pin));
	}
	else
	{
		GPIO_ResetBits(_GROUP(pin), _PORT(pin));
	}
#endif
}

void Port::Write(bool value)const
{
#if defined STM32F0
	if (this->_Pin == P0)
		return;	
	{
		if (value)
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
		else
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
	}
#elif defined STM32F1
	if (this->_Pin == P0)
		return;	
	
		if (value)
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
		else
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}	
#elif defined STM32F4
	if (this->_Pin == P0)
		return;		
		if (value)
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
		else
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}	
#endif
}

bool Port::Read()const
{
#if defined STM32F0
	GPIO_TypeDef *group = _GROUP(this->_Pin);
	return (group->IDR >> (this->_Pin & 0xF)) & 1;
#elif defined STM32F1
	GPIO_TypeDef *group = _GROUP(this->_Pin);
	return (group->IDR >> (this->_Pin & 0xF)) & 1;
#elif defined STM32F4
	GPIO_TypeDef *group = _GROUP(this->_Pin);
	return (group->IDR >> (this->_Pin & 0xF)) & 1;
#endif
}
//设置管脚模式
void Port::SetPinMode(GPIOMode_T mode)
{
	if (_Pin != P0)
	{
		GPIO_InitTypeDef gpio;
		// 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
		GPIO_StructInit(&gpio);
		gpio.GPIO_Pin = 1 << (this->_Pin & 0x0F);
#if defined STM32F0
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
#elif defined STM32F1
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		gpio.GPIO_Mode =(GPIOMode_TypeDef) mode;
#elif defined STM32F4
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
#endif
		GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
	}
}

