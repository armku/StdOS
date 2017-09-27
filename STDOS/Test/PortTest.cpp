#include "Port.h"


void EXTI_Pxy_ConfigPA0();
void EXTI_Pxy_ConfigPC13(void);
#ifdef DEBUG
    //测试代码
    extern OutputPort led2;
    InputPort exti(PC13); //PA1 PB3     PA0 PC13
    InputPort exti1(PA0);
    void OnKeyPress(InputPort *pin, bool down, void *param)
    {
        //led1.Write(onoff);
        led2 = !led2;
        //    printf("中断引脚：P%c%d 值%d \r\n", _PIN_NAME(pin), down);
    }


    InputPort key0(PA0);
    //按键事件
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

//中断线打开、关闭
void SetEXIT(int pinIndex, bool enable);

static const int PORT_IRQns[] = 
{
	EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
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
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_ConfigurationPA0()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  配置 PA0 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
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
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  配置 PC13 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Pxy_ConfigPC13(void)
{	
	InputPort_OpenEXTI(PC13);
	
	/* config the NVIC */
	NVIC_ConfigurationPC13();
}


/*
整组读取
 */
//ushort OutputPort::ReadGroup()
//{
//    return GPIO_ReadOutputData(((GPIO_TypeDef *)this->State));
//}
#if 0   
    // 配置之前，需要根据倒置情况来设定初始状态，也就是在打开端口之前必须明确端口高低状态
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
        // 有些应用的输入口需要极高的灵敏度，这个时候不需要抖动检测
        ShakeTime = 0;
        Invert = false;
    }
#endif 

#if 0
    ushort InputPort::ReadGroup() // 整组读取
    {
        return GPIO_ReadInputData(((GPIO_TypeDef*)this->State));
    }
#endif 


#if 0
    // 注册回调  及中断使能
    void InputPort::Register(IOReadHandler handler, void *param)
    {
        //    if (!PinBit)
        //        return ;

        // 检查并初始化中断线数组
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
            // 如果设置了这一位，则注册事件
            if (n &0x01)
            {
                // 注册中断事件
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
    // 申请引脚中断托管
    void InputPort::RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param)
    {
        IntState *state4 = &InterruptState[pinIndex];
        Pin pin = (Pin)((groupIndex << 4) + pinIndex);
        // 检查是否已经注册到别的引脚上
        if (state4->Pin != pin && state4->Pin != P0)
        {
            return ;
        }
        state4->Pin = pin;
        state4->Handler = handler;
        state4->Param = param;
        //    state4->OldValue = Read(pin); // 预先保存当前状态值，后面跳变时触发中断

        // 打开时钟，选择端口作为端口EXTI时钟线
        #if defined(STM32F0) || defined(STM32F4)
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
            SYSCFG_EXTILineConfig(groupIndex, pinIndex);
        #elif defined(STM32F1)
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            GPIO_EXTILineConfig(groupIndex, pinIndex);
        #endif 

        SetEXIT(pinIndex, true);
        // 打开并设置EXTI中断为低优先级
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
        // 取消注册
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
