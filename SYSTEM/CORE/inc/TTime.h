#pragma once

#include "Type.h"

class TTime
{
	public:
		TTime();
		uint Ms();//返回当前毫秒数
		void SetTime(UInt64 seconds);//设置系统当前时间,Unix格式秒数
	public:
		uint Current;//当前毫秒数
		uint CurrentTicks;//滴答数。比微妙还短的超高精度时间
	
};

extern TTime Time;//系统时间，不建议用户直接使用
