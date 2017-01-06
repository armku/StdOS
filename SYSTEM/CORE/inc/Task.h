#pragma once

#include "Type.h"

#define NULL 0
#define string char
#define debug_printf printf

class TaskScheduler;
// ����
class Task
{
    private:
        TaskScheduler *_Scheduler;

        friend class TaskScheduler;

        Task(TaskScheduler *scheduler);

    public:
        uint ID; // ���
        #if 0
            Action Callback; // �ص�
        #endif 
        void *Param; // ����
        long Period; // ����us
        ulong NextTime; // ��һ��ִ��ʱ��
        uint Times; // ִ�д���
        uint CpuTime; // �ܺķ�ʱ��
        uint SleepTime; // ��ǰ˯��ʱ��
        uint Cost; // ƽ��ִ��ʱ��
        bool Enable; // �Ƿ�����
        byte Reversed[3]; // �����������������

        //~Task();

        void ShowStatus(); // ��ʾ״̬
};
