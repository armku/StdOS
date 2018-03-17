#include "Timer.h"
#include "TInterrupt.h"

Timer **Timers = NULL; // �Ѿ�ʵ�����Ķ�ʱ������

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //Ĭ��1��һ��	  
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

void Timer::Open() // ��ʼ��ʱ��
{
    this->Config();
    this->OnOpen();    
}

void Timer::Close() // ֹͣ��ʱ��
{
    if (!Opened)
        return ;

    debug_printf("Timer%d::Stop\r\n", _index + 1);

    // �ر�ʱ��
    ClockCmd(_index, false);
    //    TIM_ITConfig(_port, TIM_IT_Update, DISABLE);
    //    TIM_ClearITPendingBit(_port, TIM_IT_Update); // ������жϱ�־λ �رղ��ɿ�
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
