#include "Pwm.h"
#include "stm32f10x.h"


PWM::PWM(PinPort pin, uint frq, uint duty): Port(pin)
{
    this->freq = frq;
    this->duty = duty;
}

void PWM::Init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    switch (this->pin)
    {
		//TIM2_CH1
		case PA0:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel1 */
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
            TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //当定时器计数值小于CCR1_Val时为高电平
            TIM_OC1Init(TIM2, &TIM_OCInitStructure); //使能通道2
            TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM2, ENABLE); //使能定时器2
            break;
        //TIM2_CH2
		case PA1:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel2 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道2的电平跳变值，输出另外一个占空比的PWM
            TIM_OC2Init(TIM2, &TIM_OCInitStructure); //使能通道2
            TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM2, ENABLE); //使能定时器3	
            break;
        //TIM2_CH3
		case PA2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel3 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道3的电平跳变值，输出另外一个占空比的PWM
            TIM_OC3Init(TIM2, &TIM_OCInitStructure); //使能通道2
            TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM2, ENABLE); //使能定时器2
            break;
        //TIM2_CH4
		case PA3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel4 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道4的电平跳变值，输出另外一个占空比的PWM
            TIM_OC4Init(TIM2, &TIM_OCInitStructure); //使能通道4
            TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
            TIM_ARRPreloadConfig(TIM2, ENABLE); // 使能TIM2重载寄存器ARR
            /* TIM2 enable counter */
            TIM_Cmd(TIM2, ENABLE); //使能定时器2	
            break;
        //TIM3_CH1
		case PA6:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel1 */
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
            TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //当定时器计数值小于CCR1_Val时为高电平
            TIM_OC1Init(TIM3, &TIM_OCInitStructure); //使能通道1
            TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //使能定时器3	
            break;
        //TIM3_CH2
		case PA7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel2 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道2的电平跳变值，输出另外一个占空比的PWM
            TIM_OC2Init(TIM3, &TIM_OCInitStructure); //使能通道2
            TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //使能定时器3	
            break;
        //TIM3_CH3
		case PB0:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel3 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道3的电平跳变值，输出另外一个占空比的PWM
            TIM_OC3Init(TIM3, &TIM_OCInitStructure); //使能通道3
            TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //使能定时器3	
            break;
        //TIM3_CH4
		case PB1:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel4 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道4的电平跳变值，输出另外一个占空比的PWM
            TIM_OC4Init(TIM3, &TIM_OCInitStructure); //使能通道4
            TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
            TIM_ARRPreloadConfig(TIM3, ENABLE); // 使能TIM3重载寄存器ARR
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //使能定时器3	
            break;
        //TIM4_CH1
		case PB6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel1 */
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
            TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //当定时器计数值小于CCR1_Val时为高电平
            TIM_OC1Init(TIM4, &TIM_OCInitStructure); //使能通道1
            TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
            /* TIM4 enable counter */
            TIM_Cmd(TIM4, ENABLE); //使能定时器4	
            break;
		//TIM4_CH2
		case PB7:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel2 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道2的电平跳变值，输出另外一个占空比的PWM
            TIM_OC2Init(TIM4, &TIM_OCInitStructure); //使能通道4
            TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM4, ENABLE); //使能定时器4	
            break;
		//TIM4_CH3
		case PB8:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel3 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道4的电平跳变值，输出另外一个占空比的PWM
            TIM_OC3Init(TIM4, &TIM_OCInitStructure); //使能通道4
            TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM4, ENABLE); //使能定时器4	
            break;
		//TIM4_CH4
		case PB9:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM信号电平跳变值 */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel4 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //设置通道4的电平跳变值，输出另外一个占空比的PWM
            TIM_OC4Init(TIM4, &TIM_OCInitStructure); //使能通道4
            TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
            TIM_ARRPreloadConfig(TIM4, ENABLE); // 使能TIM4重载寄存器ARR
            /* TIM4 enable counter */
            TIM_Cmd(TIM4, ENABLE); //使能定时器4
            break;
		default:
            break;
    }

    this->SetModeAF_PP();
}

//设置输出值
void PWM::SetOut(ushort pwmValue)
{
    this->duty = pwmValue;
    switch (this->pin)
    {
        //TIM2_CH1
		case PA0:
			TIM2->CCR1=this->duty;
            break;
		//TIM2_CH2
		case PA1:
			TIM2->CCR2=this->duty;
            break;
		//TIM2_CH3
		case PA2:
			TIM2->CCR3=this->duty;
            break;
		//TIM2_CH4
		case PA3:
			TIM2->CCR4=this->duty;
            break;
		//TIM3_CH1
		case PA6:
			TIM3->CCR1=this->duty;
            break;
        //TIM3_CH2
		case PA7:
			TIM3->CCR2=this->duty;
            break;
        //TIM3_CH3
		case PB0:
			TIM3->CCR3=this->duty;
            break;
        //TIM3_CH4
		case PB1:
			TIM3->CCR4=this->duty;
            break;
        //TIM4_CH1
		case PB6:
			TIM3->CCR1=this->duty;
            break;
		//TIM4_CH2
		case PB7:
			TIM3->CCR2=this->duty;
            break;
		//TIM4_CH3
		case PB8:
			TIM3->CCR3=this->duty;
            break;
		//TIM4_CH4
		case PB9:
			TIM3->CCR4=this->duty;
            break;
		default:
            break;
    }
}

//设置输出比例，0-100
void PWM::SetOutPercent(float per)
{
    this->duty = this->freq *per / 100;
    this->SetOut(this->duty);
}
