#include "Port.h"

#if defined(STM32F4)
#include "stm32f4xx.h"
#include "Platform\STM32F4\Pin_STM32F4.h"
#elif defined(STM32F2)
#include "stm32f2xx.h"
#elif defined(STM32F1)
#include "stm32f10x.h"
#include "Platform/STM32F1/Pin_STM32F1.h"
#elif defined(STM32F3)
#include "stm32f3xx.h"
#elif defined(STM32F0)
#include "stm32f0xx.h"
#include "Platform/STM32F0/Pin_STM32F0.h"
#elif defined(GD32F150)
#include "stm32f0xx.h"
#else
#error "请在Keil项目配置C/C++页定义芯片平台，如STM32F0/STM32F1/STM32F2/STM32F3/STM32F4/GD32F150"
#endif

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

OutputPort::OutputPort()
{
	this->Invert = 2;
	this->OpenDrain = 0;
	this->Speed = 50;
}

OutputPort::OutputPort(Pin pin, uint8_t invert, bool openDrain, uint8_t speed)
{
	this->Opened = false;

	this->Invert = 2;
	this->OpenDrain = false;
	this->Speed = 50;
	this->OpenDrain = openDrain;
	this->Speed = speed;
	this->Invert = invert;
	if (pin != P0)
	{
		this->Set(pin);
		this->Open();
	}
}

bool OutputPort::ReadInput()const
{
	if (this->Empty())
		return false;
	else
		return this->Invert ? !Port::Read() : Port::Read();
}

void OutputPort::OnOpen(void *param)
{
	Port::OnOpen(param);
	this->OpenPin(param);
}

AlternatePort::AlternatePort() : OutputPort(P0, 0, false, 50)
{}

// 读取本组所有引脚，任意脚为true则返回true，主要为单一引脚服务
bool InputPort::Read()const
{
	return this->Invert ? !Port::Read() : Port::Read();
}

bool OutputPort::Read()const
{
	return this->Invert ? !Port::Read() : Port::Read();
}
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
			this->OnOpen(&gpio);

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

void OutputPort::OpenPin(void *param)
{
#if defined STM32F0
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;

	gpio->GPIO_Mode = GPIO_Mode_OUT; //普通输出模式	
	if (this->OpenDrain)
	{
		gpio->GPIO_OType = GPIO_OType_OD;
		gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;/*设置引脚模式为无上拉*/
	}
	else
	{
		gpio->GPIO_OType = GPIO_OType_PP;//通用推挽输出			
		gpio->GPIO_PuPd = GPIO_PuPd_UP;/*设置引脚模式为上拉*/
	}
#elif defined STM32F1
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;

	if (this->OpenDrain)
	{
		gpio->GPIO_Mode = GPIO_Mode_Out_OD;
	}
	else
	{
		gpio->GPIO_Mode = GPIO_Mode_Out_PP;
	}
#elif defined STM32F4
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;

	gpio->GPIO_Mode = GPIO_Mode_OUT; //普通输出模式	
	if (this->OpenDrain)
	{
		gpio->GPIO_OType = GPIO_OType_OD; //推挽输出
		gpio->GPIO_PuPd = GPIO_PuPd_NOPULL; //            
	}
	else
	{
		gpio->GPIO_OType = GPIO_OType_PP; //推挽输出
		gpio->GPIO_PuPd = GPIO_PuPd_UP; //上拉
	}
#endif
}
void AlternatePort::OpenPin(void *param)
{
#if defined STM32F0
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Mode = GPIO_Mode_AF;
	gpio->GPIO_Speed = GPIO_Speed_50MHz;
	gpio->GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
	if (!this->OpenDrain)
	{
		gpio->GPIO_PuPd = GPIO_PuPd_UP;
	}
	else
	{
		gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;
	}
	int i = 0;
	i++;
#elif defined STM32F1
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Mode = this->OpenDrain ? GPIO_Mode_AF_OD : GPIO_Mode_AF_PP;
	int i = 0;
	i++;
#elif defined STM32F4
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Mode = GPIO_Mode_AF;
	gpio->GPIO_Speed = GPIO_Speed_50MHz;
	gpio->GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
	if (!this->OpenDrain)
	{
		gpio->GPIO_PuPd = GPIO_PuPd_UP;
	}
	else
	{
		gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;
	}
	int i = 0;
	i++;
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

void OutputPort::Write(Pin pin, bool value)
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

void InputPort::OnOpen(void *param)
{
#if defined STM32F0
	Port::OnOpen(param);
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Mode = GPIO_Mode_IN;
	if (this->Floating)
	{
		gpio->GPIO_OType = GPIO_OType_OD;
	}
	else
	{
		gpio->GPIO_OType = GPIO_OType_PP;
	}
	switch (this->Pull)
	{
	case NOPULL:
		gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;
	case UP:
		gpio->GPIO_PuPd = GPIO_PuPd_UP;
		break;
	case DOWN:
		gpio->GPIO_PuPd = GPIO_PuPd_DOWN;
		break;
	default:
		break;
	}
#elif defined STM32F1
	Port::OnOpen(param);
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	if (Floating)
		gpio->GPIO_Mode = GPIO_Mode_IN_FLOATING;
	else if (Pull == UP)
		gpio->GPIO_Mode = GPIO_Mode_IPU;
	else if (Pull == DOWN)
		gpio->GPIO_Mode = GPIO_Mode_IPD;
	// 这里很不确定，需要根据实际进行调整     
#elif defined STM32F4
	Port::OnOpen(param);
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Mode = GPIO_Mode_IN;
	if (this->Floating)
	{
		gpio->GPIO_OType = GPIO_OType_OD;
	}
	else
	{
		gpio->GPIO_OType = GPIO_OType_PP;
	}
	switch (this->Pull)
	{
	case NOPULL:
		gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;
	case UP:
		gpio->GPIO_PuPd = GPIO_PuPd_UP;
		break;
	case DOWN:
		gpio->GPIO_PuPd = GPIO_PuPd_DOWN;
		break;
	default:
		break;
	}
#endif
}

void OutputPort::Write(bool value)const
{
#if defined STM32F0
	if (this->_Pin == P0)
		return;
	if (this->Invert)
	{
		if (value)
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
		else
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
	}
	else
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
	if (this->Invert)
	{
		if (value)
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
		else
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
	}
	else
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
#elif defined STM32F4
	if (this->_Pin == P0)
		return;
	if (this->Invert)
	{
		if (value)
		{
			GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
		else
		{
			GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
		}
	}
	else
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
#endif
}

void Port::OnOpen(void *param)
{
#if defined STM32F0
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Speed = GPIO_Speed_50MHz;
#elif defined STM32F1
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Speed = GPIO_Speed_50MHz;
#elif defined STM32F4
	GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
	gpio->GPIO_Speed = GPIO_Speed_100MHz;
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
