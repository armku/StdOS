#include "BspPlatform.h"
#include "Sys.h"
#include "Core\Queue.h"
#include "Port.h"
#include "Platform\stm32.h"
#include "Device\DeviceConfigHelper.h"

void NVIC_Configuration(void);//�ж����ȼ���ʼ����������
void TIM2_Configuration(void);

int main123(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ��TIM2ʱ��
	NVIC_Configuration();  //�����ж����ȼ���ʼ����������
	TIM2_Configuration();  //����TIM3��ʼ����������
}

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//����һ��TIM_InitTypeDef���͵Ľṹ��
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//���жϱ�־���Ա��´��жϵ���ʹ��

	TIM_TimeBaseStructure.TIM_Period = 1000;//1���ӻ�2000������
	TIM_TimeBaseStructure.TIM_Prescaler = 35999; //36000��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//��ʼ����ʱ��
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ������ж�

	TIM_Cmd(TIM2, ENABLE);//��ʱ��ʹ��
}

static void NVIC_Configuration(void)//�ж����ȼ���ʼ����
{
	NVIC_InitTypeDef   NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TimeUpdate();
void BspPlatformInit()
{	
	DeviceConfigCenter::ConfigCom(COM1, 256000);
	DeviceConfigCenter::ConfigCom(COM3, 256000);
	DeviceConfigCenter::TimeTickInit();//ϵͳ�ö�ʱ����ʼ��
	DeviceConfigCenter::PTim2Update = TimeUpdate;
	main123();
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
