#include "BspPlatform.h"
#if defined STM32F0
#elif defined STM32F1
#include "stm32f10x.h"
#elif defined STM32F4
#endif
#include "Sys.h"
#include "Core\Queue.h"
#include "Port.h"
#include "Platform\stm32.h"
#include "Device\DeviceConfigHelper.h"

void TimeUpdate(); void TIM3_Int_Init(u16 arr, u16 psc);
void BspPlatformInit()
{	
	DeviceConfigCenter::ConfigCom(COM1, 256000);
	DeviceConfigCenter::ConfigCom(COM3, 256000);
	DeviceConfigCenter::TimeTickInit();//ϵͳ�ö�ʱ����ʼ��
	DeviceConfigCenter::PTim2Update = TimeUpdate;
	DeviceConfigCenter::PTim3Update = TimeUpdate;//F4
	TIM3_Int_Init(5000 - 1, 8400 - 1);
}
//F4
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  ///ʹ��TIM3ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);//��ʼ��TIM3

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
int DeviceConfigCenter::CurrentTick()
{
#if defined STM32F0
	return (TIM2->CNT) >> 1;
#elif defined STM32F1
	return (TIM2->CNT) >> 1;
#elif defined STM32F4
	return (TIM2->CNT) >> 1;
#endif
}
uint32_t DeviceConfigCenter::CurrentTicks1()
{
#if defined STM32F0
	return SysTick->LOAD - SysTick->VAL;
#elif defined STM32F1
	return SysTick->LOAD - SysTick->VAL;
#elif defined STM32F4
	return SysTick->LOAD - SysTick->VAL;
#endif
}
#include <stdio.h>
#include "stdarg.h"
/////////////////////////////////////////////////////////////////////////////
int StdPrintf(const char *format, ...)
{
	static char sprint_buf[1024];
	va_list args;

	int n;
	va_start(args, format);
	n = vsprintf(sprint_buf, format, args);
	va_end(args);

	DeviceConfigCenter::comSend(COM1, Buffer(sprint_buf, n));
	return n;
}

