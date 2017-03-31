#pragma once

#include "Delegate.h"

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
        Action Callback; // 回调
        void *Param; // 参数
		const char* Name;//名称
        long Period; // 周期us
        UInt64 NextTime; // 下一次执行时间
        uint Times; // 执行次数
        uint CpuTime; // 总耗费时间
        uint SleepTime; // 当前睡眠时间
        uint Cost; // 平均执行时间
        bool Enable; // 是否启用
		bool operator==(Task& tsk);
        byte Reversed[3]; // 保留，避免对齐问题

        //~Task();

        void ShowStatus(); // 显示状态
};
