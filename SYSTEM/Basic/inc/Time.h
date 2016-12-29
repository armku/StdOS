/*
系统时间 Time
Time 是系统时间 TTime 类的全局对象，不建议用户层直接使用。
Time.Current 当前毫秒数。同 Sys.Ms。
Time.CurrentTicks 嘀嗒数。比微秒还短的超高精度时间。
Time.SetTime(UInt64 seconds) 设置系统当前时间，Unix 格式秒数。
*/
#pragma once

#include "Type.h"

class CTime
{
	public:
		uint64_t Current;
		uint64_t CurrentTicks;
		void SetTime(UInt64 seconds);
};
