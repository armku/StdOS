#include "Kernel\Sys.h"
#include "Port.h"

#include <typeinfo>
using namespace ::std;

/******************************** Port ********************************/

#if DEBUG
// 保护引脚，别的功能要使用时将会报错。返回是否保护成功
static bool Port_Reserve(Pin pin, bool flag);
#endif

// 端口基本功能
#define REGION_Port 1


Port::Port()
{
	this->_Pin = P0;
	this->Opened = false;
	this->Invert=false;
}


#ifndef TINY
Port::~Port()
{
	//Close();
}
#endif


String Port::ToString() const
{
	String str;
	str += 'P';
	if (_Pin == P0)
	{
		str += '0';
	}
	else
	{
		str += (char)('A' + (_Pin >> 4));
		str += (byte)(_Pin & 0x0F);
	}
	return str;
}

// 单一引脚初始化
Port& Port::Set(Pin pin)
{
	// 如果引脚不变，则不做处理
	if (pin == _Pin) return *this;

#ifndef TINY
	// 释放已有引脚的保护
	if (_Pin != P0) Close();
#endif

	_Pin = pin;

	return *this;
}

bool Port::Empty() const
{
	if (_Pin != P0) return false;

	return true;
}

void Port::Clear()
{
	_Pin = P0;
}



void Port::Close()
{
	if (!Opened) return;
	if (_Pin == P0) return;

//	OnClose();

#if DEBUG
	// 保护引脚
	auto name = typeid(*this).name();
	while (*name >= '0' && *name <= '9') name++;
	debug_printf("%s", name);
//	Port_Reserve(_Pin, false);
	debug_printf("\r\n");
#endif

	Opened = false;
}










#include "Port.h"
#include "stm32f10x.h"

#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

GPIO_TypeDef* GPIO_TypeDefGroup[] = { GPIOA ,GPIOB,GPIOC ,GPIOD ,GPIOE ,GPIOF,GPIOG };

GPIO_TypeDef *IndexToGroup(byte index)
{
#if defined STM32F0
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
#elif defined STM32F1
	return GPIO_TypeDefGroup[index];
#elif defined STM32F4
	return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
#endif
}



Port &Port::SetPin(Pin pin)
{
	if (this->_Pin != pin)
	{
		if (this->_Pin != P0)
			this->Close();
		this->_Pin = pin;

	}
	return  *this;
}

//设置管脚模式
void Port::pinMode(GPIOMode_T mode)
{
	if (this->Empty())
		return;
	//***必须先开启时钟，否则不正常
#if defined STM32F0
	// 打开时钟
	int gi = _Pin >> 4;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA << gi, ENABLE);
#elif defined STM32F1
	// 打开时钟
	int gi = _Pin >> 4;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
	// PA15/PB3/PB4 需要关闭JTAG
	switch (_Pin)
	{
	case PA15:
	case PB3:
	case PB4:
		//debug_printf("Close JTAG for P%c%d\r\n", _PIN_NAME(_Pin));
		// PA15是jtag接口中的一员 想要使用 必须开启remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		break;
	}
#elif defined STM32F4
	// 打开时钟
	int gi = _Pin >> 4;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);
#endif

#if defined STM32F0
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
#elif defined STM32F1
	GPIO_InitTypeDef gpio;
	// 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = 1 << (this->_Pin & 0x0F);

	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = (GPIOMode_TypeDef)mode;

	GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
#elif defined STM32F4
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
#endif

	this->Opened = true;
}



bool Port::ReadInput()const
{
	if (this->Empty())
		return false;
	else
		return Port::Read();
}

void Port::Write(bool value)const
{
	if (this->Empty())
		return;
#if defined STM32F0	
	if (value)
	{
		GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
	}
	else
	{
		GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
	}
#elif defined STM32F1
	if (this->Invert == false)
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
	else
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
#elif defined STM32F4
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
	if (this->Empty())
		return false;
#if defined STM32F0
	GPIO_TypeDef *group = _GROUP(this->_Pin);
	return (group->IDR >> (this->_Pin & 0xF)) & 1;
#elif defined STM32F1
	GPIO_TypeDef *group = _GROUP(this->_Pin);
	bool ret= (group->IDR >> (this->_Pin & 0xF)) & 1;
	return this->Invert ? (!ret) : ret;
#elif defined STM32F4
	GPIO_TypeDef *group = _GROUP(this->_Pin);
	return (group->IDR >> (this->_Pin & 0xF)) & 1;
#endif
}
