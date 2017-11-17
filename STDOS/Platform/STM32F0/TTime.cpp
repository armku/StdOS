/*
���ļ���Ҫ�Ż���
*/
#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"
#include "stm32f0xx.h"

extern Timer *timerTick;
extern Delegate < Timer & > abc;
extern int gTicks; //
extern byte fac_us; //us��ʱ������ ÿ��us��Ҫ��systickʱ���� 	
void timTickrefesh(void *param);
void TTime::Init()
{
    // ��ʼ��Ϊ���
        this->Index = 6;
	
	gTicks = (Sys.Clock >> 3) / 0xF4240u;
	
    timerTick = new Timer((TIMER)this->Index);
    abc.Bind(timTickrefesh);
    timerTick->Register(abc);
    timerTick->Open();
    //timerTick->SetFrequency(1000);
    //        timer2->Config();

    //��ʼ���ӳٺ���
    //SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
    //SYSCLK:ϵͳʱ��
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ���� 
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    SysTick_Config(9000); //����SysTick tick is 9ms	9000
    NVIC_SetPriority(SysTick_IRQn, 0);
	switch(this->Index)
	{
		case 2:
			NVIC_SetPriority(TIM3_IRQn, 0);
			break;
		case 5:
			break;
		case 6:
			NVIC_SetPriority(TIM7_IRQn, 0);
			break;
		default:
			break;
	}
}
extern uint systickcnt;
// ��ǰ�δ�ʱ��
uint TTime::CurrentTicks()const
{
	SmartIRQ irq;

    uint value = (SysTick->LOAD - SysTick->VAL);	
    return systickcnt + value;
}

// ��ǰ������
UInt64 TTime::Current()const
{
    __IO ushort ms = 0;
    switch (this->Index)
    {
		case 2:
			ms=(TIM3->CNT) >> 1;
			break;
        case 5:
            ms = (TIM6->CNT) >> 1;
            break;
        case 6:
            ms = (TIM7->CNT) >> 1;
            break;
        default:
            break;
    }
    return this->Milliseconds + ms;
}
// ΢�뼶�ӳ�
void TTime::Delay(int nus)const
{
    uint ticks;
    uint told, tnow, tcnt = 0;
    uint reload = 0;
        reload = SysTick->LOAD; //LOAD��ֵ
    ticks = nus * fac_us; //��Ҫ�Ľ�����
    tcnt = 0;
        told = SysTick->VAL; //�ս���ʱ�ļ�����ֵ
    while (1)
    {
            tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break;
            //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }
    }
}
#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //����Ϊ��ຯ��
    //THUMBָ�֧�ֻ������
    //�������·���ʵ��ִ�л��ָ��WFI  
    void WFI_SET(void)
    {
            __ASM volatile("wfi");
    }

    //����ջ����ַ
    //addr:ջ����ַ
    __asm void MSR_MSP(uint addr)
    {
        MSR MSP, r0  //set Main Stack value
        BX r14
    }
    #ifdef __cplusplus
    }
#endif
