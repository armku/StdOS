#include "Port.h"
#include "Platform\stm32.h"

GPIO_TypeDef *IndexToGroup(byte index);

bool Port::Open()
{
    if (this->Opened == false)
    {
        if (_Pin != P0)
        {
            // ��ʱ��
            int gi = _Pin >> 4;
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA << gi, ENABLE);
            
            GPIO_InitTypeDef gpio;
            // �ر�Ҫ���أ���Щ�ṹ���Ա������Ϊû�г�ʼ������ɴ��
            GPIO_StructInit(&gpio);
            gpio.GPIO_Pin = 1 << (this->_Pin &0x0F);
            this->OnOpen(&gpio);

            GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
        }

        this->Opened = true;
    }
    return true;
}

void OutputPort::OpenPin(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;

    	gpio->GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ	
        if (this->OpenDrain)
        {
            gpio->GPIO_OType = GPIO_OType_OD;
			gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;/*��������ģʽΪ������*/
        }
        else
        {
            gpio->GPIO_OType = GPIO_OType_PP;//ͨ���������			
			gpio->GPIO_PuPd = GPIO_PuPd_UP;/*��������ģʽΪ����*/
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
void Port::RemapConfig(uint param, bool sta)
{
	
}
void InputPort::OpenPin(void* param)
{
}
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////����Ϊ���///////////////////////////////////////
// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

static const int PORT_IRQns[] = 
{
	EXTI0_1_IRQn, EXTI0_1_IRQn,  // ����
	EXTI2_3_IRQn, EXTI2_3_IRQn,  // ����
	EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn  // EXTI15_10
};
GPIO_TypeDef *IndexToGroup(byte index)
{
    return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
}

byte GroupToIndex(GPIO_TypeDef *group)
{
    return (byte)(((int)group - GPIOA_BASE) >> 10);
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

//�ж��ߴ򿪡��ر�
void SetEXIT(int pinIndex, bool enable,InputPort::Trigger trigger=InputPort::Both)
{
    /* ����EXTI�ж��� */
    EXTI_InitTypeDef ext;
    EXTI_StructInit(&ext);
    ext.EXTI_Line = EXTI_Line0 << (pinIndex&0X0F);
    ext.EXTI_Mode = EXTI_Mode_Interrupt;
	switch(trigger)
	{
		case InputPort::Rising:
			ext.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
			break;
		case InputPort::Falling:
			ext.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
			break;
		case InputPort::Both:
		default:
			ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �������½��ش���
			break;
	}    
    ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
    EXTI_Init(&ext);
}

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
	gpio->GPIO_Speed = GPIO_Speed_50MHz;
}
bool Port::Read()const
{
    GPIO_TypeDef *group = _GROUP(this->_Pin);
    return (group->IDR >> (this->_Pin &0xF)) &1;
}
