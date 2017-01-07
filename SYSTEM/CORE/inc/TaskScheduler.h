#pragma once

#include <stdio.h>
#include "Type.h"
#include "Task.h"

template<class Task,int width>
class FixedArray;


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
            // �������񣬷��������š�dueTime�״ε���ʱ��us��period���ȼ��us��-1��ʾ������һ��
        uint Add(Action func, void *param, ulong dueTime = 0, long period = 0);
         
        void Remove(uint taskid);

        void Start();
        void Stop();
        // ִ��һ��ѭ����ָ��������ʱ��
        void Execute(uint usMax);

        static void ShowStatus(void *param); // ��ʾ״̬

        Task *operator[](int taskid);
};
