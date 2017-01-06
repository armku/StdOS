#pragma once

#include "Type.h"

typedef struct Thread_T
{
    uint TickCur; //当前计数值	
    uint periodMs; //回调间隔
    bool canRun; //可以执行
    uint firstMs; //延时时间	
    uint ID; //编号
    const char *Name; //名称
    void(*callback)(void); //回调函数	
} Thread_T;

struct Node
{
    Thread_T data; //数据
    Node *pNext; //下一节点
};

class TaskOld
{
	public:
		TaskOld();
		uint AddTask(void(*callback)(void),void* para, uint firstms, int periodms, const char *name = "未命名任务");
        void TimeTick(); //间隔1ms调用一次
		void SetTask(uint taskid,bool onoff,int delayms=0);//设置任务参数
        void Routin(); //运行        
		void Start();//启动
    protected:
        //链表测试
        Node *nodeHead; //第一个节点
        Node *nodeLast; //最后一个链表节点
        uint nodeCount; //任务节点数量
        void taskCls(); //初始化
		Node *findTask(uint taskid);//查找任务
    private:
        bool isStart; //是否启动
};
