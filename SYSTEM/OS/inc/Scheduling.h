#pragma once
#include "Type.h"
#include "Thread_T.h"

#define PROCESSCNT 10	//任务数量

class CTaskScheduler
{
    public:
        CTaskScheduler();
        ~CTaskScheduler();
    public:
        uint ticks; //形同运行滴答数
    private:
        ushort	NextID; //下一个ID
        CThread thread[PROCESSCNT];
    public:
        void ThreadAdd(void(*callback)(void), uint intervalms, uint delaycntms = 0); //延时执行时间
        void TimeTick(); //间隔1ms调用一次
        void Routin(); //运行
};
