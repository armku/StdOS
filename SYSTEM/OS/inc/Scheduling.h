#pragma once
#include "Type.h"
#include "Thread_T.h"

#define PROCESSCNT 10	//��������

class CTaskScheduler
{
    public:
        CTaskScheduler();
        ~CTaskScheduler();
    public:
        uint ticks; //��ͬ���еδ���
    private:
        ushort	NextID; //��һ��ID
        CThread thread[PROCESSCNT];
    public:
        void ThreadAdd(void(*callback)(void), uint intervalms, uint delaycntms = 0); //��ʱִ��ʱ��
        void TimeTick(); //���1ms����һ��
        void Routin(); //����
};
