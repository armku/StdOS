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
    this->OnClose();
}

void Timer::OnInterrupt()
{
    if (this->OnTick)
    {
        ((Action)this->OnTick.Method)(this->OnTick.Target);
    }
}
