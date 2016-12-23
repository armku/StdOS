#include "sys.h"
#include "delay.h"
#include "Data.h"

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    static uint8_t fac_us = 0; //us��ʱ������
    //static uint16_t fac_ms = 0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

    //systick�жϷ�����,ʹ��ucosʱ�õ�
    void SysTick_Handler(void)
    {
        Scheduling.TimeTick();
    }

    //��ʼ���ӳٺ���
    //SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
    //SYSCLK:ϵͳʱ��
    void delay_init()
    {
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8
        //fac_ms = (uint16_t)fac_us * 1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����
        SysTick_Config(SystemCoreClock / delay_ostickspersec); //tick is 1ms		
    }
    //��ʱnus
    //nusΪҪ��ʱ��us��.
    void delay_us(uint32_t nus)
    {
        uint32_t ticks;
        uint32_t told, tnow, tcnt = 0;
        uint32_t reload = SysTick->LOAD; //LOAD��ֵ
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
    //��ʱnms
    //nms:Ҫ��ʱ��ms��
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
