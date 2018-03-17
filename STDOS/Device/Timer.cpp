#include "Timer.h"
#include "TInterrupt.h"

Timer **Timers = NULL; // 已经实例化的定时器对象

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //默认1秒一次	  
	this->OnInit();
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
