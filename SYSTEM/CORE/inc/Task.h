#pragma once
#include "Type.h"

#define PROCESSCNT 20	//��������

//�����ڲ�ʹ��
class CThread
{
	public:
		CThread();
		~CThread();
	public:
		uint TickCur;//��ǰ����ֵ	
		uint intervalms;//�ص����
		bool canRun;//����ִ��
		uint delaycnt;//��ʱʱ��		
	public:
		void (*callback)(void);//�ص�����
		void checkRun();//�ж��Ƿ�����
		void SetPara(void (*callback)(void),uint intervalms,uint delaycntms=0);//���ò���
};

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
