#include "Timer.h"
#include "TInterrupt.h"

Timer **Timers = NULL; // 已经实例化的定时器对象

void Timer::SetHandler(bool set){

}

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //默认1秒一次
	#if defined(STM32F0)
		this->Prescaler = 47999; //48000分频
	#elif defined(STM32F1)
		this->Prescaler=36000-1;//则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=系统时钟/(分频系数+1) =1M
		this->Period=2000-1;
	#elif defined(STM32F4)
		this->Prescaler = 5000-1;
		this->Period= 8400-1;
	#endif    
}

Timer::~Timer()
{
    if (Opened)
        Close();
    #if 0
        if (_Handler)
            Register(NULL);
    #endif 
    Timers[_index] = NULL;
}

void Timer::Open() // 开始定时器
{
    this->Config();
    this->OnOpen();    
}

void Timer::Close() // 停止定时器
{
    if (!Opened)
        return ;

    debug_printf("Timer%d::Stop\r\n", _index + 1);

    // 关闭时钟
    ClockCmd(_index, false);
    //    TIM_ITConfig(_port, TIM_IT_Update, DISABLE);
    //    TIM_ClearITPendingBit(_port, TIM_IT_Update); // 仅清除中断标志位 关闭不可靠
    //    TIM_Cmd(_port, DISABLE);
    this->OnClose();
}

void Timer::OnInterrupt()
{
    if (this->OnTick)
    {
        ((Action)this->OnTick.Method)(this->OnTick.Target);
    }
}

//定时器测试
#if 0
    extern uint time2cnt;
    Delegate < Timer & > abc;
    void tim2refesh(void *param)
    {
        time2cnt++;
    }
    Timer *timer2;
    void TimeTest()
    {
        // 初始化为输出
        timer2 = new Timer(Timer7);
        abc.Bind(tim2refesh);
        timer2->Register(abc);
        timer2->Open();
        //	timer2->SetFrequency(1);
        //	timer2->Config();
    }
#endif
