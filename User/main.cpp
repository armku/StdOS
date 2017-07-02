#include <stdio.h>
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\Button.h"
#include "Port.h"
#include "WatchDog.h"
#include "SString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "Pwm.h"
//#include "Modbus\Modbus1.h"
#include "Spi.h"
#include "RTC.h"
#include "Flash.h"
#include "Drivers\lcd_dr.h"
#include "TInterrupt.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
#if 1
    AlternatePort led1(PB0, false, true);
    //OutputPort led2(PF7);
    //OutputPort led3(PF8);
#else 
    OutputPort led1(PD0);
    OutputPort led2(PD1);
    OutputPort led3(PD2);
#endif 

void LedTask(void *param)
{
    //OutputPort *leds = (OutputPort*)param;
    //*leds = ! * leds;
    //    led2 = key0;
}

#define namee "StdOS"
//PWM ledLCD(PD12);
void TIMx_Breathing_Init(void);
int main(void)
{
    TSys &sys = (TSys &)(Sys);

    sys.Name = (char*)namee;
    byte aa = vers[0];
    aa = aa;

    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
    TIMx_Breathing_Init();

    //Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

    Sys.Start();
}

#include "stm32f10x.h"
/* LED���ȵȼ� PWM�� */
uint8_t indexWave[] = 
{
    1, 1, 2, 2, 3, 4, 6, 8, 10, 14, 19, 25, 33, 44, 59, 80, 107, 143, 191, 255, 255, 191, 143, 107, 80, 59, 44, 33, 25, 19, 14, 10, 8, 6, 4, 3, 2, 2, 1, 1
};

static void NVIC_Config_PWM(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* ����TIM3_IRQ�ж�Ϊ�ж�Դ */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> �ж� & TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 *
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * 
 */

/*    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 */
static void TIMx_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;


    /* ����TIM3CLK ʱ��Ϊ72MHZ */
    //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 					//ʹ��TIM3ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* ������ʱ������ */
    TIM_TimeBaseStructure.TIM_Period = 255; //����ʱ����0������255����Ϊ266�Σ�Ϊһ����ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = 1999; //����Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWMģʽ���� */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�����
    TIM_OCInitStructure.TIM_Pulse = 0; //���ó�ʼPWM������Ϊ0	
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ

    TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3


    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	

    TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIM3���ؼĴ���ARR

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //ʹ��update�ж�

    NVIC_Config_PWM(); //�����ж����ȼ�		

}

void TIMx_Breathing_Init(void)
{
    TIMx_Mode_Config();
}

/* �������жϷ����� */
void macTIMx_IRQHandler(void)
{
    static uint8_t pwm_index = 0; //����PWM���
    static uint8_t period_cnt = 0; //���ڼ���������

    period_cnt++;
    if (period_cnt >= 10)
    //�����������������10�������һ��������PWM��
    {

        TIM3->CCR3 = indexWave[pwm_index]; //����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
        pwm_index++; //��־PWM�����һ��Ԫ��

        if (pwm_index >= 40)
        //��PWM������Ѿ�������һ�飬����PWM����־
        {
            pwm_index = 0;
        }

        period_cnt = 0; //�������ڼ�����־
    }
}
