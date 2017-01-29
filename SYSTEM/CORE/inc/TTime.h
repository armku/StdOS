#pragma once

#include "Type.h"

class TTime
{
	public:
		TTime();
		static UInt64 Ms();//系统启动以来的毫秒数
		uint Seconds();//系统时间
		void SetTime(UInt64 seconds);//设置系统当前时间,Unix格式秒数
		void Sleep(uint ms);//延时ms		
		
		static UInt64 Current;//系统启动以来的毫秒数	
		UInt64 CurrentTicks;//滴答数。比微妙还短的超高精度时间		
		uint TicksToUs(uint ticks) const;
		uint UsToTicks(uint us) const;
	
};

extern TTime Time;//系统时间，不建议用户直接使用
