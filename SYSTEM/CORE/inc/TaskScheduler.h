#pragma once

#include <stdio.h>
#include "Type.h"
#include "Task.h"

template<class Task,int width>
class FixedArray;


// 任务调度器
class TaskScheduler
{
    private:
        #if 0
            FixedArray < Task, 32 > _Tasks;
        #endif 
        uint _gid; // 总编号

        friend class Task;

    public:
        string Name; // 系统名称
        int Count; // 任务个数
        Task *Current; // 正在执行的任务
        bool Running; // 是否正在运行
        byte Reversed[3]; // 保留，避免对齐问题

        TaskScheduler(string name = NULL);
        ~TaskScheduler();        
            // 创建任务，返回任务编号。dueTime首次调度时间us，period调度间隔us，-1表示仅处理一次
        uint Add(Action func, void *param, ulong dueTime = 0, long period = 0);
         
        void Remove(uint taskid);

        void Start();
        void Stop();
        // 执行一次循环。指定最大可用时间
        void Execute(uint usMax);

        static void ShowStatus(void *param); // 显示状态

        Task *operator[](int taskid);
};
