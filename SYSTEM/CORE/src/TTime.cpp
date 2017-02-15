#include "TTime.h"
#include "stm32f10x.h"

extern byte fac_us; //ÿ��us��Ҫ��systickʱ���� 
#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //systick�жϷ�����,ʹ��ucosʱ�õ�
    void SysTick_Handler(void)
    {
        Time.mCurrent++;
    }

    //��ʱnus
    //nusΪҪ��ʱ��us��.
    void delay_us(uint nus)
    {
        uint ticks;
        uint told, tnow, tcnt = 0;
        uint reload = SysTick->LOAD; //LOAD��ֵ
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
        };
    }
    //����Ϊ��ຯ��
    void WFI_SET(void); //ִ��WFIָ��
    void INTX_DISABLE(void); //�ر������ж�
    void INTX_ENABLE(void); //���������ж�
    void MSR_MSP(uint addr); //���ö�ջ��ַ
    //THUMBָ�֧�ֻ������
    //�������·���ʵ��ִ�л��ָ��WFI  
    void WFI_SET(void)
    {
        __ASM volatile("wfi");
    }

    //�ر������ж�
    void INTX_DISABLE(void)
    {
        __ASM volatile("cpsid i");
    }

    //���������ж�
    void INTX_ENABLE(void)
    {
        __ASM volatile("cpsie i");
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
TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��

TTime::TTime(){}
//���ص�ǰ������
UInt64 TTime::Ms()const
{
    return this->mCurrent;
}

//����ϵͳ��ǰʱ��,Unix��ʽ����
void TTime::SetTime(UInt64 seconds){

}
//ϵͳʱ��
uint TTime::Seconds()
{
    return 0;
}

//��ʱms
void TTime::Sleep(uint us)
{
    delay_us(us);
}
extern UInt64 TicksPerms;
//ϵͳ���������ĺ���
UInt64 TTime::Current()const
{
	UInt64 ret=this->mCurrent *1000;
	ret+=(TicksPerms-SysTick->VAL)/fac_us;
    return ret;
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

/*
1us=30ticks
 */
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
