#include <stdio.h>
#include "Task.h"

Task::Task()
{
    this->Init();
}
//���10ms����һ��
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
//����
void Task::Routin() 
{
    if (this->isStart)
    {
        Node *pnode = this->nodeHead;
        while (pnode != 0)
        {
            if (pnode->data.TickCur > pnode->data.intervalms)
            {
                pnode->data.callback();
                pnode->data.TickCur = 0;
            }
            pnode = pnode->pNext;
        }
    }
}

void Task::AddTask(void(*callback)(void), uint delaycntms, uint intervalms, const char *name)
{
    Node *nodeNew = new Node(); //�°�����

    nodeNew->data.TickCur = 0;
    nodeNew->data.canRun = 1;
    nodeNew->data.delaycnt = delaycntms;
    nodeNew->data.intervalms = intervalms;
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
    printf("�������%02d: ", this->nodeCount++);
    printf(name);
    printf("\n");
}

//��ʼ��
void Task::Init()
{
    this->nodeCount = 0;
    this->nodeHead = 0;
    this->nodeLast = this->nodeHead;
    this->isStart = false;
}

//��ʼ
void Task::Start()
{
    this->isStart = true;
}
