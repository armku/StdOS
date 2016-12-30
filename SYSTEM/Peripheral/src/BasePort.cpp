#include "stm32f10x.h"
#include "BasePort.h"
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

BasePort::BasePort(PinPort pin)
{
    this->pin = pin;
}
//写入值，true：高电平，false：低电平
void BasePort::Write(const bool value)
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
void BasePort::operator = (const bool value)
{
	this->Write(value);
}
//写入值，其他:高电平 0:低电平
void BasePort::operator = (const byte value)
{
	this->Write((bool)value);
}
////引脚模式
void BasePort::SetMode(PIN_MODE mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(_RCC_APB2(pin), ENABLE);

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
    GPIO_InitStructure.GPIO_Pin = _PORT(this->pin);

    GPIO_Init(_GROUP(pin), &GPIO_InitStructure);
}

void BasePort::SetModeAIN()
{
    this->SetMode(AIN);
}

void BasePort::SetModeIN_FLOATING()
{
    this->SetMode(INPUT);
}

void BasePort::SetModeINPUT_IPD()
{
    this->SetMode(INPUT_PD);
}

void BasePort::SetModeINPUT_IPU()
{
    this->SetMode(INPUT_PU);
}

void BasePort::SetModeOut_OD()
{
    this->SetMode(OUTPUT_OD);
}

void BasePort::SetModeOut_PP()
{
    this->SetMode(OUTPUT_PP);
}

void BasePort::SetModeAF_OD()
{
    this->SetMode(AF_OD);
}

void BasePort::SetModeAF_PP()
{
    this->SetMode(AF_PP);
}

void BasePort::Set()
{
    this->pinbit = 1;
    GPIO_SetBits(_GROUP(pin), _PORT(pin));
}

void BasePort::Reset()
{
    this->pinbit = 0;
    GPIO_ResetBits(_GROUP(pin), _PORT(pin));
}

byte BasePort::Read(void)
{
    return GPIO_ReadInputDataBit(_GROUP(pin), _PORT(pin));
}
byte BasePort::Read1(void)
{
    return GPIO_ReadInputDataBit(_GROUP(pin), _PORT(pin));
}
