#pragma once

#include "Delegate.h"

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
        Action Callback; // �ص�
        void *Param; // ����
		const char* Name;//����
        long Period; // ����us
        UInt64 NextTime; // ��һ��ִ��ʱ��
        uint Times; // ִ�д���
        uint CpuTime; // �ܺķ�ʱ��
        uint SleepTime; // ��ǰ˯��ʱ��
        uint Cost; // ƽ��ִ��ʱ��
        bool Enable; // �Ƿ�����
		bool operator==(Task& tsk);
        byte Reversed[3]; // �����������������

        //~Task();

        void ShowStatus(); // ��ʾ״̬
};
