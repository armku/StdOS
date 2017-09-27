#include "Port.h"


void EXTI_Pxy_ConfigPA0();
void EXTI_Pxy_ConfigPC13(void);
#ifdef DEBUG
    //���Դ���
    extern OutputPort led2;
    InputPort exti(PC13); //PA1 PB3     PA0 PC13
    InputPort exti1(PA0);
    void OnKeyPress(InputPort *pin, bool down, void *param)
    {
        //led1.Write(onoff);
        led2 = !led2;
        //    printf("�ж����ţ�P%c%d ֵ%d \r\n", _PIN_NAME(pin), down);
    }


    InputPort key0(PA0);
    //�����¼�
    void OnPress(InputPort &port, bool down)
    {
        debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
    }
    InputPort key(PC13);
	void led2flash(void *param)
	{
		OnKeyPress(&exti,false,param);
	}
    void InterruptTest()
    {
        /* exti line config */
        EXTI_Pxy_ConfigPA0();
        EXTI_Pxy_ConfigPC13();
		
        exti1.UsePress();

        key.Press = OnPress;
        key.UsePress();
        key.Open();
        //        exti.Register(OnKeyPress);
        //        exti1.Register(OnKeyPress);
		Sys.AddTask(led2flash,&led2,0,200,"led2");
    }
#endif
#include "stm32f10x.h"
#include "TInterrupt.h"

//�ж��ߴ򿪡��ر�
void SetEXIT(int pinIndex, bool enable);

static const int PORT_IRQns[] = 
{
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5��������
	EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
	EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
};

void InputPort_OpenEXTI(Pin pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	
	SetEXIT(pin&0X0F, true);
	//Interrupt.SetPriority(PORT_IRQns[1], 1u);
	//Interrupt.Activate(PORT_IRQns[v3],(void (__cdecl *)(unsigned __int16, void *))EXTI_IRQHandler,v1);
}
 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_ConfigurationPA0()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Pxy_ConfigPA0()
{	
	InputPort_OpenEXTI(PA0);
												
	/* config the NVIC */
	NVIC_ConfigurationPA0();
}

static void NVIC_ConfigurationPC13(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  ���� PC13 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Pxy_ConfigPC13(void)
{	
	InputPort_OpenEXTI(PC13);
	
	/* config the NVIC */
	NVIC_ConfigurationPC13();
}


/*
�����ȡ
 */
//ushort OutputPort::ReadGroup()
//{
//    return GPIO_ReadOutputData(((GPIO_TypeDef *)this->State));
//}
#if 0   
    // ����֮ǰ����Ҫ���ݵ���������趨��ʼ״̬��Ҳ�����ڴ򿪶˿�֮ǰ������ȷ�˿ڸߵ�״̬
    ushort dat = GPIO_ReadOutputData(((GPIO_TypeDef*)this->State));
    //    if (!Invert)
    //        dat &= ~PinBit;
    //    else
    //        dat |= PinBit;
    GPIO_Write(((GPIO_TypeDef*)this->State), dat);
#endif 

//void OutputPort::WriteGroup(ushort value)
//{
//    GPIO_Write(((GPIO_TypeDef *)this->State), value);
//}
#if 0
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
#endif 

#if 0
    ushort InputPort::ReadGroup() // �����ȡ
    {
        return GPIO_ReadInputData(((GPIO_TypeDef*)this->State));
    }
#endif 


#if 0
    // ע��ص�  ���ж�ʹ��
    void InputPort::Register(IOReadHandler handler, void *param)
    {
        //    if (!PinBit)
        //        return ;

        // ��鲢��ʼ���ж�������
        if (!hasInitState)
        {
            for (int i = 0; i < 16; i++)
            {
                IntState *state1 = &InterruptState[i];
                state1->Pin = P0;
                state1->Handler = NULL;
                state1->Used = 0;
            }
            hasInitState = true;
        }

        byte gi = _Pin >> 4;
        gi = gi;
        //    ushort n = PinBit;
        ushort n = 0;
        for (int i = 0; i < 16 && n != 0; i++)
        {
            // �����������һλ����ע���¼�
            if (n &0x01)
            {
                // ע���ж��¼�
                if (handler)
                {
                    IntState *state2 = &InterruptState[i];
                    state2->ShakeTime = ShakeTime;
                    RegisterInput(gi, i, handler, param);
                }
                else
                {
                    UnRegisterInput(i);
                }
            }
            n >>= 1;
        }

        //    _Registed = handler != NULL;
    }
#endif 

#if 0
    // ���������ж��й�
    void InputPort::RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param)
    {
        IntState *state4 = &InterruptState[pinIndex];
        Pin pin = (Pin)((groupIndex << 4) + pinIndex);
        // ����Ƿ��Ѿ�ע�ᵽ���������
        if (state4->Pin != pin && state4->Pin != P0)
        {
            return ;
        }
        state4->Pin = pin;
        state4->Handler = handler;
        state4->Param = param;
        //    state4->OldValue = Read(pin); // Ԥ�ȱ��浱ǰ״ֵ̬����������ʱ�����ж�

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
        Interrupt.SetPriority(PORT_IRQns[pinIndex]);
        state4->Used++;
        if (state4->Used == 1)
        {
            Interrupt.Activate(PORT_IRQns[pinIndex], EXTI_IRQHandler, this);
        }
    }
#endif 
#if 0
    void InputPort::UnRegisterInput(int pinIndex)
    {
        IntState *state5 = &InterruptState[pinIndex];
        // ȡ��ע��
        state5->Pin = P0;
        state5->Handler = 0;

        SetEXIT(pinIndex, false);

        state5->Used--;
        if (state5->Used == 0)
        {
            Interrupt.Deactivate(PORT_IRQns[pinIndex]);
        }
    }
#endif
