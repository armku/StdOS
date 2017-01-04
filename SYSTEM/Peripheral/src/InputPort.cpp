#include "stm32f10x_exti.h"
#include "InputPort.h"

InputPort::InputPort(Pin pin,byte invert):Port()
{
	this->Set(pin);
	this->SetModeOut_PP();
	switch(invert)
	{
	case 0:
		this->binvert=invert;
		break;
	case 1:
		this->binvert=invert;
		break;
	case 2:
		if(this->Read())
		{
		this->binvert=true;
		}
		else
		{
		this->binvert=false;
		}
		break;
	default:
		break;
	}
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
void InputPort::Init()
{	
	EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(this->gPIO_PortSourceGPIO, this->gPIO_PinSource);
    //PA0 与中断线的映射 

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断请求
    //3中中断方式任选一个
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿中断
    EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Rising_Falling;//上升沿和下降沿都中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = this->nVIC_IRQChannel; //中断向量
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级0 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级0 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
    //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

/* 中断状态结构体 */
/* 一共16条中断线，意味着同一条线每一组只能有一个引脚使用中断 */
typedef struct TIntState
{
    Pin Pin;
    BtnInCallback Handler;
    bool OldValue;
} IntState;

// 16条中断线
static IntState State[16];

void InputPort::Exti0_state(bool onoff)
//外部中断1初始化  PA0 的使能是使能  ON为使能   OFF为使能
{
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //下降沿中断
    if (onoff)
    {
        EXTI_InitStructure.EXTI_LineCmd = ENABLE; //中断使能	
    }
    else
    {
        EXTI_InitStructure.EXTI_LineCmd = DISABLE; //中断使能
    }
    EXTI_Init(&EXTI_InitStructure);
}

void InputPort::On()
{
    this->Exti0_state(true);
}

void InputPort::Off()
{
    this->Exti0_state(false);
}

// 注册回调  及中断使能
void InputPort::Register(BtnInCallback handler)
{
    byte pins = this->_Pin &0x0F;
    IntState *state = &State[pins];
    // 注册中断事件
    if (handler)
    {
        // 检查是否已经注册到别的引脚上
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
        // 取消注册
        state->Pin = P0;
        state->Handler = 0;
    }	
}
void InputPort::Toggle()
{
    this->pinbit = !this->pinbit;
    if (this->pinbit)
    {
        this->Set();
    }
    else
    {
        this->Reset();
    }
}

extern "C"
{   
    void GPIO_ISR(int num) // 0 <= num <= 15
    {
        IntState *state = &State[num];
		
		// 如果未指定委托，则不处理
        if (!state->Handler)
		{
            return ;
		}
				
        bool value;
                
        value = Port::ReadPinPort(state->Pin); // 获取引脚状态
        
        if (state->Handler)
        {
            state->Handler(state->Pin, value);
        }
    }

    void EXTI0_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line0) != RESET)
        {

            EXTI_ClearITPendingBit(EXTI_Line0); //清除中断标志位	
			GPIO_ISR(0);
        }
    }

    void EXTI1_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line1); //清除中断标志位
			GPIO_ISR(1);
        } 
    }

    void EXTI2_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line2); //清除中断标志位           
			GPIO_ISR(2);
        }
    }

    void EXTI3_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line3); //清除中断标志位
			GPIO_ISR(3);
        }
    }

    void EXTI4_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line4); //清除中断标志位
			GPIO_ISR(4);
        }
    }

    void EXTI9_5_IRQHandler()
    {
        if (EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line5); //清除中断标志位
			GPIO_ISR(5);
        }
        if (EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line6); //清除中断标志位		
        	GPIO_ISR(6);
        }
        if (EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line7); //清除中断标志位		
        	GPIO_ISR(7);
        }
        if (EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line8); //清除中断标志位		
        	GPIO_ISR(8);
        }
        if (EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line9); //清除中断标志位		
        	GPIO_ISR(9);
        }
    }
    /// IO 线中断，中断口为PC13
    void EXTI15_10_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line10); //清除中断标志位		
        	GPIO_ISR(10);
        }
        if (EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line11); //清除中断标志位		
        	GPIO_ISR(11);
        }
        if (EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line12); //清除中断标志位		
        	GPIO_ISR(12);
        }
        if (EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line13); //清除中断标志位		
        	GPIO_ISR(13);
        }
        if (EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line14); //清除中断标志位		
        	GPIO_ISR(14);
        }
        if (EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            EXTI_ClearITPendingBit(EXTI_Line15); //清除中断标志位		
        	GPIO_ISR(15);
        }
    }
}

/*
测试通过
PA0 √
PA1
PA2 ?
PA3 ?
PA4
PA5
PA6
PA7
PA8 √
PA9 √
PA10 ?
PA11 √
PA12 √
PA13
PA14
PA15

PB0 √
PB1 √
PB2 √
PB3 √
PB4 √
PB5 √
PB6 √
PB7 √
PB8 √
PB9 ?
PB10 ?
PB11 √
PB12 √
PB13 √
PB14 √
PB15 √

PC0 √
PC1 √
PC2 √
PC3 √
PC4 √
PC5 √
PC6 √
PC7 √
PC8 √
PC9 √
PC10 ?
PC11 √
PC12 √
PC13 √
PC14
PC15

PD0
PD1
PD2 √
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
PE6 √
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
PG8 √
PG9 √
PG10
PG11
PG12
PG13
PG14
PG15 √
 */
