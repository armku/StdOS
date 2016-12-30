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

class Task
{
    public:
        Task();
        uint AddTask(void(*callback)(void), uint firstms, uint periodms, const char *name = "δ��������"); //��ʱִ��ʱ��
        void TimeTick(); //���1ms����һ��
        void Routin(); //����
        void Start(); //��ʼ
    protected:
        //�������
        Node *nodeHead; //��һ���ڵ�
        Node *nodeLast; //���һ������ڵ�
        uint nodeCount; //����ڵ�����
        void Init(); //��ʼ��
    private:
        bool isStart; //�Ƿ�����

};
