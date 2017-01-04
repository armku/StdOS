#include "stm32f10x.h"
#include "Port.h"

#define NULL 0
#define GPIO_Mode_IN GPIO_Mode_IN_FLOATING
#define GPIO_Mode_AF GPIO_Mode_AF_OD
#define GPIO_OType_OD GPIO_Mode_Out_OD
#define GPIO_OType_PP GPIO_Mode_Out_PP
#define GPIO_Mode_OUT GPIO_Mode_Out_OD


// ��ȡ������
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

#if 0
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
#endif 

// �˿ڻ�������
#define REGION_Port 1
#ifdef REGION_Port
    Port::Port()
    {
        _Pin = P0;
        Group = NULL;
        PinBit = 0;
    }

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

        #if DEBUG
            // �����������
            OnReserve(_Pin, false);
        #endif 
    }

    // ��һ���ų�ʼ��
    Port &Port::Set(Pin pin)
    {
        //assert_param(pin != P0);

        #if DEBUG
            if (_Pin != P0)
                OnReserve(_Pin, false);
        #endif 

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
                InitState = ((ulong)Group->CRH << 32) + Group->CRL;
        #endif 

        #if DEBUG
            // ��������
            if (_Pin != P0)
                OnReserve(_Pin, true);
        #endif 

        if (_Pin != P0)
            Config();

        return  *this;
    }

    void Port::Config()
    {
        GPIO_InitTypeDef gpio;
        // �ر�Ҫ���أ���Щ�ṹ���Ա������Ϊû�г�ʼ������ɴ��
        GPIO_StructInit(&gpio);

        OnConfig(gpio);
        GPIO_Init(Group, &gpio);
    }

    void Port::OnConfig(GPIO_InitTypeDef &gpio)
    {
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
    }

    GPIO_TypeDef *Port::IndexToGroup(byte index)
    {
        return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
    }
    byte Port::GroupToIndex(GPIO_TypeDef *group)
    {
        return (byte)(((int)group - GPIOA_BASE) >> 10);
    }
#endif 

// �˿����ű���
#if DEBUG
    static ushort Reserved[8]; // ���ű���λ����¼ÿ�������Ƿ��Ѿ�����������ֹ���ģ��ʹ��

    // �������ţ���Ĺ���Ҫʹ��ʱ���ᱨ�������Ƿ񱣻��ɹ�
    bool Port::Reserve(Pin pin, bool flag)
    {
        int port = pin >> 4, bit = 1 << (pin &0x0F);
        if (flag)
        {
            if (Reserved[port] &bit)
            {
                // ��������Ѿ�����������ʾ���ܶ�ط�����ReservePin����д��־���õ�False��ֱ�����쳣
                debug_printf("ReservePin P%c%d already reserved\r\n", _PIN_NAME(pin));
                return false; // already reserved
            }
            Reserved[port] |= bit;

            debug_printf("ReservePin P%c%d\r\n", _PIN_NAME(pin));
        }
        else
        {
            Reserved[port] &= ~bit;

            #if defined(STM32F1)
                int config = 0;
                uint shift = (pin &7) << 2; // 4 bits / pin
                uint mask = 0xF << shift; // ���ε�����λ
                GPIO_TypeDef *port2 = IndexToGroup(port); // pointer to the actual port registers
                if (pin &0x08)
                {
                    // bit 8 - 15
                    config = port2->CRH &mask;
                }
                else
                {
                    // bit 0-7
                    config = port2->CRL &mask;
                }

                config >>= shift; // ��λ�����ұ�
                config &= 0xF;
                debug_printf("UnReservePin P%c%d Config=0x%02x\r\n", _PIN_NAME(pin), config);
            #else 
                debug_printf("UnReservePin P%c%d\r\n", _PIN_NAME(pin));
            #endif 
        }

        return true;
    }

    bool Port::OnReserve(Pin pin, bool flag)
    {
        return Reserve(pin, flag);
    }

    bool OutputPort::OnReserve(Pin pin, bool flag)
    {
        debug_printf("Output::");

        return Port::OnReserve(pin, flag);
    }

    bool AlternatePort::OnReserve(Pin pin, bool flag)
    {
        debug_printf("Alternate::");

        return Port::OnReserve(pin, flag);
    }

    bool InputPort::OnReserve(Pin pin, bool flag)
    {
        debug_printf("Input::");

        return Port::OnReserve(pin, flag);
    }

    // �����Ƿ񱻱���
    bool Port::IsBusy(Pin pin)
    {
        int port = pin >> 4, sh = pin &0x0F;
        return (Reserved[port] >> sh) &1;
    }
#endif 

// ��������
#define REGION_Config 1
#ifdef REGION_Config
    

    
    #if 0
        void InputPort::OnConfig(GPIO_InitTypeDef &gpio)
        {
            Port::OnConfig(gpio);

            #ifdef STM32F1
                if (Floating)
                    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                else if (PuPd == PuPd_UP)
                    gpio.GPIO_Mode = GPIO_Mode_IPU;
                else if (PuPd == PuPd_DOWN)
                    gpio.GPIO_Mode = GPIO_Mode_IPD;
                // ����ܲ�ȷ������Ҫ����ʵ�ʽ��е���
            #else 
                gpio.GPIO_Mode = GPIO_Mode_IN;
                //gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
            #endif 
        }
    #endif 
    void AnalogInPort::OnConfig(GPIO_InitTypeDef &gpio)
    {
        Port::OnConfig(gpio);

        #ifdef STM32F1
            gpio.GPIO_Mode = GPIO_Mode_AIN; //
        #else 
            gpio.GPIO_Mode = GPIO_Mode_AN;
            //gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
        #endif 
    }
#endif 

// ����˿�
#define REGION_Output 1
#ifdef REGION_Output
    
#endif 

#if 0
    // ����˿�
    #define REGION_Input 1
    #ifdef REGION_Input
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

        InputPort::InputPort()
        {
            // ȡ�������ж�
            if (_Registed)
                Register(NULL);
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
                    IntState *state = &State;
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
                        IntState *state = &State;
                        state->ShakeTime = ShakeTime;
                        RegisterInput(gi, i, handler, param);
                    }
                    else
                        UnRegisterInput(i);
                }
                n >>= 1;
            }

            _Registed = handler != NULL;
        }

        #define IT 1
        #ifdef IT
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
    #endif //#ifdef REGION_Input
#endif
