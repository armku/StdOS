#pragma once

#include "Type.h"

typedef struct Thread_T
{
    uint TickCur; //��ǰ����ֵ	
    uint periodMs; //�ص����
    bool canRun; //����ִ��
    uint firstMs; //��ʱʱ��	
    uint ID; //���
    const char *Name; //����
    void(*callback)(void); //�ص�����	
} Thread_T;

struct Node
{
    Thread_T data; //����
    Node *pNext; //��һ�ڵ�
};

class TaskOld
{
	public:
		TaskOld();
		uint AddTask(void(*callback)(void),void* para, uint firstms, int periodms, const char *name = "δ��������");
        void TimeTick(); //���1ms����һ��
		void SetTask(uint taskid,bool onoff,int delayms=0);//�����������
        void Routin(); //����        
		void Start();//����
    protected:
        //�������
        Node *nodeHead; //��һ���ڵ�
        Node *nodeLast; //���һ������ڵ�
        uint nodeCount; //����ڵ�����
        void taskCls(); //��ʼ��
		Node *findTask(uint taskid);//��������
    private:
        bool isStart; //�Ƿ�����
};
