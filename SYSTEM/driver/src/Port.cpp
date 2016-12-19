#include "stm32f10x.h"
#include "Port.h"
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (uint16_t)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

CPort::CPort(uint16_t pin)
{
    this->pin = pin;
}

CPort::~CPort(){

}
////引脚模式
void CPort::SetMode(PIN_MODE mode)
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

void CPort::SetModeAIN()
{
    this->SetMode(AIN);
}

void CPort::SetModeIN_FLOATING()
{
    this->SetMode(INPUT);
}

void CPort::SetModeINPUT_IPD()
{
    this->SetMode(INPUT_PD);
}

void CPort::SetModeINPUT_IPU()
{
    this->SetMode(INPUT_PU);
}

void CPort::SetModeOut_OD()
{
    this->SetMode(OUTPUT_OD);
}

void CPort::SetModeOut_PP()
{
    this->SetMode(OUTPUT_PP);
}

void CPort::SetModeAF_OD()
{
    this->SetMode(AF_OD);
}

void CPort::SetModeAF_PP()
{
    this->SetMode(AF_PP);
}

void CPort::Set()
{
    this->pinbit = 1;
    GPIO_SetBits(_GROUP(pin), _PORT(pin));
}

void CPort::Reset()
{
    this->pinbit = 0;
    GPIO_ResetBits(_GROUP(pin), _PORT(pin));
}

void CPort::Toggle()
{
    this->pinbit = !this->pinbit;
    if (this->pinbit)
    {
        this->Set();
    }
    else
    {
        this->Reset();
    }
}

uint8_t CPort::Read(void)
{
    return GPIO_ReadInputDataBit(_GROUP(pin), _PORT(pin));
}

void CPort::Write(uint8_t da)
{
    if (da)
    {
        this->Set();
    }
    else
    {
        this->Reset();
    }
}
