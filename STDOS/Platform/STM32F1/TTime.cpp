/*
���ļ���Ҫ�Ż���
 */
#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"
#include "stm32f10x.h"

extern Timer *timerTick;
extern Delegate < Timer & > abc;
extern int gTicks; //
void timTickrefesh(void *param);
void TTime::Init()
{
    // ��ʼ��Ϊ���
    this->Index = 5;

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
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    SysTick_Config(9000); //����SysTick tick is 9ms	9000
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�jtag������swd	
    NVIC_SetPriority(SysTick_IRQn, 0);
    switch (this->Index)
    {
        case 2:
            NVIC_SetPriority(TIM3_IRQn, 0);
            break;
        case 5:
            NVIC_SetPriority(TIM6_IRQn, 0);
            break;
        case 6:
            NVIC_SetPriority(TIM7_IRQn, 0);
            break;
        default:
            break;
    }
}

// ��ǰ�δ�ʱ��
uint TTime::CurrentTicks()const
{
    SmartIRQ irq;

//    uint value = (SysTick->LOAD - SysTick->VAL);
//    if (SysTick->CTRL &SysTick_CTRL_COUNTFLAG)
//    {
//        systickcnt += SysTick->LOAD;
//    }
    return SysTick->LOAD - SysTick->VAL;
}

// ��ǰ������
UInt64 TTime::Current()const
{
    __IO ushort ms = 0;
    switch (this->Index)
    {
        case 2:
            ms = (TIM3->CNT) >> 1;
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
    uint ticks=0;
    uint told=0;
	uint tnow=0;
	uint tcnt = 0;
	uint tin = 0;//�ս���ʱ��ʱ��
    uint reload = 0;
	if(nus > 1000)
		nus=1000;
    reload = SysTick->LOAD; //LOAD��ֵ
    ticks = nus * gTicks; //��Ҫ�Ľ�����
    tcnt = 0;
    tin = SysTick->VAL; //�ս���ʱ�ļ�����ֵ
	
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < tin)
                tcnt = tin - tnow;
            //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else
                tcnt = reload - tnow + tin;
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
