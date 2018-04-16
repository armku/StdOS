#include "Timer.h"
#include "TInterrupt.h"

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //Ĭ��1��һ��	  
	this->OnInit();
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
