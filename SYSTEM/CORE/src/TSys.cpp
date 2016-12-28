/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "stdio.h"
#include "TSys.h"
#include "Delay.h"
#include "Scheduling.h"

Task Scheduling; //调度

TSys Sys;//系统参数

TSys::TSys(uint clock, MessagePort_T messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
}

void TSys::Show(bool newLine)const{

}

//初始化
void TSys::Init(){

}
//启动系统任务调度，该函数内部为死循环。*在此之间，添加的所有任务函数将得不到调度，所有睡眠方法无效！
void TSys::Start()
{
	while(true)
	{
		Scheduling.Routin();
	}	
}

//显示系统信息
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:0801 Ver:0.0.6113 Build:2016-01-01\n");
    printf("STDOS::STM32F103C8 72MHz Flash:512k RAM:20k\n");
    printf("DevID:0x0414 RevID:0x1309\n");
    printf("CPUID:0x412fc231 ARM:ARMv7-M Cortex-M3: R1p2\n");
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:04-53-36-35-34-31-31-31-32-30-32-34\n");
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
	delay_ms(ms);
}
//异步热重启系统。延迟一定毫秒数执行。
void TSys::Reboot(uint msDelay){}
/*
添加任务，参数分别是：任务函数、参数、首次时间、间隔时
间、名称。返回值是一个 uint 的任务唯一编号。	
*/
uint TSys::AddTask(void(*callback)(void),void* para,uint delaycntms, uint intervalms,CString name )
{
	
	return 0;
}
//临时用
uint TSys::AddTask(void(*callback)(void),void* para,uint delaycntms, uint intervalms)
{
	Scheduling.AddTask(callback,delaycntms,intervalms);
	return 0;
}
//删除任务
void TSys::Remove(uint taskid)
{

}
