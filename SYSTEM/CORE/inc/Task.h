#pragma once

#include "Type.h"

typedef struct Thread_T
{
    uint TickCur; //当前计数值	
    uint periodMs; //回调间隔
    bool canRun; //可以执行
    uint firstMs; //延时时间	
    uint ID; //编号
    const char *Name; //名称
    void(*callback)(void); //回调函数	
} Thread_T;

struct Node
{
    Thread_T data; //数据
    Node *pNext; //下一节点
};

class Task
{
	public:
		Task();
};
