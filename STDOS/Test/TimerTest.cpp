#include "Timer.h"
#include "TInterrupt.h"




//��ʱ������
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
        // ��ʼ��Ϊ���
        timer2 = new Timer(Timer7);
        abc.Bind(tim2refesh);
        timer2->Register(abc);
        timer2->Open();
        //	timer2->SetFrequency(1);
        //	timer2->Config();
    }
#endif
