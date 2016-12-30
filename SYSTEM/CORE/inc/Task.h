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
};
