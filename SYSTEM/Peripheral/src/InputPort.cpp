#include <stdio.h>
#include "InputPort.h"
#include "TInterrupt.h"

// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

void InputPort::OnConfig(GPIO_InitTypeDef &gpio)
{
    Port::OnConfig(gpio);

    #ifdef STM32F1
        if (Floating)
            gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        else if (Pull == UP)
            gpio.GPIO_Mode = GPIO_Mode_IPU;
        else if (Pull == DOWN)
            gpio.GPIO_Mode = GPIO_Mode_IPD;
        // ����ܲ�ȷ������Ҫ����ʵ�ʽ��е���
    #else 
        gpio.GPIO_Mode = GPIO_Mode_IN;
        gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}


/* �ж�״̬�ṹ�� */
/* һ��16���ж��ߣ���ζ��ͬһ����ÿһ��ֻ����һ������ʹ���ж� */
typedef struct TIntState
{
    Pin Pin;
    InputPort::IOReadHandler Handler; // ί���¼�
    void *Param; // �¼�������һ��������Ϊ�¼������ߵĶ���Ȼ�������̬�������ó�Ա����
    bool OldValue;

    uint ShakeTime; // ����ʱ��
    int Used; // ��ʹ�ô���������ǰ5���ж���˵�����ֻ����1�����ں�����ж�����˵�����ܶ��
} IntState;

// 16���ж���
static IntState State[16];
static bool hasInitState = false;

void RegisterInput(int groupIndex, int pinIndex, InputPort::IOReadHandler handler);
void UnRegisterInput(int pinIndex);

InputPort::~InputPort()
{
    // ȡ�������ж�
    if (_Registed)
    {
        Register(NULL);
    }
}

ushort InputPort::ReadGroup() // �����ȡ
{
    return GPIO_ReadInputData(Group);
}

// ��ȡ�����������ţ������Ϊtrue�򷵻�true����ҪΪ��һ���ŷ���
bool InputPort::Read()
{
    // תΪboolʱ��תΪ0/1
    bool rs = GPIO_ReadInputData(Group) &PinBit;
    return rs ^ Invert;
}

bool InputPort::Read(Pin pin)
{
    GPIO_TypeDef *group = _GROUP(pin);
    return (group->IDR >> (pin &0xF)) &1;
}


// ע��ص�  ���ж�ʹ��
void InputPort::Register(IOReadHandler handler, void *param)
{
    if (!PinBit)
        return ;

    // ��鲢��ʼ���ж�������
    if (!hasInitState)
    {
        for (int i = 0; i < 16; i++)
        {
            IntState *state = &State[i];
            state->Pin = P0;
            state->Handler = NULL;
            state->Used = 0;
        }
        hasInitState = true;
    }

    byte gi = _Pin >> 4;gi=gi;
    ushort n = PinBit;
    for (int i = 0; i < 16 && n != 0; i++)
    {
        // �����������һλ����ע���¼�
        if (n &0x01)
        {
            // ע���ж��¼�
            if (handler)
            {
                IntState *state = &State[i];
                state->ShakeTime = ShakeTime;                
                RegisterInput(gi, i, handler, param);
            }
            else
            {                
                UnRegisterInput(i);               
            }
        }
        n >>= 1;
    }

    _Registed = handler != NULL;
	this->RegisterAdd();
}
//���Ӳ��Թ���
void InputPort::RegisterAdd()
{
	printf("RegAdd\r\n");
}

void GPIO_ISR(int num) // 0 <= num <= 15
{
    if (!hasInitState)
        return ;

    IntState *state = State + num;
    if (!state)
        return ;

    uint bit = 1 << num;
    bool value;
    //byte line = EXTI_Line0 << num;
    // ���δָ��ί�У��򲻴���
    if (!state->Handler)
        return ;

    // Ĭ��20us����ʱ��
    uint shakeTime = state->ShakeTime;

    do
    {
        EXTI->PR = bit; // ���ù���λ
        value = InputPort::Read(state->Pin); // ��ȡ����״̬
        if (shakeTime > 0)
        {
            // ֵ�����б䶯�Ŵ���
            if (value == state->OldValue)
			{
                return ;            
            }
			Time.Sleep(shakeTime); // ���ⶶ��
        }
    }
    while (EXTI->PR &bit); // ����ٴι������ظ�
    //EXTI_ClearITPendingBit(line);
    // ֵ�����б䶯�Ŵ���
    if (shakeTime > 0 && value == state->OldValue)
        return ;
    state->OldValue = value;
    if (state->Handler)
    {
        #if 0
            // ��ֵvalueΪtrue��˵�����������ڶ���������down������ȡ��
            state->Handler(state->Pin, !value, state->Param);
        #endif 
    }
}

//�����ж��ߴ���
void EXTI_IRQHandler(ushort num, void *param)
{
    #if defined(STM32F1) || defined(STM32F4)
        // EXTI0 - EXTI4
        if (num <= EXTI4_IRQn)
            GPIO_ISR(num - EXTI0_IRQn);
        else if (num == EXTI9_5_IRQn)
        {
            // EXTI5 - EXTI9
            uint pending = EXTI->PR &EXTI->IMR &0x03E0; // pending bits 5..9
            int num = 5;
            pending >>= 5;
            do
            {
                if (pending &1)
                    GPIO_ISR(num);
                num++;
                pending >>= 1;
            }
            while (pending);
        }
        else if (num == EXTI15_10_IRQn)
        {
            // EXTI10 - EXTI15
            uint pending = EXTI->PR &EXTI->IMR &0xFC00; // pending bits 10..15
            int num = 10;
            pending >>= 10;
            do
            {
                if (pending &1)
                    GPIO_ISR(num);
                num++;
                pending >>= 1;
            }
            while (pending);
        }
    #elif defined(STM32F0)
        switch (num)
        {
            case EXTI0_1_IRQn:
                {
                    uint pending = EXTI->PR &EXTI->IMR &0x0003; // pending bits 0..1
                    int num = 0;
                    pending >>= 0;
                    do
                    {
                        if (pending &1)
                            GPIO_ISR(num);
                        num++;
                        pending >>= 1;
                    }
                    while (pending);
                    break;
                }
            case EXTI2_3_IRQn:
                {
                    uint pending = EXTI->PR &EXTI->IMR &0x000c; // pending bits 3..2
                    int num = 2;
                    pending >>= 2;
                    do
                    {
                        if (pending &1)
                            GPIO_ISR(num);
                        num++;
                        pending >>= 1;
                    }
                    while (pending);
                }
            case EXTI4_15_IRQn:
                {
                    uint pending = EXTI->PR &EXTI->IMR &0xFFF0; // pending bits 4..15
                    int num = 4;
                    pending >>= 4;
                    do
                    {
                        if (pending &1)
                            GPIO_ISR(num);
                        num++;
                        pending >>= 1;
                    }
                    while (pending);
                }
        }
    #endif 
}

void SetEXIT(int pinIndex, bool enable)
{
    /* ����EXTI�ж��� */
    EXTI_InitTypeDef ext;
    EXTI_StructInit(&ext);
    ext.EXTI_Line = EXTI_Line0 << pinIndex;
    ext.EXTI_Mode = EXTI_Mode_Interrupt;
    ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �������½��ش���
    ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
    EXTI_Init(&ext);
}
#if defined(STM32F1) || defined(STM32F4)
    static const int PORT_IRQns[] = 
    {
        EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5��������
        EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
        EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
    };
	#endif
// ���������ж��й�
void InputPort::RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param)
{
    IntState *state = &State[pinIndex];
    Pin pin = (Pin)((groupIndex << 4) + pinIndex);
    // ����Ƿ��Ѿ�ע�ᵽ���������
    if (state->Pin != pin && state->Pin != P0)
    {
        return ;
    }
    state->Pin = pin;
    state->Handler = handler;
    state->Param = param;
    state->OldValue = Read(pin); // Ԥ�ȱ��浱ǰ״ֵ̬����������ʱ�����ж�
	
    // ��ʱ�ӣ�ѡ��˿���Ϊ�˿�EXTIʱ����
    #if defined(STM32F0) || defined(STM32F4)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
        SYSCFG_EXTILineConfig(groupIndex, pinIndex);
    #elif defined(STM32F1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_EXTILineConfig(groupIndex, pinIndex);
    #endif 
	
    SetEXIT(pinIndex, true);    
        // �򿪲�����EXTI�ж�Ϊ�����ȼ�
    Interrupt.SetPriority(PORT_IRQns[pinIndex], 1);   
    state->Used++;
    if (state->Used == 1)
    {        
        Interrupt.Activate(PORT_IRQns[pinIndex], EXTI_IRQHandler, this);        
    }
}

void InputPort::UnRegisterInput(int pinIndex)
{
    IntState *state = &State[pinIndex];
    // ȡ��ע��
    state->Pin = P0;
    state->Handler = 0;

    SetEXIT(pinIndex, false);

    state->Used--;
    if (state->Used == 0)
    {        
        Interrupt.Deactivate(PORT_IRQns[pinIndex]);        
    }
}
/////////////////////////////////////////////////////////////////
//����Ϊ�ɰ�
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
typedef struct TIntStateOld
{
    Pin Pin;
    BtnInCallbackOld Handler;
    bool OldValue;
} IntStateOld;

// 16���ж���
static IntStateOld StateOld[16];

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
void InputPortOld::RegisterOld(BtnInCallbackOld handler)
{
    byte pins = this->_Pin &0x0F;
    IntStateOld *state = &StateOld[pins];
    // ע���ж��¼�
    if (handler)
    {
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
    void GPIO_ISROld(int num) // 0 <= num <= 15
    {
        IntStateOld *state = &StateOld[num];
		
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
			GPIO_ISROld(0);
			EXTI_IRQHandler(0,0);
			printf("�ж���0\r\n");
        }
    }

    void EXTI1_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line1); //����жϱ�־λ
			GPIO_ISROld(1);
			printf("�ж���1\r\n");
        } 
    }

    void EXTI2_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line2); //����жϱ�־λ           
			GPIO_ISROld(2);
			printf("�ж���2\r\n");
        }
    }

    void EXTI3_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line3); //����жϱ�־λ
			GPIO_ISROld(3);
			printf("�ж���3\r\n");
        }
    }

    void EXTI4_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line4); //����жϱ�־λ
			GPIO_ISROld(4);
			printf("�ж���4\r\n");
        }
    }

    void EXTI9_5_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line5); //����жϱ�־λ
			GPIO_ISROld(5);
        }
        if (EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line6); //����жϱ�־λ		
        	GPIO_ISROld(6);
        }
        if (EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line7); //����жϱ�־λ		
        	GPIO_ISROld(7);
        }
        if (EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line8); //����жϱ�־λ		
        	GPIO_ISROld(8);
        }
        if (EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line9); //����жϱ�־λ		
        	GPIO_ISROld(9);
        }
		printf("�ж���9_5\r\n");
    }
    /// IO ���жϣ��жϿ�ΪPC13
    void EXTI15_10_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line10); //����жϱ�־λ		
        	GPIO_ISROld(10);
        }
        if (EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line11); //����жϱ�־λ		
        	GPIO_ISROld(11);
        }
        if (EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line12); //����жϱ�־λ		
        	GPIO_ISROld(12);
        }
        if (EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line13); //����жϱ�־λ		
        	GPIO_ISROld(13);
        }
        if (EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line14); //����жϱ�־λ		
        	GPIO_ISROld(14);
        }
        if (EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line15); //����жϱ�־λ		
        	GPIO_ISROld(15);
        }
		printf("�ж���5_10\r\n");
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

