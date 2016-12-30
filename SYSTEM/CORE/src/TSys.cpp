/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "stdio.h"
#include "TSys.h"
#include "stm32f10x.h"

#define delay_ostickspersec 1000			//时钟频率
static byte fac_us = 0; //us延时倍乘数
//static uint16_t fac_ms = 0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    void delay_us(uint nus);
    #ifdef __cplusplus
    }
#endif 




//Task Scheduling; //调度
TSys Sys; //系统参数

TSys::TSys(uint clock, MessagePort_T messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
	this->taskCls();
}

void TSys::Show(bool newLine)const{

}

//初始化
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void TSys::Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8
    //fac_ms = (uint16_t)fac_us * 1000;					//非OS下,代表每个ms需要的systick时钟数
    SysTick_Config(SystemCoreClock / delay_ostickspersec); //tick is 1ms	
	
	this->FlashSize = *(uint16_t *)(0X1FFFF7E0);  // 容量
	
	for(int i=0;i<12;i++)
	{
		this->ID[i]=*(byte*)(0X1FFFF7E8+i);
	}
	this->FlashSize=*(ushort*)(0X1FFFF7E0);
}

//启动系统任务调度，该函数内部为死循环。*在此之间，添加的所有任务函数将得不到调度，所有睡眠方法无效！
void TSys::Start()
{	
	this->isStart = true;
    while (true)
    {
        this->Routin();
    }
}

//显示系统信息
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:0801 Ver:0.0.6113 Build:2016-01-01\n");
    printf("STDOS::STM32F103C8 72MHz Flash:%dk RAM:20k\n",this->FlashSize);
    printf("DevID:0x0414 RevID:0x1309\n");
    printf("CPUID:0x412fc231 ARM:ARMv7-M Cortex-M3: R1p2\n");
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n",ID[0],ID[1],ID[2],ID[3],ID[4],ID[5],ID[6],ID[7],ID[8],ID[9],ID[10],ID[11]);
    printf("Time : 2016-12-28 10:56:32\n");
    printf("Support: http://www.armku.com\n");
}

//系统启动以来的毫秒数，无符号长整型8字节
uint64_t TSys::Ms()
{
    return this->ms;
}

//系统绝对UTC时间，整型4字节，Unix格式，1970年以来的总秒数。
uint TSys::Seconds()
{
    return this->seconds;
}

//微妙级延迟，常用于高精度外设信号控制
void TSys::Delay(uint us)
{
    delay_us(us);
}

//毫秒级睡眠，常用于业务层杂宁等待一定时间
void TSys::Sleep(uint ms)
{
    while (ms--)
    {
        this->Delay(1000);
    }
}

//异步热重启系统。延迟一定毫秒数执行。
void TSys::Reboot(uint msDelay){}
//删除任务
void TSys::Remove(uint taskid){

}
//间隔10ms调用一次
void TSys::TimeTick() 
{
    if (this->isStart)
    {
        Node *pnode = this->nodeHead;
        while (pnode != 0)
        {
            pnode->data.TickCur++;
            pnode = pnode->pNext;
        }
    }
	Sys.ms++;
}
//运行
void TSys::Routin() 
{
    if (this->isStart)
    {
        Node *pnode = this->nodeHead;
        while (pnode != 0)
        {
            if (pnode->data.TickCur > pnode->data.periodMs)
            {
                pnode->data.callback();
                pnode->data.TickCur = 0;
            }
            pnode = pnode->pNext;
        }
    }
}
/*
添加任务，参数分别是：任务函数、参数、首次时间、间隔时
间、名称。返回值是一个 uint 的任务唯一编号。	
 */
uint TSys::AddTask(void(*callback)(void),void* para, uint firstms, int periodms, const char *name)
{
    Node *nodeNew = new Node(); //新版链表

    nodeNew->data.TickCur = 0;
    nodeNew->data.canRun = 1;
    nodeNew->data.firstMs = firstms;
    nodeNew->data.periodMs = periodms;
    nodeNew->data.ID = this->nodeCount;
    nodeNew->data.Name = name;
    nodeNew->data.callback = callback;
    if (this->nodeHead == 0)
    {
        this->nodeHead = nodeNew;
        this->nodeLast = this->nodeHead;
    }
    else
    {
        this->nodeLast->pNext = nodeNew;
        this->nodeLast = nodeNew;
    }
    printf("Sys::添加%02d: ", this->nodeCount++);
    printf(name);
	printf(" First=%dms Period=%dms",firstms,periodms);
    printf("\n");
	return this->nodeCount;
}
/*
设置任务参数
*/
void TSys::SetTask(uint taskid,bool onoff,int delayms)
{
	Node *tsk=this->findTask(taskid);
	if(tsk!=0)
	{
		//add code
	}
}
//查找任务
Node *TSys::findTask(uint taskid)
{
	Node *ret=0;
	
	return ret;
}
//初始化
void TSys::taskCls()
{
    this->nodeCount = 0;
    this->nodeHead = 0;
    this->nodeLast = this->nodeHead;
    this->isStart = false;
}

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //systick中断服务函数,使用ucos时用到
    void SysTick_Handler(void)
    {
        Sys.TimeTick();		
    }

    //延时nus
    //nus为要延时的us数.
    void delay_us(uint nus)
    {
        uint ticks;
        uint told, tnow, tcnt = 0;
        uint reload = SysTick->LOAD; //LOAD的值
        ticks = nus * fac_us; //需要的节拍数
        tcnt = 0;
        told = SysTick->VAL; //刚进入时的计数器值
        while (1)
        {
            tnow = SysTick->VAL;
            if (tnow != told)
            {
                if (tnow < told)
                    tcnt += told - tnow;
                //这里注意一下SYSTICK是一个递减的计数器就可以了.
                else
                    tcnt += reload - tnow + told;
                told = tnow;
                if (tcnt >= ticks)
                    break;
                //时间超过/等于要延迟的时间,则退出.
            }
        };
    }
    #ifdef __cplusplus
    }
#endif
