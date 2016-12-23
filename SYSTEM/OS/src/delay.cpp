#include "sys.h"
#include "delay.h"
#include "Data.h"

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    static uint8_t fac_us = 0; //us延时倍乘数
    //static uint16_t fac_ms = 0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数

    //systick中断服务函数,使用ucos时用到
    void SysTick_Handler(void)
    {
        Scheduling.TimeTick();
    }

    //初始化延迟函数
    //SYSTICK的时钟固定为HCLK时钟的1/8
    //SYSCLK:系统时钟
    void delay_init()
    {
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
        fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8
        //fac_ms = (uint16_t)fac_us * 1000;					//非OS下,代表每个ms需要的systick时钟数
        SysTick_Config(SystemCoreClock / delay_ostickspersec); //tick is 1ms		
    }
    //延时nus
    //nus为要延时的us数.
    void delay_us(uint32_t nus)
    {
        uint32_t ticks;
        uint32_t told, tnow, tcnt = 0;
        uint32_t reload = SysTick->LOAD; //LOAD的值
        ticks = nus * fac_us; //需要的节拍数
        tcnt = 0;
        told = SysTick->VAL; //刚进入时的计数器值
        while (1)
        {
            tnow = SysTick->VAL;
            if (tnow != told)
            {
                if (tnow < told)
                    tcnt += told - tnow;
                //这里注意一下SYSTICK是一个递减的计数器就可以了.
                else
                    tcnt += reload - tnow + told;
                told = tnow;
                if (tcnt >= ticks)
                    break;
                //时间超过/等于要延迟的时间,则退出.
            }
        };
    }
    //延时nms
    //nms:要延时的ms数
    void delay_ms(uint16_t nms)
    {

        while (nms--)
        {
            delay_us(1000);
        }
        //  delay_us((uint32_t)(nms * 1000.0));
    }
    #ifdef __cplusplus
    }
#endif
