#pragma once
#include "Type.h"
#include "Thread_T.h"

#define PROCESSCNT 20	//��������

class Task
{
    public:
        Task();
    public:
        uint ticks; //��ͬ���еδ���
    private:
        ushort	NextID; //��һ��ID
        CThread thread[PROCESSCNT];
    public:
        void AddTask(void(*callback)(void), uint delaycntms, uint intervalms); //��ʱִ��ʱ��
        void TimeTick(); //���1ms����һ��
        void Routin(); //����
};
