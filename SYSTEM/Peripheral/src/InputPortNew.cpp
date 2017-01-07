#include <stdio.h>
#include "InputPortNew.h"

// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

void InputPortNew::OnConfig(GPIO_InitTypeDef &gpio)
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
        //gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}


/* �ж�״̬�ṹ�� */
/* һ��16���ж��ߣ���ζ��ͬһ����ÿһ��ֻ����һ������ʹ���ж� */
typedef struct TIntState
{
    Pin Pin;
    InputPortNew::IOReadHandler Handler; // ί���¼�
    void *Param; // �¼�������һ��������Ϊ�¼������ߵĶ���Ȼ�������̬�������ó�Ա����
    bool OldValue;

    uint ShakeTime; // ����ʱ��
    int Used; // ��ʹ�ô���������ǰ5���ж���˵�����ֻ����1�����ں�����ж�����˵�����ܶ��
} IntState;

// 16���ж���
static IntState State[16];
static bool hasInitState = false;

void RegisterInput(int groupIndex, int pinIndex, InputPortNew::IOReadHandler handler);
void UnRegisterInput(int pinIndex);

InputPortNew::~InputPortNew()
{
    // ȡ�������ж�
    if (_Registed)
        Register(NULL);
}

ushort InputPortNew::ReadGroup() // �����ȡ
{
    return GPIO_ReadInputData(Group);
}

// ��ȡ�����������ţ������Ϊtrue�򷵻�true����ҪΪ��һ���ŷ���
bool InputPortNew::Read()
{
    // תΪboolʱ��תΪ0/1
    bool rs = GPIO_ReadInputData(Group) &PinBit;
    return rs ^ Invert;
}

bool InputPortNew::Read(Pin pin)
{
    GPIO_TypeDef *group = _GROUP(pin);
    return (group->IDR >> (pin &0xF)) &1;
}


// ע��ص�  ���ж�ʹ��
void InputPortNew::Register(IOReadHandler handler, void *param)
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
                #if 0
                    RegisterInput(gi, i, handler, param);
                #endif 
            }
            else
            {
                #if 0
                    UnRegisterInput(i);
                #endif 
            }
        }
        n >>= 1;
    }

    _Registed = handler != NULL;
}

#define IT 1
#ifdef IT
    #if 0
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
                value = InputPortNew::Read(state->Pin); // ��ȡ����״̬
                if (shakeTime > 0)
                {
                    // ֵ�����б䶯�Ŵ���
                    if (value == state->OldValue)
                        return ;

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
                // ��ֵvalueΪtrue��˵�����������ڶ���������down������ȡ��
                state->Handler(state->Pin, !value, state->Param);
            }
        }
    #endif 
    #if 0
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
    #endif 
#endif 

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

#if 0
    // ���������ж��й�
    void InputPortNew::RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param)
    {
        IntState *state = &State[pinIndex];
        Pin pin = (Pin)((groupIndex << 4) + pinIndex);
        // ����Ƿ��Ѿ�ע�ᵽ���������
        if (state->Pin != pin && state->Pin != P0)
        {
            #if DEBUG
                debug_printf("EXTI%d can't register to P%c%d, it has register to P%c%d\r\n", groupIndex, _PIN_NAME(pin), _PIN_NAME(state->Pin));
            #endif 
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
#endif 
#if 0
    void InputPortNew::UnRegisterInput(int pinIndex)
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
#endif
// �˿����ű���
#if DEBUG    
    bool InputPortNew::OnReserve(Pin pin, bool flag)
    {
        debug_printf("Input::");

        return Port::OnReserve(pin, flag);
    }
#endif
