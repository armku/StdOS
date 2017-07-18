#include "TTime.h"
#include "Device\RTC.h"
#ifdef STM32F1
    #include "stm32f10x.h"
#endif 
#ifdef STM32F4
    #include "stm32f4xx.h"
#endif 

extern byte fac_us; //ÿ��us��Ҫ��systickʱ���� 
TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
extern UInt64 TicksPerms;

void TTime::OnHandler(ushort num, void *param){

}

TTime::TTime()
{
	this->BaseSeconds=0;
	this->Seconds=0;
}
void RtcRefresh(void *param)
{
	HardRTC *rtc = (HardRTC*)param;
    rtc->LoadTime();
}
// ʹ��RTC��������Initǰ����
void TTime::UseRTC()
{
	HardRTC *rtc=HardRTC::Instance();
	rtc->LowPower = false;
    rtc->External = false;

	rtc->Init();
	Sys.AddTask(RtcRefresh, rtc, 100, 100, "Rtc");
}
void TTime::Init()
{

}
// ��ǰ�δ�ʱ��
uint TTime::CurrentTicks()const
{
    return 0;
}

// ��ǰ������
UInt64 TTime::Current()const
{
    #if 1
        UInt64 ret = this->Milliseconds *1000;//��ת��Ϊus
        ret += (TicksPerms - SysTick->VAL) / fac_us;
        return ret;
    #else 
        return this->mCurrent *1000;
    #endif 
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
    uint reload = SysTick->LOAD; //LOAD��ֵ
    ticks = us * fac_us; //��Ҫ�Ľ�����
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

    //systick�жϷ�����,ʹ��ucosʱ�õ�
    void SysTick_Handler(void)
    {
        Time.Milliseconds++;
    }

    //����Ϊ��ຯ��
    void WFI_SET(void); //ִ��WFIָ��    
    void MSR_MSP(uint addr); //���ö�ջ��ַ
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
