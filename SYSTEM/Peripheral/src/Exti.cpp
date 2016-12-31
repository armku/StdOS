#include "stm32f10x_exti.h"
#include "Exti.h"

CExti::CExti(PinPort pin)
{
    this->port = new BasePort(pin);
    this->port->SetModeINPUT_IPD();
    #if 0
        //默认为PA0
        this->eXTI_Line = EXTI_Line0;
        this->nVIC_IRQChannel = EXTI0_IRQn;
        this->gPIO_PinSource = GPIO_PinSource0;
        this->gPIO_PortSourceGPIO = GPIO_PortSourceGPIOA;
    #else 
        this->eXTI_Line = EXTI_Line0 + pin % 16;
        if ((pin % 16) < 5)
        {
            this->nVIC_IRQChannel = EXTI0_IRQn + (pin % 16);
        }
        else
        {
            this->nVIC_IRQChannel = EXTI15_10_IRQn;
        }
        this->gPIO_PinSource = GPIO_PinSource0 + pin % 16;
        this->gPIO_PortSourceGPIO = GPIO_PortSourceGPIOA + pin / 16;
    #endif 
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
