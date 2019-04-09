#include "delay.h"

#define SysTick_LoadValue (F_CPU / 1000U) //���ݵ�ǰʱ��Ƶ���Զ�����Systick��ʼֵ

volatile static uint32_t System_ms = 0; //ϵͳʱ�Ӽ�ʱ����
static SysClock_TypeDef SysClock[] = {
    {16000000, RCC_PLLMul_2},//16MHz, 2��Ƶ
    {24000000, RCC_PLLMul_3},//24MHz, 3��Ƶ
    {32000000, RCC_PLLMul_4},//32MHz, 4��Ƶ
    {40000000, RCC_PLLMul_5},//40MHz, 5��Ƶ
    {48000000, RCC_PLLMul_6},//48MHz, 6��Ƶ
    {56000000, RCC_PLLMul_7},//56MHz, 7��Ƶ
    {64000000, RCC_PLLMul_8},//64MHz, 8��Ƶ
    {72000000, RCC_PLLMul_9},//72MHz, 9��Ƶ
    {80000000, RCC_PLLMul_10},//80MHz, 10��Ƶ
    {88000000, RCC_PLLMul_11},//88MHz, 11��Ƶ
    {96000000, RCC_PLLMul_12},//96MHz, 12��Ƶ
    {104000000, RCC_PLLMul_13},//104MHz, 13��Ƶ
    {112000000, RCC_PLLMul_14},//112MHz, 14��Ƶ
    {120000000, RCC_PLLMul_15},//120MHz, 15��Ƶ
    {128000000, RCC_PLLMul_16},//128MHz, 16��Ƶ
};

/**
  * @brief  ϵͳ�δ�ʱ����ʼ������ʱ1ms
  * @param  ��
  * @retval ��
  */
void Delay_Init(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SysTick_LoadValue);
}

/**
  * @brief  ϵͳʱ������
  * @param  fcpu:��Ƶֵ (���F_CPU_Typeö��)
  * @retval ��
  */
void SysClock_Init(F_CPU_Type fcpu)
{
    SystemCoreClock = SysClock[fcpu].F_CPU_x;
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while(RCC_GetSYSCLKSource());
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PLLCmd(DISABLE);
    RCC_HSEConfig(RCC_HSE_ON);
    RCC_WaitForHSEStartUp();
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, SysClock[fcpu].RCC_PLLMul_x);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ;
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

/**
  * @brief  ϵͳ�δ�ʱ���ж����
  * @param  ��
  * @retval ��
  */
void SysTick_Handler1(void)
{
    System_ms++;
}

/**
  * @brief  ��ȡ��Ƭ�����ϵ����������ĺ�����
  * @param  ��
  * @retval ��ǰϵͳʱ�Ӻ�����
  */
uint32_t millis(void)
{
    return System_ms;
}

/**
  * @brief  ��ȡ��Ƭ�����ϵ�����������΢����
  * @param  ��
  * @retval ��ǰϵͳʱ��΢����
  */
uint32_t micros(void)
{
    return (System_ms * 1000 + (SysTick_LoadValue - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}

/**
  * @brief  ���뼶��ʱ
  * @param  ms: Ҫ��ʱ�ĺ�����
  * @retval ��
  */
void delay_ms(uint32_t ms)
{
    uint32_t Stop_TimePoint = System_ms + ms;
    while(System_ms < Stop_TimePoint);
}

/**
  * @brief  ΢�뼶��ʱ
  * @param  us: Ҫ��ʱ��΢����
  * @retval ��
  */
void delay_us(uint32_t us)
{
    uint32_t total = 0;
    uint32_t target = CYCLES_PER_MICROSECOND * us;
    int last = SysTick->VAL;
    int now = last;
    int diff = 0;
start:
    now = SysTick->VAL;
    diff = last - now;
    if(diff > 0)
    {
        total += diff;
    }
    else
    {
        total += diff + SysTick_LoadValue;
    }
    if(total > target)
    {
        return;
    }
    last = now;
    goto start;
}
