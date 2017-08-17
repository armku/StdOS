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

extern byte fac_us; //ÿ��us��Ҫ��systickʱ���� 
TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
extern UInt64 TicksPerms;

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
Timer *timer2;
Delegate < Timer & > abc;
void tim2refesh(void *param)
{
    time6cnt++;
}
void TTime::Init()
{
	// ��ʼ��Ϊ���
	#if defined STM32F0
	#elif defined STM32F1
		timer2 = new Timer(Timer6);
	#elif defined STM32F4
	#endif
    abc.Bind(tim2refesh);
    timer2->Register(abc);
    timer2->Open();
    timer2->SetFrequency(1000);
    //        timer2->Config();
}
// ��ǰ�δ�ʱ��
uint TTime::CurrentTicks()const
{
    return 0;
}

// ��ǰ������
UInt64 TTime::Current()const
{
    UInt64 ret = 0;
    #if defined(STM32F1) || defined(STM32F4)	
        ret = this->Milliseconds *1000; //��ת��Ϊus
        ret += (TicksPerms - SysTick->VAL) / fac_us;
    #elif defined STM32F0
        ret = this->Milliseconds *1000; //��ת��Ϊus
        ret += (TicksPerms - SysTick->VAL) / fac_us;
    #endif 
    //return this->mCurrent *1000;
    return ret;
}

// ����ʱ��
void TTime::SetTime(UInt64 seconds){

}

void TTime::Sleep(int ms, bool *running)const
{
    this->Delay(ms *1000);
}

// ΢�뼶�ӳ�
void TTime::Delay(int us)const
{
    uint ticks;
    uint told, tnow, tcnt = 0;
    uint reload = 0;
    #if defined(STM32F1) || defined(STM32F4)
        reload = SysTick->LOAD; //LOAD��ֵ
    #elif defined STM32F0
        reload = SysTick->LOAD; //LOAD��ֵ
    #endif 
    ticks = us * fac_us; //��Ҫ�Ľ�����
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
    };
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
