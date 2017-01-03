#pragma once

#include "Type.h"
#include "Object.h"
#include "Task.h"

/* 串口定义 */

typedef enum
{
	COM1=0,COM2,COM3,COM4,COM5,COM_NONE=0XFF
}MessagePort_T;//端口号

// 使用何种模块的宏定义
#define using(module) Sys.module.Init = T##module##_Init;

#ifdef DEBUG
    #define debug_printf printf
#else
    __inline void debug_printf( const char *format, ... ) {}
#endif

class TSys:public Object
{
	public:
		TSys(uint clock =72000000,MessagePort_T messagePort=COM1);
		void Init();//初始化
		void Start();//启动系统任务调度，该函数内部为死循环。
		virtual void Show(bool newLine=false) const;
		void ShowInfo();//显示系统信息
		uint64_t Ms();//系统启动以来的毫秒数，无符号长整型8字节
		uint Seconds();//系统绝对UTC时间，整型4字节，Unix格式，1970年以来的总秒数。
		void Delay(uint us);//微妙级延迟，常用于高精度外设信号控制
		void Sleep(uint ms);//毫秒级睡眠，常用于业务层杂宁等待一定时间
		void Reboot(uint msDelay);//异步热重启系统。延迟一定毫秒数执行。		
	public:
		uint AddTask(void(*callback)(void),void* para, uint firstms, int periodms, const char *name = "未命名任务");
		void SetTask(uint taskid,bool onoff,int delayms=0);//设置任务参数
		void Remove(uint taskid);//删除任务
		void TimeTick(); //间隔1ms调用一次
		void Routin(); //运行  
	public:
		uint Clock;//系统时钟
		MessagePort_T MessagePort;//调试接口
	public:
		byte ID[12];
		ushort FlashSize;
	private:
		Task task;	
	
};
extern TSys Sys;//系统参数
