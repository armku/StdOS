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
        uint32_t ticks; //形同运行滴答数
    private:
        uint16_t NextID; //下一个ID
        CThread thread[PROCESSCNT];
    public:
        void ThreadAdd(void(*callback)(void), uint32_t intervalms, uint32_t delaycntms = 0); //延时执行时间
        void TimeTick(); //间隔1ms调用一次
        void Routin(); //运行
};
