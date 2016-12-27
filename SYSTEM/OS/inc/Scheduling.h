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
        uint32_t ticks; //��ͬ���еδ���
    private:
        uint16_t NextID; //��һ��ID
        CThread thread[PROCESSCNT];
    public:
        void ThreadAdd(void(*callback)(void), uint32_t intervalms, uint32_t delaycntms = 0); //��ʱִ��ʱ��
        void TimeTick(); //���1ms����һ��
        void Routin(); //����
};
