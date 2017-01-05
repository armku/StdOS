#include <stdio.h>
#include "Task.h"

TaskOld::TaskOld()
{
	this->taskCls();
}
//����
void TaskOld::Start()
{
	this->isStart = true;
}
//���10ms����һ��
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
//����
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
������񣬲����ֱ��ǣ����������������״�ʱ�䡢���ʱ
�䡢���ơ�����ֵ��һ�� uint ������Ψһ��š�	
 */
uint TaskOld::AddTask(void(*callback)(void),void* para, uint firstms, int periodms, const char *name)
{
    Node *nodeNew = new Node(); //�°�����

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
    printf("Sys::���%02d: ", this->nodeCount++);
    printf(name);
	printf(" First=%dms Period=%dms",firstms,periodms);
    printf("\n");
	#endif
	return this->nodeCount;
}
/*
�����������
*/
void TaskOld::SetTask(uint taskid,bool onoff,int delayms)
{
	Node *tsk=this->findTask(taskid);
	if(tsk!=0)
	{
		//add code
	}
}
//��������
Node *TaskOld::findTask(uint taskid)
{
	Node *ret=0;
	
	return ret;
}
//��ʼ��
void TaskOld::taskCls()
{
    this->nodeCount = 0;
    this->nodeHead = 0;
    this->nodeLast = this->nodeHead;
    this->isStart = false;
}

