#pragma once

#include "Type.h"
#include "Object.h"
#include "Task.h"
#include "TTime.h"

/* 串口定义 */

typedef enum
{
	COM1=0,COM2,COM3,COM4,COM5,COM_NONE=0XFF
}COM_Def;//端口号
typedef enum
{
    AIN = 0x0,  //模拟输入 
    INPUT = 0x04,  //输入
    INPUT_PD = 0x28,  //浮空输入
    INPUT_PU = 0x48,  //推挽式输入
    OUTPUT_OD = 0x14,  //开漏输出
    OUTPUT_PP = 0x10,  //推挽式输出
    AF_OD = 0x1C,  //开漏复用输出
    AF_PP = 0x18  //推挽复用输出
} PIN_MODE;
#ifndef NULL
	#define NULL 0
#endif
#define STM32F1    //使用STM32F1系列单片机
/* 针脚 ------------------------------------------------------------------*/
//引脚定义
typedef enum
{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
	PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
	PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
	PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
	PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
	P0 = 0XFFFF
}Pin;

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
		TSys(uint clock =72000000,COM_Def messagePort=COM1);
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
		COM_Def MessagePort;//调试接口
		byte Inited;//串口用参数
	public:
		byte ID[12];
		ushort FlashSize;
	private:
		Task task;	
	
};
extern TSys Sys;//系统参数
extern TTime Time; //系统时间，不建议用户直接使用
