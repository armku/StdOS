#include "Port.h"
#include "Platform\stm32.h"

GPIO_TypeDef *IndexToGroup(uint8_t index);

bool Port::Open()
{
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
        gpio.GPIO_Pin = 1 << (this->_Pin &0x0F);
        this->OnOpen(&gpio);

        GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
        this->Opened = true;
        return true;
    }
}

void OutputPort::OpenPin(void *param)
{
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
}
void AnalogInPort::OpenPin(void* param)
{
	
}
void AlternatePort::OpenPin(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
         gpio->GPIO_Mode = GPIO_Mode_AF;
		gpio->GPIO_Speed = GPIO_Speed_50MHz;
        gpio->GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
		if(!this->OpenDrain)
		{
			gpio->GPIO_PuPd = GPIO_PuPd_UP;
		}
		else
		{
			gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;
		}
    int i = 0;
    i++;
}
void Port::RemapConfig(uint32_t param, bool sta)
{
	
}
void InputPort::OpenPin(void* param)
{
}
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////以下为添加///////////////////////////////////////
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

    #if 0
        static const int PORT_IRQns[] = 
        {
            EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
            EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
            EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
        };
    #endif 
GPIO_TypeDef *IndexToGroup(uint8_t index)
{
    return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
}

uint8_t GroupToIndex(GPIO_TypeDef *group)
{
    return (uint8_t)(((int)group - GPIOA_BASE) >> 10);
}

void OutputPort::Write(Pin pin, bool value)
{
    if (value)
    {
        GPIO_SetBits(_GROUP(pin), _PORT(pin));
    }
    else
    {
        GPIO_ResetBits(_GROUP(pin), _PORT(pin));
    }
}
void AnalogInPort::OnOpen(void *param)
{
    Port::OnOpen(param);
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
    gpio->GPIO_Mode = GPIO_Mode_AN;
    //gpio->GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
}
void InputPort::OnOpen(void *param)
{
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
}
void GPIO_ISR(int num);
//中断线打开、关闭
void SetEXIT(int pinIndex, bool enable,InputPort::Trigger trigger=InputPort::Both)
{
    /* 配置EXTI中断线 */
    EXTI_InitTypeDef ext;
    EXTI_StructInit(&ext);
    ext.EXTI_Line = EXTI_Line0 << (pinIndex&0X0F);
    ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch(trigger)
	{
		case InputPort::Rising:
			ext.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
			break;
		case InputPort::Falling:
			ext.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
			break;
		case InputPort::Both:
		default:
			ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 上升沿下降沿触发
			break;
	}    
    ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
    EXTI_Init(&ext);
}

static const int PORT_IRQns[] = 
{
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
};
#include "TInterrupt.h"
void InputPort_OpenEXTI(Pin pin,InputPort::Trigger trigger=InputPort::Both)
{
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA+pin>>4, pin&0x0f);
	
	SetEXIT(pin, true,trigger);
	Interrupt.SetPriority(PORT_IRQns[pin&0x0f], 1u);
	//Interrupt.Activate(PORT_IRQns[v3],(void (__cdecl *)(unsigned __int16, void *))EXTI_IRQHandler,v1);
}
void OutputPort::Write(bool value)const
{
    if(this->_Pin == P0)
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
}
void Port::OnOpen(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
    gpio->GPIO_Speed = GPIO_Speed_100MHz;
}
bool Port::Read()const
{
    GPIO_TypeDef *group = _GROUP(this->_Pin);
    return (group->IDR >> (this->_Pin &0xF)) &1;
}
