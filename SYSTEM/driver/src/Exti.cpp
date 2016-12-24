#include "stm32f10x_exti.h"
#include "Exti.h"

CExti::CExti(uint16_t pin)
{
	this->port=new CPort(pin);
	if(pin ==PA0)
	{
		this->eXTI_Line=EXTI_Line0;
		this->nVIC_IRQChannel=EXTI0_IRQn;
		this->gPIO_PinSource=GPIO_PinSource1;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOA;
	}
	else if(pin ==PA1)
	{
		this->eXTI_Line=EXTI_Line1;
		this->nVIC_IRQChannel=EXTI1_IRQn;
		this->gPIO_PinSource=GPIO_PinSource2;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOA;
	} 
	else if(pin ==PA2)
	{
		this->eXTI_Line=EXTI_Line1;
		this->nVIC_IRQChannel=EXTI2_IRQn;
		this->gPIO_PinSource=GPIO_PinSource3;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOA;
	} 
	else if(pin ==PA3)
	{
		this->eXTI_Line=EXTI_Line3;
		this->nVIC_IRQChannel=EXTI3_IRQn;
		this->gPIO_PinSource=GPIO_PinSource4;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOA;
	} 
	else if(pin ==PA4)
	{
		this->eXTI_Line=EXTI_Line4;
		this->nVIC_IRQChannel=EXTI4_IRQn;
		this->gPIO_PinSource=GPIO_PinSource0;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOA;
	} 
	else if(pin ==PB1)
	{
		this->eXTI_Line=EXTI_Line1;
		this->nVIC_IRQChannel=EXTI1_IRQn;
		this->gPIO_PinSource=GPIO_PinSource1;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOB;
	} 
	else if(pin ==PB2)
	{
		this->eXTI_Line=EXTI_Line2;
		this->nVIC_IRQChannel=EXTI2_IRQn;
		this->gPIO_PinSource=GPIO_PinSource2;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOB;
	} 
	else if(pin ==PB3)
	{
		this->eXTI_Line=EXTI_Line3;
		this->nVIC_IRQChannel=EXTI3_IRQn;
		this->gPIO_PinSource=GPIO_PinSource3;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOB;
	} 
	else
	{
		//Ĭ��ΪPA0
		this->eXTI_Line=EXTI_Line0;
		this->nVIC_IRQChannel=EXTI0_IRQn;
		this->gPIO_PinSource=GPIO_PinSource0;
		this->gPIO_PortSourceGPIO=GPIO_PortSourceGPIOA;
	}
}
CExti::~CExti()
{

}
void CExti::Init()
{
    EXTI_InitTypeDef EXTI_InitStructure;
        
    GPIO_EXTILineConfig(this->gPIO_PortSourceGPIO, this->gPIO_PinSource);
    //PA0 ���ж��ߵ�ӳ�� 

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line; 
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����
    //3���жϷ�ʽ��ѡһ��
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�������ж�
    //EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Rising_Falling//�����غ��½��ض��ж�
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

void CExti::Exti0_state(bool onoff)
//�ⲿ�ж�1��ʼ��  PA0 ��ʹ����ʹ��  ONΪʹ��   OFFΪʹ��
{
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = this->eXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
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

void CExti::On()
{
    this->Exti0_state(true);
}

void CExti::Off()
{
    this->Exti0_state(false);
}
