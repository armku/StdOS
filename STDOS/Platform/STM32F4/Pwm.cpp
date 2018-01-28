#include "Pwm.h"
#include "Platform\stm32.h"

void Pwm::Open()
{
    /*
    REMAP
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
     */
    TIM_OCInitTypeDef TIM_OCInitStructure;

    Timer::Open();

    /* PWMģʽ���� */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�����
    TIM_OCInitStructure.TIM_Pulse = 0; //���ó�ʼPWM������Ϊ0	
    TIM_OCInitStructure.TIM_OCPolarity = this->Polarity ? TIM_OCPolarity_Low : TIM_OCPolarity_High; //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ

    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            if (this->Enabled[0])
            {
                TIM_OC1Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }
            if (this->Enabled[1])
            {
                TIM_OC2Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }
            if (this->Enabled[2])
            {
                TIM_OC3Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }
            if (this->Enabled[3])
            {
                TIM_OC4Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }

            TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIM3���ؼĴ���ARR
            break;
        case Timer3:
            if (this->Enabled[0])
            {
                TIM_OC1Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }
            if (this->Enabled[1])
            {
                TIM_OC2Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }
            if (this->Enabled[2])
            {
                TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }
            if (this->Enabled[3])
            {
                TIM_OC4Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
                TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
            }

            TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIM3���ؼĴ���ARR
            break;
        case Timer4:
                if (this->Enabled[0])
                {
                    TIM_OC1Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }
                if (this->Enabled[1])
                {
                    TIM_OC2Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }

                if (this->Enabled[2])
                {
                    TIM_OC3Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }
                if (this->Enabled[3])
                {
                    TIM_OC4Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }
                TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIM3���ؼĴ���ARR
            break;
        case Timer5:
                if (this->Enabled[0])
                {
                    TIM_OC1Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }
                if (this->Enabled[1])
                {
                    TIM_OC2Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }
                if (this->Enabled[2])
                {
                    TIM_OC3Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }
                if (this->Enabled[3])
                {
                    TIM_OC4Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
                    TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
                }
                TIM_ARRPreloadConfig(TIM5, ENABLE); //ʹ��TIM3���ؼĴ���ARR
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

void Pwm::Close()
{
    Timer::Close();
}

// ˢ�����
void Pwm::Flush(){}
void Pwm::Config()
{
    Timer::Config();
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            if (this->Enabled[0])
            {
                this->Ports[0] = new AlternatePort(PA0, false, true);
            }
            if (this->Enabled[1])
            {
                this->Ports[1] = new AlternatePort(PA1, false, true);
            }
            if (this->Enabled[2])
            {
                this->Ports[2] = new AlternatePort(PA2, false, true);
            }
            if (this->Enabled[3])
            {
                this->Ports[3] = new AlternatePort(PA3, false, true);
            }
            break;
        case Timer3:
            if (this->Enabled[0])
            {
                this->Ports[0] = new AlternatePort(PA6, false, true);
            }
            if (this->Enabled[1])
            {
                this->Ports[1] = new AlternatePort(PA7, false, true);
            }
            if (this->Enabled[2])
            {
                this->Ports[2] = new AlternatePort(PB0, false, true);
            }
            if (this->Enabled[3])
            {
                this->Ports[3] = new AlternatePort(PB1, false, true);
            }
            break;
        case Timer4:
            if (this->Enabled[0])
            {
                this->Ports[0] = new AlternatePort(PB6, false, true);
            }
            if (this->Enabled[1])
            {
                this->Ports[1] = new AlternatePort(PB7, false, true);
            }
            if (this->Enabled[2])
            {
                this->Ports[2] = new AlternatePort(PB8, false, true);
            }
            if (this->Enabled[3])
            {
                this->Ports[3] = new AlternatePort(PB9, false, true);
            }
            break;
        case Timer5:
            if (this->Enabled[0])
            {
                this->Ports[0] = new AlternatePort();
            }
            if (this->Enabled[1])
            {
                this->Ports[1] = new AlternatePort();
            }
            if (this->Enabled[2])
            {
                this->Ports[2] = new AlternatePort();
            }
            if (this->Enabled[3])
            {
                this->Ports[3] = new AlternatePort();
            }
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

void PwmData::OnInterrupt(){

}
void PwmData::Config(){}
////////////////////////////////////////����Ϊ���
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
			break;
        case Timer5:
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
#if 0
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
#endif
