#pragma once
#include "Type.h"

#define PROCESSCNT 20	//任务数量

//仅限内部使用
class CThread
{
	public:
		CThread();
		uint TickCur;//当前计数值	
		uint intervalms;//回调间隔
		bool canRun;//可以执行
		uint delaycnt;//延时时间		
	public:
		void (*callback)(void);//回调函数
		void checkRun();//判断是否运行
		void SetPara(void (*callback)(void),uint intervalms,uint delaycntms=0);//设置参数
};

typedef struct Thread
{
	uint TickCur;//当前计数值	
	uint intervalms;//回调间隔
	bool canRun;//可以执行
	uint delaycnt;//延时时间	
	void (*callback)(void);//回调函数
	Thread *pnext;
	
}Thread_T;

class Task
{
    public:
        Task();
        uint ticks; //形同运行滴答数
    private:
        ushort	NextID; //下一个ID
        CThread thread[PROCESSCNT];
    public:
        void AddTask(void(*callback)(void), uint delaycntms, uint intervalms); //延时执行时间
        void TimeTick(); //间隔1ms调用一次
        void Routin(); //运行
};
