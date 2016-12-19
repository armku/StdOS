#include "stm32f10x_exti.h"
#include "Exti.h"

CExti::CExti(uint16_t pin)
{
	this->port=new CPort(pin);
}
CExti::~CExti()
{

}
void CExti::Init()
{
    EXTI_InitTypeDef EXTI_InitStructure;
        
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    //PA0 ���ж��ߵ�ӳ�� 

    EXTI_InitStructure.EXTI_Line = EXTI_Line0; //��0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����
    //3���жϷ�ʽ��ѡһ��
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�������ж�
    //EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Rising_Falling//�����غ��½��ض��ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ʹ��PA0���ڵ��ⲿ�ж�ͨ��
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

    EXTI_InitStructure.EXTI_Line = EXTI_Line0; //��0
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
