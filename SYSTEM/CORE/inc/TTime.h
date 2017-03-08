#pragma once

#include "Type.h"

class TTime
{
	private:
		static void OnHandler(ushort num,void* param);
	public:
		uint seconds;//全局秒数，系统启动后总秒数，累加
		UInt64 Milliseconds;//全局毫秒数，系统启动后总毫秒(1000毫秒整部分，累加
		uint BaseSeconds;//基准秒数，系统启动时相对于1970年的秒数，时间调节，加上Seconds得到当前时间Now();
		byte Index;//定时器
		#if !(defined(STM32F0)||(GD32F130))
		byte Div;//分频系数，最大分频时无法让大于64M主频的芯片分配得到1k时钟
		#endif
		#if 0
		Func OnInit;
	Func OnLoad;
	Func OnSave;
	#endif
	typedef void (*FuncInt)(int);
	#if 0
	Function OnSleep;
	#endif
	
	TTime();
	public:
		
	
		//以上为新内容
		UInt64 Ms() const;//系统启动以来的毫秒数
		UInt64 Current() const;//系统启动以来的毫秒
		uint Seconds();//系统时间
		void SetTime(UInt64 seconds);//设置系统当前时间,Unix格式秒数
		void Sleep(uint us);//延时us		
					
		uint TicksToUs(uint ticks) const;
		uint UsToTicks(uint us) const;
	
		UInt64 mCurrent;//系统启动以来的毫秒数	
		UInt64 CurrentTicks;//滴答数。比微妙还短的超高精度时间		
};
extern TTime Time; //系统时间，不建议用户直接使用
