#pragma once

#include <stdio.h>
#include "Type.h"
#include "Task.h"
#include "FixedArray.h"

// 任务调度器
class TaskScheduler
{
    private:        
        FixedArray < Task, 32 > _Tasks;       
        uint mgid; // 总编号

        friend class Task;

    public:
        char* Name; // 系统名称
        int Count; // 任务个数
        Task *Current; // 正在执行的任务
        bool Running; // 是否正在运行
        byte Reversed[3]; // 保留，避免对齐问题

        TaskScheduler(char* name = NULL);
        ~TaskScheduler();        
            // 创建任务，返回任务编号。dueTime首次调度时间us，period调度间隔us，-1表示仅处理一次
        uint Add(Action func, void *param, long dueTime = 0, long period = 0,const char *name="No Name");         
        void Remove(uint taskid);
		void SetTask(uint taskid,bool onoff,long delaytime=0);//设置任务执行、就绪状态
        void Start();
        void Stop();
	// 执行一次循环。指定最大可用时间
        void Execute(uint usMax);
		Task *operator[](int taskid);
	private:        
		static	void ShowTime(void * param);//显示时间
        static void ShowStatus(void *param); // 显示状态
        
};
