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
PB3
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
#include "stm32f10x_exti.h"
#include "Exti.h"

CExti::CExti(PinPort pin)
{
    this->port = new BasePort(pin);
    this->port->SetModeINPUT_IPD();

    this->eXTI_Line = EXTI_Line0 << (pin % 16);
    if ((pin % 16) < 5)
    {
        this->nVIC_IRQChannel = EXTI0_IRQn + (pin % 16);
    }
    else if((pin%16)<11)
    {
        this->nVIC_IRQChannel = EXTI9_5_IRQn;
    }
	else
	{
		this->nVIC_IRQChannel = EXTI15_10_IRQn;
	}
    this->gPIO_PinSource = GPIO_PinSource0 + pin % 16;
    this->gPIO_PortSourceGPIO = GPIO_PortSourceGPIOA + pin / 16;
}

void CExti::Init()
{
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(this->gPIO_PortSourceGPIO, this->gPIO_PinSource);
    //PA0 与中断线的映射 

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断请求
    //3中中断方式任选一个
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿中断
    //EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Rising_Falling//上升沿和下降沿都中断
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

void CExti::Exti0_state(bool onoff)
//外部中断1初始化  PA0 的使能是使能  ON为使能   OFF为使能
{
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
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

void CExti::On()
{
    this->Exti0_state(true);
}

void CExti::Off()
{
    this->Exti0_state(false);
}
