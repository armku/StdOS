#include <stdio.h>
#include "Task.h"

TaskOld::TaskOld()
{
	this->taskCls();
}
//启动
void TaskOld::Start()
{
	this->isStart = true;
}
//间隔10ms调用一次
void TaskOld::TimeTick() 
{
    if (this->isStart)
    {
        Node *pnode = this->nodeHead;
        while (pnode != 0)
        {
            pnode->data.TickCur++;
            pnode = pnode->pNext;
        }
    }	
}
//运行
void TaskOld::Routin() 
{
    if (this->isStart)
    {
        Node *pnode = this->nodeHead;
        while (pnode != 0)
        {
            if (pnode->data.TickCur > pnode->data.periodMs)
            {
                pnode->data.callback();
                pnode->data.TickCur = 0;
            }
            pnode = pnode->pNext;
        }
    }
}
/*
添加任务，参数分别是：任务函数、参数、首次时间、间隔时
间、名称。返回值是一个 uint 的任务唯一编号。	
 */
uint TaskOld::AddTask(void(*callback)(void),void* para, uint firstms, int periodms, const char *name)
{
    Node *nodeNew = new Node(); //新版链表

    nodeNew->data.TickCur = 0;
    nodeNew->data.canRun = 1;
    nodeNew->data.firstMs = firstms;
    nodeNew->data.periodMs = periodms;
    nodeNew->data.ID = this->nodeCount;
    nodeNew->data.Name = name;
    nodeNew->data.callback = callback;
    if (this->nodeHead == 0)
    {
        this->nodeHead = nodeNew;
        this->nodeLast = this->nodeHead;
    }
    else
    {
        this->nodeLast->pNext = nodeNew;
        this->nodeLast = nodeNew;
    }
	#if DEBUG
    printf("Sys::添加%02d: ", this->nodeCount++);
    printf(name);
	printf(" First=%dms Period=%dms",firstms,periodms);
    printf("\n");
	#endif
	return this->nodeCount;
}
/*
设置任务参数
*/
void TaskOld::SetTask(uint taskid,bool onoff,int delayms)
{
	Node *tsk=this->findTask(taskid);
	if(tsk!=0)
	{
		//add code
	}
}
//查找任务
Node *TaskOld::findTask(uint taskid)
{
	Node *ret=0;
	
	return ret;
}
//初始化
void TaskOld::taskCls()
{
    this->nodeCount = 0;
    this->nodeHead = 0;
    this->nodeLast = this->nodeHead;
    this->isStart = false;
}

