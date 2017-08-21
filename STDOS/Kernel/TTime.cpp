#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��

void TTime::OnHandler(ushort num, void *param){

}

TTime::TTime()
{
    this->BaseSeconds = 0;
    this->Seconds = 0;
}

void RtcRefresh(void *param)
{
    HardRTC *rtc = (HardRTC*)param;
    rtc->LoadTime();
}

// ʹ��RTC��������Initǰ����
void TTime::UseRTC()
{
    HardRTC *rtc = HardRTC::Instance();
    rtc->LowPower = false;
    rtc->External = false;

    rtc->Init();
    Sys.AddTask(RtcRefresh, rtc, 100, 100, "Rtc");
}

extern uint time6cnt;
Timer *timerTick;
Delegate < Timer & > abc;
void timTickrefesh(void *param)
{
    Time.Milliseconds += 1000;
    Time.Seconds++;
    time6cnt++;
}

static byte fac_us = 0; //us��ʱ������ ÿ��us��Ҫ��systickʱ���� 			   
static ushort fac_ms = 0; //ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
void TTime::Init()
{

    // ��ʼ��Ϊ���
    #if defined STM32F0
        this->Index = 6;
    #elif defined STM32F1
        this->Index = 5;
    #elif defined STM32F4
        this->Index = 2;
    #endif 
    timerTick = new Timer((TIMER)this->Index);
    abc.Bind(timTickrefesh);
    timerTick->Register(abc);
    timerTick->Open();
    //timerTick->SetFrequency(1000);
    //        timer2->Config();

    //��ʼ���ӳٺ���
    //SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
    //SYSCLK:ϵͳʱ��
    #if defined STM32F0
    #elif defined STM32F1
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
        fac_us = SystemCoreClock / 8000000; //Ϊϵͳʱ�ӵ�1/8  
        fac_ms = (ushort)fac_us *1000; //����ÿ��ms��Ҫ��systickʱ����   
    #elif defined STM32F4
        uint SYSCLK = 168;
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
        fac_us = SYSCLK / 8; //�����Ƿ�ʹ��OS,fac_us����Ҫʹ��

        fac_ms = (u16)fac_us *1000; //��OS��,����ÿ��ms��Ҫ��systickʱ����   
    #endif 

    #ifdef STM32F0
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ���� 
    #elif defined STM32F1
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ���� 
    #elif defined STM32F4
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ���� 
    #endif 
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    SysTick_Config(9000); //����SysTick tick is 9ms	9000
    #ifdef STM32F0
    #elif defined STM32F1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�jtag������swd	
    #elif defined STM32F4
    #endif 
    NVIC_SetPriority(SysTick_IRQn, 0);
	switch(this->Index)
	{
		case 2:
			NVIC_SetPriority(TIM3_IRQn, 0);
			break;
		case 5:
			#if defined STM32F0
			#else
				NVIC_SetPriority(TIM6_IRQn, 0);
			#endif
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
    return systickcnt;
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

// ����ʱ��
void TTime::SetTime(UInt64 seconds){

}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void TTime::Sleep(int nms, bool *running)const
{
    this->Delay(nms *1000);
}

// ΢�뼶�ӳ�
void TTime::Delay(int nus)const
{
    uint ticks;
    uint told, tnow, tcnt = 0;
    uint reload = 0;
    #if defined(STM32F1) || defined(STM32F4)
        reload = SysTick->LOAD; //LOAD��ֵ
    #elif defined STM32F0
        reload = SysTick->LOAD; //LOAD��ֵ
    #endif 
    ticks = nus * fac_us; //��Ҫ�Ľ�����
    tcnt = 0;
    #if defined(STM32F1) || defined(STM32F4)
        told = SysTick->VAL; //�ս���ʱ�ļ�����ֵ
    #elif defined STM32F0
        told = SysTick->VAL; //�ս���ʱ�ļ�����ֵ
    #endif 
    while (1)
    {
        #if defined(STM32F1) || defined(STM32F4)
            tnow = SysTick->VAL;
        #elif defined STM32F0
            tnow = SysTick->VAL;
        #endif 
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

uint TTime::TicksToUs(uint ticks)const
{
    if (!ticks)
    {
        return 0;
    }
    //1000000 = 64 * 15625 = 2^6 * 15625
    //(0xFFFFFFFF / (1000000 >> 6)) = 274877.90688
    //���TICKS<274877,��ô�˷��������
    if (ticks <= (0xffffffff / (1000000 >> 6)))
    {
        return (ticks *(1000000 >> 6)) >> (15-6);
    }
    else
    {
        return ((UInt64)ticks *(1000000 >> 6)) >> (16-6);
    }
}

uint TTime::UsToTicks(uint us)const
{
    if (!us)
    {
        return 0;
    }

    //1000000 = 64 * 15625 = 2^6 * 15625
    //((0xffffffff +1) >> (15-6)) = 8388608 = 0x800000 = 1 << (32 - 9)
    //���ticks<274877,��ô�˷��������
    if (us < (1 << (32-(15-6))))
    {
        return (us << (15-6)) / (1000000 >> 6);
    }
    else
    {
        return ((UInt64)us << (15-6)) / (1000000 >> 6);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
TimeWheel::TimeWheel(uint ms){

}

void TimeWheel::Reset(uint ms){

}
// �Ƿ��ѹ���
bool TimeWheel::Expired()
{
    return true;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
    this->Start = 0;
}

void TimeCost::Reset(){}
// ��ȥ��ʱ�䣬΢��
int TimeCost::Elapsed()const
{
    uint ret = 12;
    ret -= this->Start;
    return ret;
}

void TimeCost::Show(cstring format)const
{
    printf("ִ�� %d ΢��\r\n", 12);
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
        #if defined(STM32F1) || defined(STM32F4)
            __ASM volatile("wfi");
        #elif defined STM32F0
            __ASM volatile("wfi");
        #endif 
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
