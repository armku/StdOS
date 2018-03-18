#include "Pwm.h"

//#define _PWM_TEST_H
#ifdef _PWM_TEST_H

//呼吸灯例程
Pwm pwm(Timer3);
Delegate < Timer & > abc;
void macTIMx_IRQHandler(void);
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
void tim2refesh(void *param)
{
	static uint8_t pwm_index = 0; //用于PWM查表
	static uint8_t period_cnt = 0; //用于计算周期数

	period_cnt++;
	if (period_cnt >= 10)
		//若输出的周期数大于10，输出下一种脉冲宽的PWM波
	{

		pwm.SetPulse(2, indexWave[pwm_index]);
		pwm_index++; //标志PWM表的下一个元素

		if (pwm_index >= 40)
			//若PWM脉冲表已经输出完成一遍，重置PWM查表标志
		{
			pwm_index = 0;
		}

		period_cnt = 0; //重置周期计数标志
	}
}

void breathledtest()
{
	abc.Bind(tim2refesh);
	pwm.Enabled[0] = false;
	pwm.Enabled[1] = false;
	pwm.Enabled[2] = true;
	pwm.Enabled[3] = false;
	pwm.Pulse[0] = 0;
	pwm.Pulse[1] = 0;
	pwm.Pulse[2] = 0;
	pwm.Pulse[3] = 0;
	pwm.Register(abc);
	pwm.Period = 255; //默认1秒一次
	pwm.Prescaler = 1999;
	pwm.Open();
}

#endif // _PWM_TEST_H
