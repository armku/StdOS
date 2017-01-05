#pragma once

#include <stdio.h>
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

// ���������
class TaskScheduler
{
    private:
        #if 0
            FixedArray < Task, 32 > _Tasks;
        #endif 
        uint _gid; // �ܱ��

        friend class Task;

    public:
        string Name; // ϵͳ����
        int Count; // �������
        Task *Current; // ����ִ�е�����
        bool Running; // �Ƿ���������
        byte Reversed[3]; // �����������������

        TaskScheduler(string name = NULL);
        ~TaskScheduler();
        #if 0
            // �������񣬷��������š�dueTime�״ε���ʱ��us��period���ȼ��us��-1��ʾ������һ��
            uint Add(Action func, void *param, ulong dueTime = 0, long period = 0);
        #endif 
        void Remove(uint taskid);

        void Start();
        void Stop();
        // ִ��һ��ѭ����ָ��������ʱ��
        void Execute(uint usMax);

        static void ShowStatus(void *param); // ��ʾ״̬

        Task *operator[](int taskid);
};
