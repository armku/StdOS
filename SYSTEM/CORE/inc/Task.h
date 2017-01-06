#pragma once

#include "Type.h"

#define NULL 0
#define string char
#define debug_printf printf

class TaskScheduler;
// 任务
class Task
{
    private:
        TaskScheduler *_Scheduler;

        friend class TaskScheduler;

        Task(TaskScheduler *scheduler);

    public:
        uint ID; // 编号
        #if 0
            Action Callback; // 回调
        #endif 
        void *Param; // 参数
        long Period; // 周期us
        ulong NextTime; // 下一次执行时间
        uint Times; // 执行次数
        uint CpuTime; // 总耗费时间
        uint SleepTime; // 当前睡眠时间
        uint Cost; // 平均执行时间
        bool Enable; // 是否启用
        byte Reversed[3]; // 保留，避免对齐问题

        //~Task();

        void ShowStatus(); // 显示状态
};
