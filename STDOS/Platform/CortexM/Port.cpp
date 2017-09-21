#include "Port.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

void Port_OnOpen()
{
  
}
int IndexToGroup(int a1)
{
  return (a1 << 10) + 1073809408;
}

void OutputPort::Write(bool value)const
{
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
}

void Port::OnOpen(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
    #ifdef STM32F0
		gpio->GPIO_Speed = GPIO_Speed_50MHz;
    #elif defined STM32F1
        gpio->GPIO_Speed = GPIO_Speed_50MHz;
    #elif defined STM32F4
        gpio->GPIO_Speed = GPIO_Speed_100MHz;
    #endif 
}

void Port::OnClose(){}

void Port::Opening()
{
}
bool InputPort::OnRegister()
{
}
void InputPort::ClosePin()
{
	
}
bool OutputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}

bool Port::Read()const
{
    GPIO_TypeDef *group = _GROUP(this->_Pin);
    return (group->IDR >> (this->_Pin &0xF)) &1;
}
