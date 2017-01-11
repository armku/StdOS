#include "stm32f10x_exti.h"
#include "InputPortOld.h"

// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

InputPortOld::InputPortOld()
{
	this->_Pin=P0;
}
InputPortOld::InputPortOld(Pin pin)
{
	this->_Pin = pin;
	this->SetModeOut_PP();
	
	this->SetModeINPUT_IPD();

    this->eXTI_Line = EXTI_Line0 << (pin &0X0F);
    if ((pin &0X0F) < 5)
    {
        this->nVIC_IRQChannel = EXTI0_IRQn + (pin &0X0F);
    }
    else if ((pin &0X0F) < 11)
    {
        this->nVIC_IRQChannel = EXTI9_5_IRQn;
    }
    else
    {
        this->nVIC_IRQChannel = EXTI15_10_IRQn;
    }
    this->gPIO_PinSource = GPIO_PinSource0 + pin &0X0F;
    this->gPIO_PortSourceGPIO = GPIO_PortSourceGPIOA + pin / 16;
}
void InputPortOld::InitOld()
{	
	EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(this->gPIO_PortSourceGPIO, this->gPIO_PinSource);
    //PA0 ���ж��ߵ�ӳ�� 

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����
    //3���жϷ�ʽ��ѡһ��
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�������ж�
    EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Rising_Falling;//�����غ��½��ض��ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = this->nVIC_IRQChannel; //�ж�����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�0 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�0 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);
    //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

/* �ж�״̬�ṹ�� */
/* һ��16���ж��ߣ���ζ��ͬһ����ÿһ��ֻ����һ������ʹ���ж� */
typedef struct TIntState
{
    Pin Pin;
    BtnInCallback Handler;
    bool OldValue;
} IntState;

// 16���ж���
static IntState State[16];

void InputPortOld::Exti0_state(bool onoff)
//�ⲿ�ж�1��ʼ��  PA0 ��ʹ����ʹ��  ONΪʹ��   OFFΪʹ��
{
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�½����ж�
    if (onoff)
    {
        EXTI_InitStructure.EXTI_LineCmd = ENABLE; //�ж�ʹ��	
    }
    else
    {
        EXTI_InitStructure.EXTI_LineCmd = DISABLE; //�ж�ʹ��
    }
    EXTI_Init(&EXTI_InitStructure);
}

void InputPortOld::On()
{
    this->Exti0_state(true);
}

void InputPortOld::Off()
{
    this->Exti0_state(false);
}

// ע��ص�  ���ж�ʹ��
void InputPortOld::RegisterOld(BtnInCallback handler)
{
    byte pins = this->_Pin &0x0F;
    IntState *state = &State[pins];
    // ע���ж��¼�
    if (handler)
    {
        // ����Ƿ��Ѿ�ע�ᵽ���������
//        if (state->Pin != pin && state->Pin != P0)
//        {
//            #if DEBUG
//                debug_printf("EXTI%d can't register to P%c%d, it has register to P%c%d\r\n", pins, _PIN_NAME(pin), _PIN_NAME(state->Pin));
//            #endif 
//            return ;
//        }
        state->Pin = this->_Pin;
        state->Handler = handler;
    }
    else
    {
        // ȡ��ע��
        state->Pin = P0;
        state->Handler = 0;
    }	
}

//д��ֵ��true���ߵ�ƽ��false���͵�ƽ
void InputPortOld::Write(const bool value)
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

////����ģʽ
void InputPortOld::SetMode(PIN_MODE mode)
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
    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = _PORT(this->_Pin);

    GPIO_Init(_GROUP(this->_Pin), &GPIO_InitStructure);
}

void InputPortOld::SetModeAIN()
{
    this->SetMode(AIN);
}

void InputPortOld::SetModeIN_FLOATING()
{
    this->SetMode(INPUT);
}

void InputPortOld::SetModeINPUT_IPD()
{
    this->SetMode(INPUT_PD);
}

void InputPortOld::SetModeINPUT_IPU()
{
    this->SetMode(INPUT_PU);
}

void InputPortOld::SetModeOut_OD()
{
    this->SetMode(OUTPUT_OD);
}

void InputPortOld::SetModeOut_PP()
{
    this->SetMode(OUTPUT_PP);
}

void InputPortOld::SetModeAF_OD()
{
    this->SetMode(AF_OD);
}

void InputPortOld::SetModeAF_PP()
{
    this->SetMode(AF_PP);
}

void InputPortOld::Set()
{    
    GPIO_SetBits(_GROUP(_Pin), _PORT(_Pin));
}

void InputPortOld::Reset()
{    
    GPIO_ResetBits(_GROUP(_Pin), _PORT(_Pin));
}

byte InputPortOld::Read(void)
{
    return ReadPinPort(this->_Pin);
}
//��ȡ�˿�״̬
bool InputPortOld::ReadPinPort(Pin pin)
{
	 return GPIO_ReadInputDataBit(_GROUP(pin), _PORT(pin));
}

extern "C"
{   
    void GPIO_ISR(int num) // 0 <= num <= 15
    {
        IntState *state = &State[num];
		
		// ���δָ��ί�У��򲻴���
        if (!state->Handler)
		{
            return ;
		}
				
        bool value;
                
        value = InputPortOld::ReadPinPort(state->Pin); // ��ȡ����״̬
        
        if (state->Handler)
        {
            state->Handler(state->Pin, value);
        }
    }

    void EXTI0_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line0) != RESET)
        {

            EXTI_ClearITPendingBit(EXTI_Line0); //����жϱ�־λ	
			GPIO_ISR(0);
        }
    }

    void EXTI1_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line1); //����жϱ�־λ
			GPIO_ISR(1);
        } 
    }

    void EXTI2_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line2); //����жϱ�־λ           
			GPIO_ISR(2);
        }
    }

    void EXTI3_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line3); //����жϱ�־λ
			GPIO_ISR(3);
        }
    }

    void EXTI4_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line4); //����жϱ�־λ
			GPIO_ISR(4);
        }
    }

    void EXTI9_5_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line5); //����жϱ�־λ
			GPIO_ISR(5);
        }
        if (EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line6); //����жϱ�־λ		
        	GPIO_ISR(6);
        }
        if (EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line7); //����жϱ�־λ		
        	GPIO_ISR(7);
        }
        if (EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line8); //����жϱ�־λ		
        	GPIO_ISR(8);
        }
        if (EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line9); //����жϱ�־λ		
        	GPIO_ISR(9);
        }
    }
    /// IO ���жϣ��жϿ�ΪPC13
    void EXTI15_10_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line10); //����жϱ�־λ		
        	GPIO_ISR(10);
        }
        if (EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line11); //����жϱ�־λ		
        	GPIO_ISR(11);
        }
        if (EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line12); //����жϱ�־λ		
        	GPIO_ISR(12);
        }
        if (EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line13); //����жϱ�־λ		
        	GPIO_ISR(13);
        }
        if (EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line14); //����жϱ�־λ		
        	GPIO_ISR(14);
        }
        if (EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line15); //����жϱ�־λ		
        	GPIO_ISR(15);
        }
    }
}

/*
����ͨ��
PA0 ��
PA1
PA2 ?
PA3 ?
PA4
PA5
PA6
PA7
PA8 ��
PA9 ��
PA10 ?
PA11 ��
PA12 ��
PA13
PA14
PA15

PB0 ��
PB1 ��
PB2 ��
PB3 ��
PB4 ��
PB5 ��
PB6 ��
PB7 ��
PB8 ��
PB9 ?
PB10 ?
PB11 ��
PB12 ��
PB13 ��
PB14 ��
PB15 ��

PC0 ��
PC1 ��
PC2 ��
PC3 ��
PC4 ��
PC5 ��
PC6 ��
PC7 ��
PC8 ��
PC9 ��
PC10 ?
PC11 ��
PC12 ��
PC13 ��
PC14
PC15

PD0
PD1
PD2 ��
PD3
PD4
PD5
PD6
PD7
PD8
PD9
PD10
PD11
PD12
PD13
PD14
PD15

PE0
PE1
PE2
PE3
PE4
PE5
PE6 ��
PE7
PE8
PE9
PE10
PE11
PE12
PE13
PE14
PE15

PF0
PF1
PF2
PF3
PF4
PF5
PF6
PF7
PF8
PF9
PF10
PF11
PF12
PF13
PF14
PF15

PG0
PG1
PG2
PG3
PG4
PG5
PG6
PG7
PG8 ��
PG9 ��
PG10
PG11
PG12
PG13
PG14
PG15 ��
 */
