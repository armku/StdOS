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
/* LED亮度等级 PWM表 */
uint8_t indexWave[] = 
{
    1, 1, 2, 2, 3, 4, 6, 8, 10, 14, 19, 25, 33, 44, 59, 80, 107, 143, 191, 255, 255, 191, 143, 107, 80, 59, 44, 33, 25, 19, 14, 10, 8, 6, 4, 3, 2, 2, 1, 1
};

static void NVIC_Config_PWM(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* 配置TIM3_IRQ中断为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> 中断 & TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 *
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 
 */

/*    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 */
static void TIMx_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;


    /* 设置TIM3CLK 时钟为72MHZ */
    //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 					//使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* 基本定时器配置 */
    TIM_TimeBaseStructure.TIM_Period = 255; //当定时器从0计数到255，即为266次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Prescaler = 1999; //设置预分频：
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM模式配置 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出
    TIM_OCInitStructure.TIM_Pulse = 0; //设置初始PWM脉冲宽度为0	
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //当定时器计数值小于CCR1_Val时为低电平

    TIM_OC3Init(TIM3, &TIM_OCInitStructure); //使能通道3


    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载	

    TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIM3重载寄存器ARR

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE); //使能定时器3	

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能update中断

    NVIC_Config_PWM(); //配置中断优先级		

}

void TIMx_Breathing_Init(void)
{
    TIMx_Mode_Config();
}

/* 呼吸灯中断服务函数 */
void macTIMx_IRQHandler(void)
{
    static uint8_t pwm_index = 0; //用于PWM查表
    static uint8_t period_cnt = 0; //用于计算周期数

    period_cnt++;
    if (period_cnt >= 10)
    //若输出的周期数大于10，输出下一种脉冲宽的PWM波
    {

        TIM3->CCR3 = indexWave[pwm_index]; //根据PWM表修改定时器的比较寄存器值
        pwm_index++; //标志PWM表的下一个元素

        if (pwm_index >= 40)
        //若PWM脉冲表已经输出完成一遍，重置PWM查表标志
        {
            pwm_index = 0;
        }

        period_cnt = 0; //重置周期计数标志
    }
}
