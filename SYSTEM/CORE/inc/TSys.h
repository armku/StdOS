#pragma once

#include "Type.h"
#include "Object.h"

/* 串口定义 */

typedef enum
{
	COM1=0,COM2,COM3,COM4,COM5,COM_NONE=0XFF
}MessagePort_T;//端口号
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

//class Task
//{
//    public:
//        Task();
//        

//};

class TSys:public Object
{
	public:
		TSys(uint clock =72000000,MessagePort_T messagePort=COM1);
		void Init();//初始化
		void Start();//启动系统任务调度，该函数内部为死循环。
		void Show(bool newLine=false) const;
		void ShowInfo();//显示系统信息
		uint64_t Ms();//系统启动以来的毫秒数，无符号长整型8字节
		uint Seconds();//系统绝对UTC时间，整型4字节，Unix格式，1970年以来的总秒数。
		void Delay(uint us);//微妙级延迟，常用于高精度外设信号控制
		void Sleep(uint ms);//毫秒级睡眠，常用于业务层杂宁等待一定时间
		void Reboot(uint msDelay);//异步热重启系统。延迟一定毫秒数执行。
		uint AddTask(void(*callback)(void),void* para,uint delaycntms, uint intervalms,const char* name = "未命名任务");//临时用
		void Remove(uint taskid);//删除任务
	public:
		uint Clock;//系统时钟
		MessagePort_T MessagePort;//调试接口
	public:
		uint64_t ms;//系统启动以来的毫秒数
		uint seconds;//系统绝对UTC时间，Unix格式，1970年以来的总秒数。
		byte ID[12];
		ushort FlashSize;
	
	
	
	public:
		uint AddTask(void(*callback)(void), uint firstms, uint periodms, const char *name = "未命名任务"); //延时执行时间
        void TimeTick(); //间隔1ms调用一次
        void Routin(); //运行
        void Start1(); //开始
    protected:
        //链表测试
        Node *nodeHead; //第一个节点
        Node *nodeLast; //最后一个链表节点
        uint nodeCount; //任务节点数量
        void Init1(); //初始化
    private:
        bool isStart; //是否启动
	
};
extern TSys Sys;//系统参数
