#include <stdio.h>
#include "stm32f10x.h"
#include "Port.h"
#include "TInterrupt.h"

#define GPIO_Mode_IN GPIO_Mode_IN_FLOATING
#define GPIO_Mode_AF GPIO_Mode_AF_OD
#define GPIO_OType_OD GPIO_Mode_Out_OD
#define GPIO_OType_PP GPIO_Mode_Out_PP
#define GPIO_Mode_OUT GPIO_Mode_Out_OD


// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))


#if defined(STM32F1) || defined(STM32F4)
    static const int PORT_IRQns[] = 
    {
        EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5��������
        EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
        EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
    };
#elif defined(STM32F0)
    static const int PORT_IRQns[] = 
    {
        EXTI0_1_IRQn, EXTI0_1_IRQn,  // ����
        EXTI2_3_IRQn, EXTI2_3_IRQn,  // ����
        EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn,
            EXTI4_15_IRQn  // EXTI15_10
    };
#endif 
GPIO_TypeDef *IndexToGroup(byte index)
{
    return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
}

byte GroupToIndex(GPIO_TypeDef *group)
{
    return (byte)(((int)group - GPIOA_BASE) >> 10);
}

/*
�˿ڻ�������
 */
Port::Port()
{
    _Pin = P0;
    Group = NULL;
    PinBit = 0;
}

#ifndef TINY	
    Port::~Port()
    {
        #if defined(STM32F1)
            // �ָ�Ϊ��ʼ��״̬
            ushort bits = PinBit;
            int config = InitState &0xFFFFFFFF;
            for (int i = 0; i < 16 && bits; i++, bits >>= 1)
            {
                if (i == 7)
                    config = InitState >> 32;
                if (bits &1)
                {
                    uint shift = (i &7) << 2; // ÿ����4λ
                    uint mask = 0xF << shift; // ���ε�����λ

                    GPIO_TypeDef *port = Group;
                    if (i &0x08)
                    {
                        // bit 8 - 15
                        port->CRH = port->CRH &~mask | (config &mask);
                    }
                    else
                    {
                        // bit 0-7
                        port->CRL = port->CRL &~mask | (config &mask);
                    }
                }
            }
        #endif 
    }
#endif 

/*
��һ���ų�ʼ��
 */
Port &Port::Set(Pin pin)
{
    //assert_param(pin != P0);

    _Pin = pin;
    if (_Pin != P0)
    {
        Group = IndexToGroup(pin >> 4);
        PinBit = 1 << (pin &0x0F);
    }
    else
    {
        Group = NULL;
        PinBit = 0;
    }

    #if defined(STM32F1)
        // �������ŵĳ�ʼ״̬������ʱ��ѡ��ָ�
        if (_Pin != P0)
            InitState = ((UInt64)Group->CRH << 32) + Group->CRL;
    #endif 

    if (_Pin != P0)
    {
        GPIO_InitTypeDef gpio;
        // �ر�Ҫ���أ���Щ�ṹ���Ա������Ϊû�г�ʼ������ɴ��
        GPIO_StructInit(&gpio);

        // ��ʱ��
        int gi = _Pin >> 4;
        #ifdef STM32F0
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN << gi, ENABLE);
        #elif defined(STM32F1)
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
        #elif defined(STM32F4)
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);
        #endif 

        gpio.GPIO_Pin = PinBit;

        #ifdef STM32F1
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
        #endif 
        GPIO_Init(Group, &gpio);
    }

    return  *this;
}

bool Port::Empty()const
{
    return _Pin == P0;
}

void Port::OnOpen(void *param){

}
void Port::OnClose(){}

#define GPIO_Mode_IN GPIO_Mode_IN_FLOATING
#define GPIO_Mode_AF GPIO_Mode_AF_OD
#define GPIO_OType_OD GPIO_Mode_Out_OD
#define GPIO_OType_PP GPIO_Mode_Out_PP
#define GPIO_Mode_OUT GPIO_Mode_Out_OD

OutputPort::OutputPort()
{
    Init();
}

OutputPort::OutputPort(Pin pin)
{
    Init();
    Set(pin);
}

OutputPort::OutputPort(Pin pin, byte invert, bool openDrain, byte speed)
{
    Init(invert, openDrain, speed);
    Set(pin);
}

void OutputPort::OnConfig(GPIO_InitTypeDef &gpio)
{
    #ifndef STM32F4
        assert_param(Speed == 2 || Speed == 10 || Speed == 50);
    #else 
        assert_param(Speed == 2 || Speed == 25 || Speed == 50 || Speed == 100);
    #endif 

//    Port::OnConfig(gpio);

    switch (Speed)
    {
        case 2:
            gpio.GPIO_Speed = GPIO_Speed_2MHz;
            break;
            #ifndef STM32F4
            case 10:
                gpio.GPIO_Speed = GPIO_Speed_10MHz;
                break;
            #else 
            case 25:
                gpio.GPIO_Speed = GPIO_Speed_25MHz;
                break;
            case 100:
                gpio.GPIO_Speed = GPIO_Speed_100MHz;
                break;
            #endif 
        case 50:
            gpio.GPIO_Speed = GPIO_Speed_50MHz;
            break;
    }

    #ifdef STM32F1
        gpio.GPIO_Mode = OpenDrain ? GPIO_Mode_Out_OD : GPIO_Mode_Out_PP;
    #else 
        gpio.GPIO_Mode = GPIO_Mode_OUT;
        gpio.GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 

    // ����֮ǰ����Ҫ���ݵ���������趨��ʼ״̬��Ҳ�����ڴ򿪶˿�֮ǰ������ȷ�˿ڸߵ�״̬
    ushort dat = GPIO_ReadOutputData(Group);
    if (!Invert)
        dat &= ~PinBit;
    else
        dat |= PinBit;
    GPIO_Write(Group, dat);
}

/*
�����ȡ
 */
ushort OutputPort::ReadGroup()
{
    return GPIO_ReadOutputData(Group);
}

bool OutputPort::Read()const
{
    // תΪboolʱ��תΪ0/1
    bool rs = GPIO_ReadOutputData(Group) &PinBit;
    return rs ^ Invert;
}

bool OutputPort::ReadInput()const
{
    bool rs = GPIO_ReadInputData(Group) &PinBit;
    return rs ^ Invert;
}

bool OutputPort::Read(Pin pin)
{
    GPIO_TypeDef *group = _GROUP(pin);
    return (group->IDR >> (pin &0xF)) &1;
}

void OutputPort::Write(bool value)const
{
    if (value ^ Invert)
        GPIO_SetBits(Group, PinBit);
    else
        GPIO_ResetBits(Group, PinBit);
}

OutputPort::operator bool()
{
    return Read();
}

void OutputPort::WriteGroup(ushort value)
{
    GPIO_Write(Group, value);
}

void OutputPort::Up(int ms)const
{
    Write(true);
    Sys.Sleep(ms);
    Write(false);
}

void OutputPort::Down(int ms)const
{
    Write(false);
    Sys.Sleep(ms);
    Write(true);
}

void OutputPort::Blink(int times, int ms)const
{
    bool flag = true;
    for (int i = 0; i < times; i++)
    {
        Write(flag);
        flag = !flag;
        Sys.Sleep(ms);
    }
    Write(false);
}

void OutputPort::OnOpen(void *param){}
void OutputPort::OpenPin(void *param){}
/*
���ö˿�״̬
 */
void OutputPort::Write(Pin pin, bool value)
{
    if (value)
        GPIO_SetBits(_GROUP(pin), _PORT(pin));
    else
        GPIO_ResetBits(_GROUP(pin), _PORT(pin));
}

void OutputPort::Init(bool invert, bool openDrain, uint speed)
{
    OpenDrain = openDrain;
    Speed = speed;
    Invert = invert;
}

AlternatePort::AlternatePort(): OutputPort()
{
    Init(false, false);
}

AlternatePort::AlternatePort(Pin pin): OutputPort(pin){

}

AlternatePort::AlternatePort(Pin pin, byte invert, bool openDrain, byte speed)
{
    Init(invert, openDrain, speed);
    Set(pin);
}

void AlternatePort::OpenPin(void *param){

}

void AlternatePort::OnConfig(GPIO_InitTypeDef &gpio)
{
    OutputPort::OnConfig(gpio);

    #ifdef STM32F1
        gpio.GPIO_Mode = OpenDrain ? GPIO_Mode_AF_OD : GPIO_Mode_AF_PP;
    #else 
        gpio.GPIO_Mode = GPIO_Mode_AF;
        gpio.GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}

void AnalogInPort::OnConfig(GPIO_InitTypeDef &gpio)
{
//    Port::OnConfig(gpio);

    #ifdef STM32F1
        gpio.GPIO_Mode = GPIO_Mode_AIN; //
    #else 
        gpio.GPIO_Mode = GPIO_Mode_AN;
        //gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}


InputPort::InputPort(Pin pin, bool floating, PuPd pupd)
{
    Init(floating, pupd);
    Set(pin);
}

InputPort::InputPort(){

}

InputPort::operator bool()
{
    return Read();
}

void InputPort::OnOpen(void *param){

}
void InputPort::OnClose(){

}
void InputPort::Init(bool floating, PuPd pupd)
{
    Pull = pupd;
    Floating = floating;

    _Registed = false;
    //ShakeTime = 20;
    // ��ЩӦ�õ��������Ҫ���ߵ������ȣ����ʱ����Ҫ�������
    ShakeTime = 0;
    Invert = false;
}

void InputPort::OnConfig(GPIO_InitTypeDef &gpio)
{
//    Port::OnConfig(gpio);

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
    InputPort inputport;
    InputPort::IOReadHandler Handler; // ί���¼�
    void *Param; // �¼�������һ��������Ϊ�¼������ߵĶ���Ȼ�������̬�������ó�Ա����	
    bool OldValue;

    uint ShakeTime; // ����ʱ��	
    int Used; // ��ʹ�ô���������ǰ5���ж���˵�����ֻ����1�����ں�����ж�����˵�����ܶ��
} IntState;

// 16���ж���
static IntState State[16];
static bool hasInitState = false;

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
bool InputPort::Read()const
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

    byte gi = _Pin >> 4;
    gi = gi;
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
}

void GPIO_ISR(int num) // 0 <= num <= 15
{
    if (!hasInitState)
    {
        return ;
    }
    IntState *state = State + num;
    if (!state)
    {
        return ;
    }
    #if 0
        uint bit = 1 << num;
    #endif 
    bool value;
    value = InputPort::Read(state->Pin);
    //byte line = EXTI_Line0 << num;
    // ���δָ��ί�У��򲻴���
    if (!state->Handler)
    {
        return ;
    }
    #if 0
        // Ĭ��20us����ʱ��
        uint shakeTime = state->ShakeTime;
    #endif 
    #if 0
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
    #endif 
    //EXTI_ClearITPendingBit(line);
    #if 0
        // ֵ�����б䶯�Ŵ���
        if (shakeTime > 0 && value == state->OldValue)
            return ;
        state->OldValue = value;
    #endif 
    if (state->Handler)
    {
        // ��ֵvalueΪtrue��˵�����������ڶ���������down������ȡ��
        state->Handler(&(state->inputport), value, state->Param);
    }
}

//�����ж��ߴ���
void EXTI_IRQHandler(ushort num, void *param)
{
    #if defined(STM32F1) || defined(STM32F4)
        // EXTI0 - EXTI4
        if (num <= EXTI4_IRQn)
        {
            GPIO_ISR(num - EXTI0_IRQn);
        }
        else if (num == EXTI9_5_IRQn)
        {
            // EXTI5 - EXTI9
            uint pending = EXTI->PR &EXTI->IMR &0x03E0; // pending bits 5..9
            int num = 5;
            pending >>= 5;
            do
            {
                if (pending &1)
                {
                    GPIO_ISR(num);
                }
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
                {
                    GPIO_ISR(num);
                }
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

//�ж��ߴ򿪡��ر�
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
