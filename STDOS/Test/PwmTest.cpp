#include "Pwm.h"

//#define _PWM_TEST_H
#ifdef _PWM_TEST_H

//����������
Pwm pwm(Timer3);
Delegate < Timer & > abc;
void macTIMx_IRQHandler(void);
/* LED���ȵȼ� PWM�� */
uint8_t indexWave[] =
{
	1, 1, 2, 2, 3, 4, 6, 8, 10, 14, 19, 25, 33, 44, 59, 80, 107, 143, 191, 255, 255, 191, 143, 107, 80, 59, 44, 33, 25, 19, 14, 10, 8, 6, 4, 3, 2, 2, 1, 1
};

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
void tim2refesh(void *param)
{
	static uint8_t pwm_index = 0; //����PWM���
	static uint8_t period_cnt = 0; //���ڼ���������

	period_cnt++;
	if (period_cnt >= 10)
		//�����������������10�������һ��������PWM��
	{

		pwm.SetPulse(2, indexWave[pwm_index]);
		pwm_index++; //��־PWM�����һ��Ԫ��

		if (pwm_index >= 40)
			//��PWM������Ѿ�������һ�飬����PWM����־
		{
			pwm_index = 0;
		}

		period_cnt = 0; //�������ڼ�����־
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
	pwm.Period = 255; //Ĭ��1��һ��
	pwm.Prescaler = 1999;
	pwm.Open();
}

#endif // _PWM_TEST_H
