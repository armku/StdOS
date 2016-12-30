#include <stdio.h>
#include "Task.h"

Task::Task()
{
    this->Init();
}
//间隔10ms调用一次
void Task::TimeTick() 
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
void Task::Routin() 
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

void Task::AddTask(void(*callback)(void), uint firstms, uint periodms, const char *name)
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
    printf("Sys::添加%02d: ", this->nodeCount++);
    printf(name);
	printf(" First=%dms Period=%dms",firstms,periodms);
    printf("\n");
}

//初始化
void Task::Init()
{
    this->nodeCount = 0;
    this->nodeHead = 0;
    this->nodeLast = this->nodeHead;
    this->isStart = false;
}

//开始
void Task::Start()
{
    this->isStart = true;
}
