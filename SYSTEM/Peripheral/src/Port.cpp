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





























Port::Port(Pin pin)
{
    this->Set(pin);
}
//写入值，true：高电平，false：低电平
void Port::Write(const bool value)
{
	if(value)
	{
		this->Set();
	}
	else
	{
		this->Reset();
	}
}
//写入值，true:高电平 false:低电平
void Port::operator = (const bool onoff)
{
	this->Write(onoff);
}
//写入值，其他:高电平 0:低电平
void Port::operator = (const int hilo)
{
	this->Write((bool)hilo);
}
////引脚模式
void Port::SetMode(PIN_MODE mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(_RCC_APB2(this->_Pin), ENABLE);

    switch (mode)
    {
        case AIN:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            break;
        case INPUT:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            break;
        case INPUT_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
            break;
        case INPUT_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
            break;
        case OUTPUT_OD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
            break;
        case OUTPUT_PP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            break;
        case AF_OD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
            break;
        case AF_PP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            break;
        default:
            break;
    }
    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = _PORT(this->_Pin);

    GPIO_Init(_GROUP(this->_Pin), &GPIO_InitStructure);
}

void Port::SetModeAIN()
{
    this->SetMode(AIN);
}

void Port::SetModeIN_FLOATING()
{
    this->SetMode(INPUT);
}

void Port::SetModeINPUT_IPD()
{
    this->SetMode(INPUT_PD);
}

void Port::SetModeINPUT_IPU()
{
    this->SetMode(INPUT_PU);
}

void Port::SetModeOut_OD()
{
    this->SetMode(OUTPUT_OD);
}

void Port::SetModeOut_PP()
{
    this->SetMode(OUTPUT_PP);
}

void Port::SetModeAF_OD()
{
    this->SetMode(AF_OD);
}

void Port::SetModeAF_PP()
{
    this->SetMode(AF_PP);
}

void Port::Set()
{
    this->pinbit = 1;
    GPIO_SetBits(_GROUP(_Pin), _PORT(_Pin));
}

void Port::Reset()
{
    this->pinbit = 0;
    GPIO_ResetBits(_GROUP(_Pin), _PORT(_Pin));
}

byte Port::Read(void)
{
    return ReadPinPort(this->_Pin);
}
//读取端口状态
bool Port::ReadPinPort(Pin pin)
{
	 return GPIO_ReadInputDataBit(_GROUP(pin), _PORT(pin));
}
