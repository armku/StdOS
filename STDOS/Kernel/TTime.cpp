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



int clock()
{
  return  Time.Current();
}

//int __fastcall time(int *a1)
//{
//  int *v1; // r1@1
//  int result; // r0@1

//  v1 = a1;
//  result = *((_DWORD *)&Time + 4) + Time;
//  if ( v1 )
//    *v1 = result;
//  return result;
//}


TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��

void TTime::OnHandler(ushort num, void *param)
{
//	int v3; // r5@1

//  v3 = a2;
//  if ( a2 && TIM_GetITStatus(a2, 1, a3) )
//  {
//    ++Time;
//    *((_QWORD *)&Time + 1) += 1000LL;
//    TIM_ClearITPendingBit(v3, 1);
//    if ( *((_DWORD *)&Time + 8) )
//      (*((void (**)(void))&Time + 8))();
//  }
}

TTime::TTime()
{
    this->BaseSeconds = 0;
    this->Seconds = 0;
	
//	*(_DWORD *)result = 0;
//  *(_BYTE *)(result + 21) = 0;
//  if ( (signed int)*((_WORD *)&Sys + 30) <= 128 )
//    *(_BYTE *)(result + 20) = 1;
//  else
//    *(_BYTE *)(result + 20) = 5;
//  *(_DWORD *)(result + 16) = 0;
//  *(_DWORD *)(result + 24) = 0;
//  *(_DWORD *)(result + 28) = 0;
//  *(_DWORD *)(result + 32) = 0;
//  *(_DWORD *)(result + 36) = 0;
//  return result;
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
			#elif defined STM32F1
				NVIC_SetPriority(TIM6_IRQn, 0);
			#elif defined STM32F4
			#endif
			break;
		case 6:
			NVIC_SetPriority(TIM7_IRQn, 0);
			break;
		default:
			break;
	}
	
	
//	TTime *v3; // r4@1
//  void *v4; // r7@1
//  int v5; // r0@1
//  int v6; // r6@1
//  signed __int16 v7; // r8@3
//  unsigned int v8; // r5@3
//  __int16 v9; // r9@6
//  __int64 v11; // [sp+4h] [bp-34h]@6
//  __int16 v12; // [sp+Ch] [bp-2Ch]@6
//  __int16 v13; // [sp+Eh] [bp-2Ah]@6
//  __int16 v14; // [sp+10h] [bp-28h]@6

//  v3 = this;
//  gTicks = (*((_DWORD *)&Sys + 1) >> 3) / 0xF4240u;
//  vE000E014 = 1000 * gTicks - 1;
//  vE000E018 = 0;
//  vE000E010 = 1;
//  v4 = (void *)g_Timers[*((_BYTE *)this + 20)];
//  v5 = Timer::ClockCmd((Timer *)*((_BYTE *)this + 20), 1, a3);
//  v6 = RCC_GetPCLK(v5);
//  if ( (unsigned int)v4 & 0x10000 )
//    v6 = RCC_GetPCLK2();
//  v7 = 1000;
//  v8 = 2 * v6 / 0x3E8u;
//  *((_BYTE *)v3 + 21) = 0;
//  while ( v8 >= 0x10000 )
//  {
//    v7 *= 2;
//    v8 >>= 1;
//    ++*((_BYTE *)v3 + 21);
//  }
//  TIM_TimeBaseStructInit(&v12);
//  v14 = v7 - 1;
//  v12 = v8 - 1;
//  v13 = 0;
//  TIM_TimeBaseInit(v4, &v12);
//  TIM_ITConfig(v4, 1);
//  TIM_ClearFlag(v4, 1);
//  v11 = 3186074850269404185LL;
//  v9 = *((_BYTE *)&v11 + *((_BYTE *)v3 + 20));
//  TInterrupt::SetPriority((TInterrupt *)&Interrupt, v9, 0);
//  TInterrupt::Activate((TInterrupt *)&Interrupt, v9, (void (__cdecl *)(unsigned __int16, void *))TTime::OnHandler, v4);
//  return TIM_Cmd(v4, 1);
}

extern uint systickcnt;
// ��ǰ�δ�ʱ��
uint TTime::CurrentTicks()const
{
    return systickcnt;
	
//	 return vE000E014 - vE000E018;
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
	
//	unsigned int v1; // r3@1

//  v1 = *(_WORD *)(g_Timers[*((_BYTE *)this + 20)] + 36);
//  if ( *((_BYTE *)this + 21) )
//    v1 >>= *((_BYTE *)this + 21);
//  return *((_DWORD *)this + 2) + v1;
}

// ����ʱ��
void TTime::SetTime(UInt64 seconds)
{
//	TTime *v3; // r6@1
//  int v4; // r4@1
//  int result; // r0@3

//  v3 = this;
//  v4 = a3;
//  if ( a3 >= 0xE7791F700LL )
//    v4 = a3 - 2006054656;
//  *((_DWORD *)this + 4) = v4 - *(_DWORD *)this;
//  result = *((_DWORD *)this + 8);
//  if ( result )
//    result = (*((int (**)(void))v3 + 8))();
//  return result;
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
	
//	int v3; // r8@1
//  int v4; // r4@1
//  bool *v5; // r5@1
//  __int64 v6; // r0@3
//  __int64 v7; // r6@3
//  unsigned int v8; // r0@15
//  unsigned int v9; // r1@15
//  bool v10; // cf@15

//  v3 = result;
//  v4 = a2;
//  v5 = a3;
//  if ( a2 > 0 )
//  {
//    LODWORD(v6) = TTime::Current((TTime *)result);
//    v7 = v6 + v4;
//    result = *(_DWORD *)(v3 + 36);
//    if ( result && v4 >= 10 )
//    {
//      while ( v4 >= 10 )
//      {
//        (*(void (__fastcall **)(int))(v3 + 36))(v4);
//        if ( v5 )
//        {
//          result = *v5;
//          if ( !*v5 )
//            break;
//        }
//        result = TTime::Current((TTime *)v3);
//        v4 = v7 - result;
//      }
//    }
//    if ( v4 )
//    {
//      if ( !v5 || (result = *v5) != 0 )
//      {
//        while ( 1 )
//        {
//          v8 = TTime::Current((TTime *)v3);
//          v10 = v9 >= HIDWORD(v7) + (unsigned int)(v8 < (unsigned int)v7);
//          result = v8 - v7;
//          if ( v10 )
//            break;
//          if ( v5 )
//          {
//            result = *v5;
//            if ( !*v5 )
//              break;
//          }
//        }
//      }
//    }
//  }
//  return result;
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
	
	
	
	
//	TTime *v2; // r10@1
//  int v3; // r4@1
//  int v4; // r5@5
//  int v5; // r9@7
//  unsigned int v6; // r7@7
//  int v7; // r0@7

//  v2 = (TTime *)result;
//  v3 = a2;
//  if ( a2 > 0 )
//  {
//    if ( a2 > 100 )
//      v3 = a2 - 1;
//    v4 = TTime::Current((TTime *)result);
//    if ( v3 >= 1000 )
//    {
//      v4 += v3 / 1000;
//      v3 %= 1000;
//    }
//    v5 = TTime::CurrentTicks(v2);
//    v6 = v5 + TTime::UsToTicks(v2, v3);
//    v7 = TTime::UsToTicks(v2, 0x3E7u);
//    if ( v6 >= v7 )
//    {
//      ++v4;
//      v6 -= v7;
//    }
//    while ( 1 )
//    {
//      result = TTime::Current(v2);
//      if ( result - v4 > 0 )
//        break;
//      if ( result == v4 )
//      {
//        result = TTime::CurrentTicks(v2);
//        if ( result >= v6 )
//          break;
//      }
//    }
//  }
//  return result;
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
	
	
	
//	unsigned int result; // r0@2

//  if ( a2 )
//    result = a2 / gTicks;
//  else
//    result = 0;
//  return result;
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
	
	
//	unsigned int result; // r0@2

//  if ( a2 )
//    result = a2 * gTicks;
//  else
//    result = 0;
//  return result;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
TimeWheel::TimeWheel(uint ms)
{
//	TimeWheel *v2; // r4@1

//  v2 = this;
//  *((_WORD *)this + 4) = 0;
//  TimeWheel::Reset(this, a2);
//  return v2;
}

void TimeWheel::Reset(uint ms)
{
//	TimeWheel *v2; // r5@1
//  unsigned int v3; // r4@1
//  __int64 v4; // r0@1
//  __int64 result; // r0@1

//  v2 = this;
//  v3 = a2;
//  LODWORD(v4) = TTime::Current((TTime *)&Time);
//  result = v4 + v3;
//  *(_QWORD *)v2 = result;
//  return result;
}
// �Ƿ��ѹ���
bool TimeWheel::Expired()
{
    return true;
	
	TimeWheel *v1; // r4@1
//  unsigned int v2; // r5@1
//  int v3; // r1@1
//  signed int result; // r0@2

//  v1 = this;
//  v2 = TTime::Current((TTime *)&Time);
//  if ( (unsigned int)(*(_QWORD *)v1 >> 32) >= v3 + (unsigned int)((unsigned int)*(_QWORD *)v1 < v2) )
//  {
//    if ( *((_WORD *)v1 + 4) )
//      TSys::Sleep((TSys *)&Sys, *((_WORD *)v1 + 4));
//    result = 0;
//  }
//  else
//  {
//    result = 1;
//  }
//  return result;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
    this->Reset();
	
//TimeCost *v1; // r4@1

//  v1 = this;
//  TimeCost::Reset(this);
//  return v1;
}

void TimeCost::Reset()
{
//	TimeCost *v1; // r4@1
//  __int64 v2; // r0@1
//  int result; // r0@1

//  v1 = this;
//  LODWORD(v2) = TTime::Current((TTime *)&Time);
//  *(_QWORD *)v1 = v2;
//  result = TTime::CurrentTicks((TTime *)&Time);
//  *((_DWORD *)v1 + 2) = result;
//  return result;
}
// ��ȥ��ʱ�䣬΢��
int TimeCost::Elapsed()const
{
    uint ret = 12;
    ret -= this->Start;
    return ret;
	
	
//	 TimeCost *v1; // r4@1
//  signed int v2; // r5@1
//  int v3; // r6@1
//  int result; // r0@2

//  v1 = this;
//  v2 = TTime::CurrentTicks((TTime *)&Time) - *((_DWORD *)this + 2);
//  v3 = TTime::Current((TTime *)&Time) - *(_DWORD *)v1;
//  if ( v2 <= 0 )
//  {
//    if ( v3 > 0 )
//      result = 1000 * v3 - TTime::TicksToUs((TTime *)&Time, -v2);
//    else
//      result = 0;
//  }
//  else
//  {
//    result = TTime::TicksToUs((TTime *)&Time, v2) + 1000 * v3;
//  }
//  return result;
}

void TimeCost::Show(cstring format)const
{
    printf("ִ�� %d ΢��\r\n", 12);
	
//	const char *v2; // r4@1
//  int v3; // r0@3

//  v2 = a2;
//  if ( !a2 )
//    v2 = (const char *)dword_210;
//  v3 = TimeCost::Elapsed(this);
//  return SmartOS_printf(v2, v3);
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
