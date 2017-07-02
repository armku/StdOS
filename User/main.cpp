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

Delegate<Timer&> abc;
void macTIMx_IRQHandler(void);
void tim2refesh(void * param)
{
	macTIMx_IRQHandler();
}
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
	Pwm pwm(Timer3);
	abc.Bind(tim2refesh);
	pwm.Register(abc);
	pwm.Open();

    //Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

    Sys.Start();
}

#include "stm32f10x.h"
/* LED亮度等级 PWM表 */
uint8_t indexWave[] = 
{
    1, 1, 2, 2, 3, 4, 6, 8, 10, 14, 19, 25, 33, 44, 59, 80, 107, 143, 191, 255, 255, 191, 143, 107, 80, 59, 44, 33, 25, 19, 14, 10, 8, 6, 4, 3, 2, 2, 1, 1
};

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
