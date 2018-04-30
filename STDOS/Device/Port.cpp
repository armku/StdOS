#include "Port.h"
#include "Platform\stm32.h"

Port::Port()
{
    this->_Pin = P0;
	this->Opened=false;
	this->Index=0;	
    this->State = NULL;
}

/*
��һ���ų�ʼ��
 */
Port &Port::Set(Pin pin)
{
    if(this->_Pin != pin)
	{
		if(this->_Pin!=P0)
			this->Close();
		this->_Pin=pin;	
				
	}
    return  *this;
}

bool Port::Empty()const
{
    return _Pin == P0;
}

void Port::Close()
{
	if(this->Opened)
	{
		if(this->_Pin!=P0)
		{
			this->Opened=false;
		}
	}
}

void Port::Clear()
{
	this->_Pin=P0;
}

OutputPort::OutputPort()
{
	this->Invert = 2;
	this->OpenDrain = 0;
	this->Speed = 50;
}
OutputPort::OutputPort(Pin pin):OutputPort(pin,2,false,50)
{}

OutputPort::OutputPort(Pin pin, uint8_t invert, bool openDrain, uint8_t speed)
{
    this->Opened = false;
    
	this->Invert = 2;
	this->OpenDrain = false;
	this->Speed = 50;
	this->OpenDrain = openDrain;
	this->Speed=speed;
	this->Invert = invert;
    if(pin !=P0)
	{
		this->Set(pin);
		this->Open();
	}
}

bool OutputPort::ReadInput()const
{
	if(this->Empty())
		return false;
	else
		return this->Invert ? !Port::Read(): Port::Read();
}

void OutputPort::Up(int ms)const
{
	if(!this->Empty())
	{
		this->Write(true);
		Sys.Sleep(ms);
		this->Write(false);
	}
}

void OutputPort::Down(int ms)const
{
	if(!this->Empty())
	{
		this->Write(false);
		Sys.Sleep(ms);
		this->Write(true);
	}
}

void OutputPort::Blink(int times, int ms)const
{
	if(!this->Empty())
	{
		bool flag = true;
		for (int i = 0; i < times; i++)
		{
			this->Write(flag);
			flag = !flag;
			Sys.Sleep(ms);
		}
		Write(false);
	}
}

void OutputPort::OnOpen(void *param)
{
    Port::OnOpen(param);
    this->OpenPin(param);
}

AlternatePort::AlternatePort(): OutputPort(P0,0,false,50)
{}

AlternatePort::AlternatePort(Pin pin):OutputPort(P0,0,false,50)
{
	if(pin !=P0)
	{
		this->Set(pin);
		this->Open();
	}
}

AlternatePort::AlternatePort(Pin pin, uint8_t invert, bool openDrain, uint8_t speed):OutputPort(P0,invert,openDrain,speed)
{
    if(pin !=P0)
	{
		this->Set(pin);
		this->Open();
	}
}

InputPort::InputPort(Pin pin, bool floating, PuPd pupd)
{
    this->Opened = false;
    this->Floating = floating;
    this->Pull = pupd;
	if(pin != P0)
	{
		Set(pin);
		this->Open();
	}
}

// ��ȡ�����������ţ������Ϊtrue�򷵻�true����ҪΪ��һ���ŷ���
bool InputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}
InputPort& InputPort::Init(Pin pin, bool invert)
{
	this->Set(pin);
	this->Invert=invert;
	return *this;
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

    if (pin == PA15 || pin == PB3 || pin == PB4)
    {
        if (pin == P0)
            pinindex = 48;
        else
            pinindex = pin &0xF;

        if (pin == P0)
            portname = 95;
        else
            portname = (pin >> 4) + 'A';
        debug_printf("Close JTAG Pin P%c%d \r\n", portname, pinindex);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << (pin >> 4), ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    }
}

void OpenPeriphClock(Pin pin)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << (pin >> 4), ENABLE);
}

bool Port::Open()
{
	#if defined STM32F0
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
	#elif defined STM32F1
    if (this->Opened == false)
    {
        if (_Pin != P0)
        {
            // ��ʱ��
            int gi = _Pin >> 4;
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
            // PA15/PB3/PB4 ��Ҫ�ر�JTAG
            switch (_Pin)
            {
                case PA15:
                case PB3:
                case PB4:
                    {
                        debug_printf("Close JTAG for P%c%d\r\n", _PIN_NAME(_Pin));

                        // PA15��jtag�ӿ��е�һԱ ��Ҫʹ�� ���뿪��remap
                        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
                        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
                        break;
                    }
            }

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
        // ��ʱ��
        int gi = _Pin >> 4;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);

        GPIO_InitTypeDef gpio;
        // �ر�Ҫ���أ���Щ�ṹ���Ա������Ϊû�г�ʼ������ɴ��
        GPIO_StructInit(&gpio);
        gpio.GPIO_Pin = 1 << (this->_Pin &0x0F);
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

        gpio->GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ	
        if (this->OpenDrain)
        {
            gpio->GPIO_OType = GPIO_OType_OD; //�������
            gpio->GPIO_PuPd = GPIO_PuPd_NOPULL; //            
        }
        else
        {
            gpio->GPIO_OType = GPIO_OType_PP; //�������
            gpio->GPIO_PuPd = GPIO_PuPd_UP; //����
        } 
	#endif
}
void AlternatePort::OpenPin(void *param)
{
	#if defined STM32F0
	#elif defined STM32F1
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
    gpio->GPIO_Mode = this->OpenDrain ? GPIO_Mode_AF_OD : GPIO_Mode_AF_PP;
    int i = 0;
    i++;
	#elif defined STM32F4
	#endif
}

void Port::RemapConfig(uint32_t param, bool sta){

}
///////////////////////////////����Ϊ���///////////////////////////////////////
// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

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

void InputPort::OnOpen(void *param)
{
    Port::OnOpen(param);
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
    if (Floating)
        gpio->GPIO_Mode = GPIO_Mode_IN_FLOATING;
    else if (Pull == UP)
        gpio->GPIO_Mode = GPIO_Mode_IPU;
    else if (Pull == DOWN)
        gpio->GPIO_Mode = GPIO_Mode_IPD;
    // ����ܲ�ȷ������Ҫ����ʵ�ʽ��е���     
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



































#if defined STM32F0
GPIO_TypeDef *IndexToGroup(uint8_t index);

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
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////����Ϊ���///////////////////////////////////////
// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

static const int PORT_IRQns[] = 
{
	EXTI0_1_IRQn, EXTI0_1_IRQn,  // ����
	EXTI2_3_IRQn, EXTI2_3_IRQn,  // ����
	EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn  // EXTI15_10
};
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

#elif defined STM32F1
#elif defined STM32F4
GPIO_TypeDef *IndexToGroup(uint8_t index);
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
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////����Ϊ���///////////////////////////////////////
// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (uint16_t)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

    #if 0
        static const int PORT_IRQns[] = 
        {
            EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5��������
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

static const int PORT_IRQns[] = 
{
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5��������
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

#endif
