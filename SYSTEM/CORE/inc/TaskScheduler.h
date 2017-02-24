#pragma once

#include <stdio.h>
#include "Type.h"
#include "Task.h"
#include "FixedArray.h"

// ���������
class TaskScheduler
{
    private:        
        FixedArray < Task, 32 > _Tasks;       
        uint mgid; // �ܱ��

        friend class Task;

    public:
        char* Name; // ϵͳ����
        int Count; // �������
        Task *Current; // ����ִ�е�����
        bool Running; // �Ƿ���������
        byte Reversed[3]; // �����������������

        TaskScheduler(char* name = NULL);
        ~TaskScheduler();        
            // �������񣬷��������š�dueTime�״ε���ʱ��us��period���ȼ��us��-1��ʾ������һ��
        uint Add(Action func, void *param, long dueTime = 0, long period = 0,const char *name="No Name");         
        void Remove(uint taskid);
		void SetTask(uint taskid,bool onoff,long delaytime=0);//��������ִ�С�����״̬
        void Start();
        void Stop();
	// ִ��һ��ѭ����ָ��������ʱ��
        void Execute(uint usMax);
		Task *operator[](int taskid);
	private:        
		static	void ShowTime(void * param);//��ʾʱ��
        static void ShowStatus(void *param); // ��ʾ״̬
        
};
