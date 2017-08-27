#include "Pwm.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

// index ��ʱ�����
Pwm::Pwm(TIMER index):Timer(index)
{
	
}
Pwm::~Pwm()
{
	
}

/*
TIM2 PA0,PA1,PA2,PA3
TIM3 PA6,PA7,PB0,PB1
TIM4 PB6,PB7,PB8,PB9
TIM5 
remap

TIM1 PE9,PE11,PE13,PE14
TIM2 PA15,PB3,PB10,PB11
TIM3 PC6,PC7,PC8,PC9
TIM4 PD12,PD13,PD14,PD15
TIM5 
*/


void Pwm::SetPulse(int idx, ushort pulse)
{
	if(idx>3)
	{
		return;
	}
	this->Pulse[idx]=pulse;
	switch (this->_index)
	{
		case Timer1:
            break;
        case Timer2:
            switch(idx)
			{
				case 0:
					TIM2->CCR1=this->Pulse[idx];//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
					break;
				case 1:
					TIM2->CCR2=this->Pulse[idx];
					break;
				case 2:
					TIM2->CCR3=this->Pulse[idx];
					break;
				case 3:
					TIM2->CCR4=this->Pulse[idx];
					break;
				default:
					break;
			}
			break;
        case Timer3:			
			switch(idx)
			{
				case 0:
					TIM3->CCR1=this->Pulse[idx];//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
					break;
				case 1:
					TIM3->CCR2=this->Pulse[idx];
					break;
				case 2:
					TIM3->CCR3=this->Pulse[idx];
					break;
				case 3:
					TIM3->CCR4=this->Pulse[idx];
					break;
				default:
					break;
			}
            break;
        case Timer4:
			#if defined(STM32F1) || defined(STM32F4)
            switch(idx)
			{
				case 0:
					TIM4->CCR1=this->Pulse[idx];//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
					break;
				case 1:
					TIM4->CCR2=this->Pulse[idx];
					break;
				case 2:
					TIM4->CCR3=this->Pulse[idx];
					break;
				case 3:
					TIM4->CCR4=this->Pulse[idx];
					break;
				default:
					break;
			}
			#elif defined STM32F0
			#endif
			break;
        case Timer5:
			#if defined(STM32F1) || defined(STM32F4)
            switch(idx)
			{
				case 0:
					TIM5->CCR1=this->Pulse[idx];//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
					break;
				case 1:
					TIM5->CCR2=this->Pulse[idx];
					break;
				case 2:
					TIM5->CCR3=this->Pulse[idx];
					break;
				case 3:
					TIM5->CCR4=this->Pulse[idx];
					break;
				default:
					break;
			}
			#elif defined STM32F0
			#endif
			break;
        case Timer6:
            break;
        case Timer7:
            break;
        case Timer8:
            break;
        case Timer9:
            break;
        case Timer10:
            break;
        case Timer11:
            break;
        case Timer12:
            break;
        case Timer13:
            break;
        case Timer14:
            break;
        case Timer15:
            break;
        case Timer16:
            break;
        case Timer17:
            break;
        case Timer18:
            break;
        default:
			break;
	}
}
// ����ռ�ձȣ�0~255
void Pwm::SetDuty(int idx, byte duty)
{
}
byte Pwm::GetDuty(int idx)
{
	return 0;
}
PwmData::PwmData(TIMER index):Pwm(index)
{
	
}
//����������
#if 0
Pwm pwm(Timer3);
Delegate<Timer&> abc;
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
void tim2refesh(void * param)
{
	static uint8_t pwm_index = 0; //����PWM���
    static uint8_t period_cnt = 0; //���ڼ���������

    period_cnt++;
    if (period_cnt >= 10)
    //�����������������10�������һ��������PWM��
    {

        pwm.SetPulse(2,indexWave[pwm_index]);
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
	pwm.Enabled[0]=false;
	pwm.Enabled[1]=false;
	pwm.Enabled[2]=true;
	pwm.Enabled[3]=false;
	pwm.Pulse[0]=0;
	pwm.Pulse[1]=0;
	pwm.Pulse[2]=0;
	pwm.Pulse[3]=0;
	pwm.Register(abc);
	pwm.Period = 255; //Ĭ��1��һ��
    pwm.Prescaler = 1999;
	pwm.Open();
}
#endif
