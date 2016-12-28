#pragma once
#include "Type.h"
#include "Thread_T.h"

#define PROCESSCNT 20	//任务数量

class Task
{
    public:
        Task();
    public:
        uint ticks; //形同运行滴答数
    private:
        ushort	NextID; //下一个ID
        CThread thread[PROCESSCNT];
    public:
        void AddTask(void(*callback)(void), uint delaycntms, uint intervalms); //延时执行时间
        void TimeTick(); //间隔1ms调用一次
        void Routin(); //运行
};
